import rospy
import numpy as np
import constants
import roles
from Moving import Moving
from Controller import Controller
from Position import Position
from geometry_msgs.msg import Twist, Pose2D
from std_msgs.msg import Int16
from std_msgs.msg import Float32
import shapely.geometry as geo
import random
from PathPlanner import PathPlanner


class Robot(Moving):
    """Class that determines movements and control for a robot
    all topics published by the robot class are published under the node
    /aimbot_(team_side)/players/ally(num)
    this is with the exception of the command node which is published for
    compatability with the simulator"""

    def __init__(self, num=0, team_side="home"):
        Moving.__init__(self)
        self.controller = Controller()
        self.num = num # player number
        self.pos = Position()
        self.des_pos = Position()  # place where we want to go
        self.ball_pos = Position()
        self.ally_pos = Position()
        self.opp1_pos = Position()
        self.opp2_pos = Position()
        self.role = 0
        self.count = 0 #TODO remove
        self.team_side = team_side
        self.vel = [0.0, 0.0, 0.0]  # (vx, vy, w)
        self.wheel_vel = [0.0,0.0,0.0] # (wheel1, wheel2, wheel3)
        self.publishers = dict()
        self.init_publsihers()
        self.hertz_20 = 1
        self.first = True
        self.state = dict()
        self.pause = 10
        self.path_planner = PathPlanner()
        self.control_ball = False
        self.change_roles = False
        self.init_state()

    def my_pos_sub(self):
        """Subscribe to my position"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"
        rospy.Subscriber(namespace + 'ally' + str(self.num), Pose2D, lambda msg: self.pos.import_msg(msg))

    def vision_sub(self):
        """Subscribe to other vision nodes"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"
        rospy.Subscriber(namespace + 'ball', Pose2D, lambda msg: self.ball_pos.import_msg(msg))
        if(self.num == "1"):
            ally_num = 2
        else:
            ally_num = 1
        rospy.Subscriber(namespace + 'ally' + str(ally_num), Pose2D, lambda msg: self.ally_pos.import_msg(msg))

        rospy.Subscriber(namespace + 'opp1', Pose2D, lambda msg: self.opp1_pos.import_msg(msg))

        rospy.Subscriber(namespace + 'opp2', Pose2D, lambda msg: self.opp2_pos.import_msg(msg))

    def my_role_sub(self):
        """Subscribe to my role"""
        namespace = "/aimbot_" + self.team_side + "/team/roles/"
        rospy.Subscriber(namespace + 'ally' + str(self.num), Int16, lambda msg: self.import_role(msg))

    def import_role(self, msg):
        """Imports my role from the msg"""
        self.role = msg.data

    def init_vel_pub(self):
        """Inits publishers for commanded velocities.  Needed by simulator."""
        name = '/' + self.team_side + str(self.num) + '/'
        self.publishers['cmd_vel'] = rospy.Publisher(name + 'command', Twist, queue_size=10)

    def init_debug_pub(self):
        """Inits publishers for debug values"""
        namespace = '/aimbot_' + self.team_side + "/players/ally" + str(self.num) + '/'
        self.publishers['des_pos'] = rospy.Publisher(namespace + 'des_pos', Pose2D, queue_size=10)
        self.publishers['obsv_pos'] = rospy.Publisher(namespace + 'obsv_pos', Pose2D, queue_size=10)
        self.publishers['robot_vel'] = rospy.Publisher(namespace + 'robot_vel', Twist, queue_size=10)
        self.publishers['wheel_vel1'] = rospy.Publisher(namespace + 'wheel_vel1', Float32, queue_size=10)
        self.publishers['wheel_vel2'] = rospy.Publisher(namespace + 'wheel_vel2', Float32, queue_size=10)
        self.publishers['wheel_vel3'] = rospy.Publisher(namespace + 'wheel_vel3', Float32, queue_size=10)

    def vel_pub(self):
        """Publishes the commanded velocities.  Needed by simulator."""
        msg = Twist()
        msg.linear.x = self.vel[0]
        msg.linear.y = self.vel[1]
        msg.angular.z = self.vel[2]
        self.publishers['cmd_vel'].publish(msg)

    def debug_pub(self):
        """Publishes the debug values"""
        pos_msg = Pose2D()
        twist_msg = Twist()
        float_msg = Float32()

        pos_msg.x = self.des_pos.x
        pos_msg.y = self.des_pos.y
        pos_msg.theta = self.des_pos.theta
        self.publishers['des_pos'].publish(pos_msg)

        pos_msg.x = self.pos.x
        pos_msg.y = self.pos.y
        pos_msg.theta = self.pos.theta
        self.publishers['obsv_pos'].publish(pos_msg)

        twist_msg.linear.x = self.vel[0]
        twist_msg.linear.y = self.vel[1]
        twist_msg.angular.z = self.vel[2]
        self.publishers['robot_vel'].publish(twist_msg)

        float_msg.data = self.wheel_vel[0]
        self.publishers['wheel_vel1'].publish(float_msg)

        float_msg.data = self.wheel_vel[1]
        self.publishers['wheel_vel2'].publish(float_msg)

        float_msg.data = self.wheel_vel[2]
        self.publishers['wheel_vel3'].publish(float_msg)

    def subscribe(self):
        """Subscribe to all nodes necessary for this robot"""
        self.vision_sub()
        self.my_pos_sub()
        self.my_role_sub()

    def init_publsihers(self):
        """Inits all publishers for this robot"""
        self.init_vel_pub()
        self.init_debug_pub()

    def publish(self):
        """Publishes all publishers for this robot"""
        self.vel_pub()
        self.debug_pub()

    def init_state(self):
        """Inits the state"""
        self.state["backup_top"] = False
        self.state["backup_bot"] = False
        self.state["num"] = 0

    def update(self):
        #need to wait until we have a des pos and a cur pos from publishers before running controller
        if(self.pos.init):
            """Updates the robots controller and sets velocities"""
            self.determine_des_pos()


            self.set_des_pos(self.path_planner.calc_waypoints(self.pos, self.des_pos))

            self.controller.update_des_pos(self.des_pos.x, self.des_pos.y, np.deg2rad(self.des_pos.theta))
            # TODO figure out how smooth vision values

            #print("Robot:update pre update_cur_pos")
            self.controller.update_cur_pos(self.pos.x, self.pos.y, np.deg2rad(self.pos.theta))
            #print("Robot:update update_cur_pos")
            self.controller.update()
            self.vel = [self.controller.vel[0], self.controller.vel[1], np.rad2deg(self.controller.vel[2])]
            self.wheel_vel = self.controller.wheel_vel

    def determine_des_pos(self):
        """Determine the desired position for the robot"""

        self.reset_state()

        if self.role <= roles.GAMEPLAY_CUTOFF:
            self.gameplay_roles()

        elif roles.GAMEPLAY_CUTOFF < self.role < roles.SET_CUTOFF:
            self.set_placement_role()

        elif self.role >= roles.SET_CUTOFF:
            self.debug_role()

        else:
            print("not a valid role")


    def gameplay_roles(self):
        """Determine desired poositon for gameplay"""

        if self.role == roles.STAY_PUT: # stay where you are
            self.stay_put()

        elif self.role == roles.SCORE:
            self.score_a_goal()

        elif self.role == roles.DEFEND_GOAL:
            self.defend_goal()

        elif self.role == roles.BACKUP_OFFENSE:
            #self.go_behind_ball_facing_goal(0.5)
            self.backup_offense()

        elif self.role == roles.FOLLOW_BALL:
            self.follow_ball_on_line(self.ball_pos.x - constants.follow_distance)

        elif self.role == roles.GET_BEHIND_BALL:
            self.get_behind_ball()

        elif self.role == roles.BOTH_GET_BEHIND_BALL:
            self.both_get_behind_ball()

    def reset_state(self):
        """Resets the states in certain conditions"""
        if(self.role != roles.BACKUP_OFFENSE):
            self.state["backup_top"] = False
            self.state["backup_bot"] = False

    def set_placement_role(self):
        """Determine desired position for set placement"""
        if self.role == 103: #reset field is true
            self.go_to(-.5, 0, 0)
            if (self.within_error(5)):
                self.stay_put()
        elif self.role == 104: #reset field is true
            self.go_to(-1.2, 0, 0)
            if (self.within_error(5)):
                self.stay_put()
        elif self.role == 105:  # penalty and home ally1
            self.go_to(-.06, 1.7, 0)
        elif self.role == 106:  # penalty and home ally2
            self.go_to(-.06, -1.7, 0)

        elif self.role == 107:  # penalty and away ally1
            self.go_to(-.06, 1.7, 0)
        elif self.role == 108:  # penalty and away ally2
            self.go_to(-.06, -1.7, 0)
        else:
            print("not a valid role")


    def debug_role(self):
        """determine desired position with debug role"""
        if self.role == 1097:
            if (self.within_error(1)):
                if self.state["num"] < 1:
                    self.state["num"] += 1
                else:
                    self.state["num"]= 0
                print(self.state["num"])
            if (self.state["num"] == 0):
                self.go_to(0, 0, 90)
            else:
                self.go_to(0, 0, -90)
        elif self.role == 1098: # debug role
            if(self.within_error(1)):
                if self.state["num"] < 1:
                    self.state["num"] += 1
                else:
                    self.state["num"] = 0
                print(self.state["num"])
            if(self.state["num"] == 0):
                self.go_to(0,0,0)
            else:
                self.go_to(1,0,0)
        elif self.role == 1099: # debug role
            if self.within_error(10):
                if self.state["num"] < 3:
                    self.state["num"] += 1
                else:
                    self.state["num"] = 0
                print(self.state["num"])

            self.move_square()
        else:
            print("not a valid role")

    def set_des_pos(self, des_x, des_y, des_th):
        """Sets the desired position"""
        self.des_pos.update(des_x, des_y, des_th)

    def follow_ball_on_line(self, line):
        theta_c = 0
        x_c = line
        y_c = self.ball_pos.y
        self.set_des_pos(x_c, y_c, theta_c)

    def rush_goal(self):
        # Use numpy to create vectors
        ballvec = np.array([[self.ball_pos.x], [self.ball_pos.y]])
        mevec = np.array([[self.pos.x], [self.pos.y]])
        field_width = 3.53
        goalvec = np.array([[field_width / 2], [0]])

        # unit vector from ball to goal
        uv = goalvec - ballvec
        uv = uv / np.linalg.norm(uv)

        # compute a position 20cm behind ball, but aligned with goal
        p = ballvec - 0.20 * uv

        # If I am sufficiently close to the point behind the ball,
        # or in other words, once I am 21cm behind the ball, just
        # drive to the goal.
        if np.linalg.norm(p - mevec) < 0.21:
            cmdvec = goalvec
        else:
            cmdvec = p

        self.set_des_pos(cmdvec.flatten()[0], cmdvec.flatten()[1], 0)

    def score_a_goal(self):
        """Attempt to score a goal"""
        self.go_behind_ball_facing_goal(0)

        intersect = self.get_intersect_robot_ball_with_goal()
        if(not intersect.is_empty): # if there is an intersection
            goal_pos = Position()
            goal_pos.update(intersect.x, intersect.y, 0)
            tol = 0.1
            in_tol = self.dis_from_point_to_line(self.pos, self.ball_pos, goal_pos) < tol
            if (self.theta_within_error(3) and in_tol):
            #if (self.theta_within_error(3)):
                self.attack_ball()

    def backup_offense(self):
        """Play a backup role on offense"""
        print("ally pos y: ", self.ally_pos.y)
        if(self.ally_pos.y > constants.backup_off_switch_thresh):
            print("switch to backup bot")
            self.state["backup_top"] = False
            self.state["backup_bot"] = True

        if(self.ally_pos.y < -1 * constants.backup_off_switch_thresh):
            print("switch to backup top")
            self.state["backup_top"] = True
            self.state["backup_bot"] = False

        if(self.state["backup_top"]): # backup on the +y side
            des_y = constants.field_height / 2 + self.ally_pos.y
        elif(self.state["backup_bot"]):
            des_y = -1 * constants.field_height / 2 + self.ally_pos.y
        elif(self.ally_pos.y >= 0):
            print("b top", self.state["backup_top"])
            print("b bot", self.state["backup_bot"])
            self.state["backup_top"] = False
            self.state["backup_bot"] = True
            des_y = -1 * constants.field_height / 2 + self.ally_pos.y
        else:
            print("b top", self.state["backup_top"])
            print("b bot", self.state["backup_bot"])
            self.state["backup_top"] = True
            self.state["backup_bot"] = False
            des_y = constants.field_height / 2 + self.ally_pos.y

        goal_pos = Position()
        goal_pos.x = constants.field_width / 2
        goal_pos.y = 0

        self.face_target(des_y, goal_pos, constants.follow_distance)

    def face_target(self, des_y, target, des_distance_from_target):
        """face the target at the given y value"""
        if(des_distance_from_target > np.sqrt(2) * abs(des_y) ):
            des_x = np.sqrt(np.power(des_distance_from_target, 2) - np.power(des_y, 2))
        else:
            des_x = des_distance_from_target / np.sqrt(2)
            des_y = des_x * np.sign(des_y)
        des_theta = self.get_angle_between_points(des_x, des_y, target.x, target.y)
        self.set_des_pos(des_x, des_y, np.rad2deg(des_theta))

    def go_behind_ball_facing_goal(self, des_distance_from_ball):
        """Get behind the ball facing the goal"""
        # score_y = self.ball_pos.y
        # if (score_y >= constants.goal_y_thresh):
        #     score_y = constants.goal_y_thresh
        # elif (score_y <= -1 * constants.goal_y_thresh):
        #     score_y = -1 * constants.goal_y_thresh
        # theta = self.get_angle_between_points(self.ball_pos.x, self.ball_pos.y, constants.field_width/2, score_y)
        # hypotenuse = (constants.robot_width / 2)+ des_distance_from_ball
        # x_c = self.ball_pos.x - hypotenuse * np.cos(theta)
        # y_c = self.ball_pos.y - hypotenuse * np.sin(theta)
        # theta = np.rad2deg(theta)
        #
        # self.set_des_pos(x_c, y_c, theta)
        theta = self.get_angle_between_points(self.ball_pos.x, self.ball_pos.y, constants.field_width / 2, 0)
        intersect = self.get_intersect_robot_ball_with_goal()
        if (not intersect.is_empty):  # if there is an intersection
            theta = self.get_angle_between_points(self.pos.x, self.pos.y, self.ball_pos.x, self.ball_pos.y)
        hypotenuse = (constants.robot_width / 2) + des_distance_from_ball
        x_c = self.ball_pos.x - hypotenuse * np.cos(theta)
        y_c = self.ball_pos.y - hypotenuse * np.sin(theta)
        theta = np.rad2deg(theta)

        self.set_des_pos(x_c, y_c, theta)

    def get_intersect_robot_ball_with_goal(self):
        """Gets the intersect between the line from robot to ball and the goal"""
        ball_line = self.get_extrapoled_line(self.pos.to_Point(), self.ball_pos.to_Point())

        goal_x = constants.field_width / 2
        goal_y = constants.goal_y_thresh
        goal_line = geo.LineString([(goal_x, goal_y), (goal_x, -1 * goal_y)])
        intersect = ball_line.intersection(goal_line)
        return intersect

    def both_get_behind_ball(self):
        """Both robots get behind the ball"""
        des_y = 0
        if(self.ball_pos.y >= 0):
            des_y = self.ball_pos.y - constants.get_behind_ball_space
        else:
            des_y = self.ball_pos.y + constants.get_behind_ball_space
        self.set_des_pos(-1 * constants.field_width / 2, des_y, 0)


    def get_behind_ball(self):
        """Single robot avoid the ball and then get behind it"""
        pos = Position()
        pos.x = -1 * constants.field_width / 2
        pos.y = self.pos.y
        pos.theta = 0
        if(abs(pos.y - self.ball_pos.y) < constants.get_behind_ball_space):
            if (self.ball_pos.y >= self.pos.y):
                pos.y = self.ball_pos.y - constants.get_behind_ball_space
            else:
                pos.y = self.ball_pos.y + constants.get_behind_ball_space
            pos = self.thresh_by_field_size(pos)
        self.set_des_pos(pos.x, pos.y, pos.theta)

    def ball_is_further_in(self):
        """Return whether the ball is further in"""
        return ((self.ball_pos.y >= 0) and (self.pos.y > self.ball_pos.y)
            or (self.ball_pos.y < 0 and self.pos.y < self.ball_pos.y))


    def control_ball_facing_target(self):
        """Get behind the ball facing the goal"""
        field_width = 3.53
        theta = self.get_angle_between_points(self.ball_pos.x, self.ball_pos.y, field_width/2, 0)
        robot_width = 0.175  # (7.0 in)
        robot_half_width = robot_width / 2
        hypotenuse = robot_half_width
        x_c = self.ball_pos.x - hypotenuse * np.cos(theta)
        y_c = self.ball_pos.y - hypotenuse * np.sin(theta)
        theta = np.rad2deg(theta)

        self.set_des_pos(x_c, y_c, theta)

    def attack_ball(self):
        """
        Simply pushes the ball along the "vector" from robot to ball
        """
        theta = self.get_angle_between_points(self.pos.x, self.pos.y, self.ball_pos.x, self.ball_pos.y)
        kick_dist = 0.1524  # (6.0in)
        x_c = self.ball_pos.x + kick_dist * np.cos(theta)
        y_c = self.ball_pos.y + kick_dist * np.sin(theta)
        theta_c = np.rad2deg(theta)

        self.set_des_pos(x_c, y_c, theta_c)

    def defend_goal(self):
        theta_c = 0
        x_c = (constants.robot_width / 2) - constants.field_width/2
        y_c = self.ball_pos.y
        if(y_c > constants.goal_box_width / 2):
            y_c = constants.goal_box_width / 2
        elif(y_c < -1* constants.goal_box_width / 2):
            y_c = -1 * constants.goal_box_width / 2

        self.set_des_pos(x_c, y_c, theta_c)

    def get_distance_between_points(self, x1, y1, x2, y2):
        a = x2 - x1
        b = y2 - y1
        c = np.sqrt(a ** 2 + b ** 2)
        return c

    def get_angle_between_points(self, x1, y1, x2, y2):
        a = x2 - x1
        b = y2 - y1
        theta = np.arctan2(b, a)
        return theta

    def dis_from_point_to_line(self, point_pos, pos1, pos2):
        """Gets the distance from (x0, y0) to the line that goes through (x1,y1) and (x2,y2)"""
        #return abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1) / np.sqrt((y2 - y1) ^ 2 + (x2 - x1) ^ 2)
        return abs((pos2.y - pos1.y)*point_pos.x - (pos2.x - pos1.x)*point_pos.y + pos2.x*pos1.y - pos2.y*pos1.x) / np.sqrt(np.power(pos2.y-pos1.y, 2) + np.power(pos2.x - pos1.x, 2))

    def go_to(self, x, y, theta):
        """Go to the x, y and theta position given as parameters"""
        self.set_des_pos(x, y, theta)

    def stay_put(self):
        """Stay in the spot the robot is current int"""
        self.go_to(self.pos.x,self.pos.y, self.pos.theta);

    def move_to_center(self):
        return self.go_to(0, 0, 0)

    def move_square(self):
        # actions
        square_size = 0.4
        if (self.state == 0):  # going to upper right
            state = 1
            self.go_to(square_size, square_size, 0)
        elif (self.state == 1):  # going to lower right
            state = 2
            self.go_to(square_size, -square_size, -90)
        elif (self.state == 2):  # going to lower left
            state = 3
            self.go_to(-square_size, -square_size, -180)
        elif (self.state == 3):  # going to upper left
            state = 0
            self.go_to(-square_size, square_size, 90)

    def tolerance(self, pos, desired, error):
        # if(pos >= 0):
        return pos >= (desired - error) and pos <= (desired + error)
        # else:
        # return pos <= (desired - error) and pos >= (desired + error)

    def within_error(self, errorPercent):
        """Returns if the robot is at it's desired position within the error percentage"""
        field_width = 3.53 #in meters
        xy_offset = field_width * errorPercent / 100
        x_good = abs(self.pos.x - self.des_pos.x) < xy_offset
        y_good = abs(self.pos.y - self.des_pos.y) < xy_offset

        return x_good and y_good and self.theta_within_error(errorPercent)

    def theta_within_error(self, errorPercent):
        """Returns if the robots theta is within the error percentage"""
        th_offset = 360 * errorPercent / 100
        effective_th = self.pos.theta
        if (self.pos.theta - self.des_pos.theta > 360):
            effective_th = self.pos.theta - 360
        elif (self.des_pos.theta - self.pos.theta > 360):
            effective_th = self.pos.theta + 360
        th_good = abs(effective_th - self.des_pos.theta) < th_offset
        return th_good

    def get_extrapoled_line(self, p1, p2):
        """Creates a line extrapoled in p1->p2 direction"""
        dist = p1.distance(p2)
        EXTRAPOL_RATIO = constants.field_width * 2 / dist
        a = p1
        b = geo.Point(p1.x + EXTRAPOL_RATIO * (p2.x - p1.x), p1.y + EXTRAPOL_RATIO * (p2.y - p1.y))
        return geo.LineString([a, b])


    def thresh_by_field_size(self, pos):
        """Threshold the given position by the field size"""
        if (pos.x > constants.field_width / 2):
            pos.x = constants.field_width / 2 - constants.robot_width / 2
        elif (pos.x < -1 * constants.field_width / 2):
            pos.x = -1 * constants.field_width / 2 + constants.robot_width / 2

        if(pos.y > constants.field_height / 2):
            pos.y = constants.field_height / 2 - constants.robot_width / 2
        elif(pos.y < -1 * constants.field_height / 2):
            pos.y = -1 * constants.field_height / 2 + constants.robot_width / 2
        return pos

    def rotate(self):
        """Rotate the robot"""
        theta = self.pos.theta
        if (theta + 10) < 360:
            theta += 40
        else:
            theta = 0
        self.set_des_pos(self.pos.x, self.pos.y, theta)




