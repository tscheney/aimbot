import rospy
import numpy as np
from Moving import Moving
from Controller import Controller
from Position import Position
from geometry_msgs.msg import Twist, Pose2D
from std_msgs.msg import Int16
from std_msgs.msg import Float32

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
        self.my_pos_sub()
        self.vision_sub()
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
        """Updates the robots controller and sets velocities"""
        self.determine_des_pos()
        self.controller.update_des_pos(self.des_pos.x, self.des_pos.y, self.des_pos.theta)
        self.controller.update_cur_pos(self.pos.x, self.pos.y, self.pos.theta)
        #print(self.vel)
        self.controller.update()
        self.vel = self.controller.vel
        self.wheel_vel = self.controller.wheel_vel

    def determine_des_pos(self):
        """Determine the desired position for the robot"""
        if self.role == 0: # stay where you are
            self.go_to(self.pos.x, self.pos.y, 60)
            #self.move_to_center()
        elif self.role == 1:
            self.rush_goal(self.pos, self.ball_pos)
        elif self.role == 2:
            self.follow_ball_on_line(self.ball_pos, -1.25)

    def set_des_pos(self, des_x, des_y, des_th):
        """Sets the desired position"""
        self.des_pos.x = des_x
        self.des_pos.y = des_y
        self.des_pos.theta = des_th

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

    def go_to(self, x, y, theta):
        self.set_des_pos(x, y, theta)

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




