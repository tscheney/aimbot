import rospy
import numpy as np
from Moving import Moving
from ControllerClass import Controller
from Position import Position
from geometry_msgs.msg import Twist, Pose2D
from std_msgs.msg import Int16

class Robot(Moving):
    """Class that determines movements and control for a robot
    all topics published by the robot class are published under the node
    /aimbot_(team_side)/players/ally(num)
    this is with the exception of the command node which is published for
    compatability with the simulator"""

    def __init__(self, num=0):
        Moving.__init__(self)
        self.controller = Controller()
        self.num = num # player number
        self.position = Position()
        self.ball_pos = Position()
        self.des_position = Position() # place where we want to go
        self.role = 0
        self.team_side = rospy.get_param(rospy.search_param('team_side'), 'home')
        self.vel = (0,0,0) # (vx, vy, w)
        self.publishers = dict()
        self.init_publsihers()

    def my_pos_sub(self):
        """Subscribe to my position"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"
        rospy.Subscriber(namespace + 'ally' + str(self.num), Pose2D, lambda msg: self.position.import_msg(msg))

    def vision_sub(self):
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

    def vel_pub(self):
        """Publishes the commanded velocities.  Needed by simulator."""
        msg = Twist()
        msg.linear.x = self.vel[0]
        msg.linear.y = self.vel[1]
        msg.angular.z = self.vel[2]
        self.publishers['cmd_vel'].publish(msg)

    def subscribe(self):
        """Subscribe to all nodes necessary for this robot"""
        self.my_pos_sub()
        self.vision_sub()
        self.my_role_sub()

    def init_publsihers(self):
        """Inits all publishers for this robot"""
        self.init_vel_pub()

    def publish(self):
        """Publishes all publishers for this robot"""
        self.vel_pub()

    def update(self):
        """Updates the robots controller"""
        self.determine_des_pos()
        self.controller.set_commanded_position(self.des_position.x, self.des_position.y, self.des_position.theta)
        self.vel = self.controller.update(self.position.x, self.position.y, self.position.theta)

    def set_des_pos(self, des_x, des_y, des_th):
        """Sets the desired position"""
        self.des_position.x = des_x
        self.des_position.y = des_y
        self.des_position.theta = des_th

    def determine_des_pos(self):
        """Determine the desired position for the robot"""
        if self.role == 0:
            self.go_to(self.position.x, self.position.y)
        elif self.role == 1:
            self.rush_goal(self.position, self.ball_pos)
        elif self.role == 2:
            self.follow_ball_on_line(self.ball_pos, -1.25)

    def follow_ball_on_line(self, ball, x_c):
        y_c = ball.y
        theta_c = 0
        self.set_des_pos(x_c, y_c, theta_c)

    def rush_goal(self, me, ball):
        # Use numpy to create vectors
        ballvec = np.array([[ball.x], [ball.y]])
        mevec = np.array([[me.x], [me.y]])
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


    def go_to(self, x, y):
        self.set_des_pos(x, y, 0)

    def move_to_center(self):
        return self.go_to(0, 0)

    def move_square(self, me):
        global state

        print('state is: ',state)
        # actions
        if (state == 0):  # going to upper right
            state = 1
            return self.go_to(0.15, 0.15)
        elif (state == 1):  # going to lower right
            state = 2
            return self.go_to(0.15, -0.15)
        elif (state == 2):  # going to lower left
            state = 3
            return self.go_to(-0.15, -0.15)
        elif (state == 3):  # going to upper left
            state = 0
            return self.go_to(-0.15, 0.15)

    def tolerance(self, pos, desired, error):
        # if(pos >= 0):
        return pos >= (desired - error) and pos <= (desired + error)
        # else:
        # return pos <= (desired - error) and pos >= (desired + error)

    def rotate(self, me):
        theta = me.theta
        if (theta + 10) < 360:
            theta += 40
        else:
            theta = 0
        return (me.x, me.y, theta)
