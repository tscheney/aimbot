import rospy
import numpy as np
import constants
from Moving import Moving
from Controller import Controller
from Position import Position
from geometry_msgs.msg import Twist, Pose2D
from std_msgs.msg import Int16
from std_msgs.msg import Float32
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
        self.role = 0
        self.count = 0 #TODO remove
        self.team_side = team_side
        self.vel = [0.0, 0.0, 0.0]  # (vx, vy, w)
        self.wheel_vel = [0.0,0.0,0.0] # (wheel1, wheel2, wheel3)
        self.publishers = dict()
        self.init_publsihers()
        self.hertz_20 = 1
        self.first = True
        self.state = 0
        self.pause = 10
        self.path_planner = PathPlanner()
        self.control_ball = False
        self.change_roles = False

    def my_pos_sub(self):
        """Subscribe to my position"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"
        rospy.Subscriber(namespace + 'ally' + str(self.num), Pose2D, lambda msg: self.pos.import_msg(msg))

    def vision_sub(self):
        """Subscribe to other vision nodes"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"
        rospy.Subscriber(namespace + 'ball', Pose2D, lambda msg: self.ball_pos.import_msg(msg))

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

    def update(self):

        #need to wait until we have a des pos and a cur pos from publishers before running controller
        if(self.pos.init):
            """Updates the robots controller and sets velocities"""
            self.determine_des_pos()

            self.controller.update_des_pos(self.des_pos.x, self.des_pos.y, np.deg2rad(self.des_pos.theta))
            # TODO figure out how smooth vision values
            #if (self.hertz_20 == 5 or self.first):
            #print("degrees", self.pos.theta)
            #print("radians", np.deg2rad(self.pos.theta))
            self.controller.update_cur_pos(self.pos.x, self.pos.y, np.deg2rad(self.pos.theta))
            #    self.hertz_20 = 0
            #    self.first = False
            #self.hertz_20 = self.hertz_20 + 1


            #print(self.vel)
            self.controller.update()
            self.vel = [self.controller.vel[0], self.controller.vel[1], np.rad2deg(self.controller.vel[2])]
            self.wheel_vel = self.controller.wheel_vel

    def determine_des_pos(self):
        """Determine the desired position for the robot"""

        if self.role == 0: # stay where you are
            self.stay_put()

        elif self.role == 1:
            self.score_a_goal()

        elif self.role == 2:
            self.follow_ball_on_line(-3 * constants.field_width / 4)
        elif self.role == 3:
            self.go_behind_ball_facing_target(0.5)
        elif self.role == 4:
            if(self.ball_pos.x >= 0):
                self.follow_ball_on_line(0)
            else:
                self.follow_ball_on_line(-1 * constants.field_width / 4)
        elif 100 <= self.role < 1000:
            self.set_placement_role()
        elif self.role >= 1000:
            self.debug_role()
        else:
            print("not a valid role")

    def set_placement_role(self):
        """Determine desired position for set placement"""
        if self.role == 103: #reset field is true
            if (self.within_error(10)):
                return #dont do anything
            self.go_to(-.5,0, 0)
        elif self.role == 104: #reset field is true
            if (self.within_error(10)):
                return
            self.go_to(-1.2, 0, 0)

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
                if self.state < 1:
                    self.state += 1
                else:
                    self.state = 0
                print(self.state)
            if (self.state == 0):
                self.go_to(0, 0, 90)
            else:
                self.go_to(0, 0, -90)
        elif self.role == 1098: # debug role
            if(self.within_error(1)):
                if self.state < 1:
                    self.state += 1
                else:
                    self.state = 0
                print(self.state)
            if(self.state == 0):
                self.go_to(0,0,0)
            else:
                self.go_to(1,0,0)
        elif self.role == 1099: # debug role
            if self.within_error(10):
                if self.state < 3:
                    self.state += 1
                else:
                    self.state = 0
                print(self.state)

            self.move_square()
        else:
            print("not a valid role")

    def set_des_pos(self, des_x, des_y, des_th):
        """Sets the desired position"""
        self.des_pos.x = des_x
        self.des_pos.y = des_y
        self.des_pos.theta = des_th

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
        self.go_behind_ball_facing_target(0)
        goal_pos = Position()
        goal_pos.x = constants.field_width/2
        goal_pos.y = 0
        tol = 0.1
        in_tol = self.dis_from_point_to_line(self.pos, self.ball_pos, goal_pos) < tol
        if (self.theta_within_error(3) and in_tol):
            self.attack_ball()

    def go_behind_ball_facing_target(self, des_distance_from_ball):
        """Get behind the ball facing the goal"""
        theta = self.get_angle_between_points(self.ball_pos.x, self.ball_pos.y, constants.field_width/2, 0)
        hypotenuse = (constants.robot_width / 2)+ des_distance_from_ball
        x_c = self.ball_pos.x - hypotenuse * np.cos(theta)
        y_c = self.ball_pos.y - hypotenuse * np.sin(theta)
        theta = np.rad2deg(theta)

        self.set_des_pos(x_c, y_c, theta)

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

        #calculate theta based on the balls position relative to the goal
        # idealy it will face the front of the bot on a straight line path to the
        # goal. then rush the goal and score.
        # it seems to be a bit buggy when the ball is really close to the goal
        # but to some y above or below it.
        # if (ball.x >= 0):
        #     adj_length = 1.595 - ball.x
        #     opp_length = ball.y
        # else:
        #     adj_length = 1.595 + ball.x
        #     opp_length = ball.y
        # if (ball.y >= 0):
        #     theta = np.tan(opp_length / adj_length)
        #     if theta > 0:
        #         theta = -np.rad2deg(theta)
        #     else:
        #         theta = np.rad2deg(theta)
        # else:
        #     theta = np.tan(opp_length/adj_length)
        #     if theta > 0:
        #         theta = np.rad2deg(theta)
        #     else:
        #         theta = -np.rad2deg(theta)
        #
        # while (theta > 90):
        #     theta = theta - 90
        # while (theta < -90):
        #     theta = theta + 90
        # print("theta is", theta)
        #
        # if ball.x < me.x:
        #     print("get behind ball")
        #     if ball.y < 0:
        #         if ball.x -.5 > -1.595:
        #             x = ball.x-.5
        #             y = ball.y+.3
        #             cmdvec = np.array([[x], [y]])
        #             self.set_des_pos(cmdvec.flatten()[0], cmdvec.flatten()[1], theta)
        #             #return
        #         else:
        #             x = ball.x + .05
        #             y = ball.y+.3
        #             cmdvec = np.array([[x], [y]])
        #             self.set_des_pos(cmdvec.flatten()[0], cmdvec.flatten()[1], theta)
        #             #return
        #     else:
        #         if ball.x -.5 > -1.595:
        #             x = ball.x -.5
        #             y = ball.y -.3
        #             cmdvec = np.array([[x], [y]])
        #             self.set_des_pos(cmdvec.flatten()[0], cmdvec.flatten()[1], theta)
        #             #return
        #         else:
        #             x = ball.x + .05
        #             y = ball.y -.3
        #             cmdvec = np.array([[x], [y]])
        #             self.set_des_pos(cmdvec.flatten()[0], cmdvec.flatten()[1], theta)
        #             #return
        # # If I am sufficiently close to the point behind the ball,
        # # or in other words, once I am 21cm behind the ball, just
        # # drive to the goal.
        #elif np.linalg.norm(p - mevec) < 0.15:
        #    cmdvec = goalvec
        #     print("rush goal")
        # else:
        #     print("get in line")
        #    cmdvec = p
        #    cmdvec = p
        #self.set_des_pos(cmdvec.flatten()[0], cmdvec.flatten()[1], theta


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

    def rotate(self):
        """Rotate the robot"""
        theta = self.pos.theta
        if (theta + 10) < 360:
            theta += 40
        else:
            theta = 0
        self.set_des_pos(self.pos.x, self.pos.y, theta)




