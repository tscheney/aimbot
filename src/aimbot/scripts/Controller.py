import numpy as np
from Observer import Observer
from MotorController import MotorController
from Position import Position

xy_scale = 2
xy_max_step = 0.22
xP = {
    'A': np.matrix([[0, 1], [0, -0.0006]]),  # A matrix for observer
    'B': np.matrix([[0], [0.5]]),  # B matrix for observer
    'C': np.matrix([[1, 0]]),  # C matrix for observer
    'K': np.matrix([[124.866 * xy_scale, 48.8645]]),  # Observer coefficient
    'ki': 6,  # Observer coefficient
    'kr': 124.866 *xy_scale,  # Observer coefficient
    'L': np.matrix([[244], [6.243e3]]),  # Correction coefficient used by xdhat
    'Ld': 12000,  # Correction coefficient used by dhat
    # 'K': np.matrix([[32.1629, 21.7255]]),
    # 'ki_ctrl': 3,
    # 'kr': 32.1629,
    # 'L': np.matrix([[137.9],[2030]]),
    # 'Ld': 3000,
    'F_max': 60,  # Maximum force applied to the system
    'max_step': xy_max_step,  # Max step size
    'x_e': np.matrix([[0], [0]]),  # Equilibrium States
    'type': 'x'
}

yP = {
    'A': np.matrix([[0, 1], [0, -0.0006]]),  # A matrix for observer
    'B': np.matrix([[0], [0.5]]),  # B matrix for observer
    'C': np.matrix([[1, 0]]),  # C matrix for observer
    'K': np.matrix([[124.866 * xy_scale, 48.8645]]),  # Observer coefficient
    'ki': 6,  # Observer coefficient
    'kr': 124.866 * xy_scale,  # Observer coefficient
    'L': np.matrix([[244], [6.243e3]]),  # Correction coefficient used by xdhat
    'Ld': 12000,  # Correction coefficient used by dhat
    # 'K': np.matrix([[32.1629, 21.7255]]),
    # 'ki_ctrl': 3,
    # 'kr': 32.1629,
    # 'L': np.matrix([[137.9],[2030]]),
    # 'Ld': 3000,
    'F_max': 60,  # Maximum force applied to the system
    'max_step': xy_max_step,  # Max step size
    'x_e': np.matrix([[0], [0]]),  # Equilibrium States
    'type': 'y'
}

th_scale = 2 #good for simulator
#th_scale = 0.01
thetaP = {
    'A': np.matrix([[0, 1], [0, -0.0006]]),  # A matrix for observer
    #'B': np.matrix([[0], [50]]),  # B matrix for observer
    'B': np.matrix([[0], [50]]),  # B matrix for observer
    'C': np.matrix([[1, 0]]),  # C matrix for observer
    # 'K': np.matrix([[38.4718, 2.7479]]),        # Observer coefficient
    # 'ki': 1.9099,                               # Observer coefficient
    # 'kr': 38.4718,                              # Observer coefficient
    # 'L': np.matrix([[137],[1.9236e4]]),         # Correction coefficient used by xhat
    # 'Ld': 3.8197e3,                             # Correction coefficient used by dhat
    #'K': np.matrix([[19.2928, 1.9436]]),
    'K': np.matrix([[19.2928 * th_scale, 1.9436]]),
    #'ki': 3.81,
    'ki': 1,
    #'kr': 19.2928,
    'kr': 19.2928 * th_scale,
    'L': np.matrix([[0.0972e4], [9.6463e4]]),
    #'L': np.matrix([[0], [0]]),
    'Ld': 1.9099e3,
    #'Ld': 0,
    'F_max': 30,  # Maximum force applied to the system
    #'F_max': 3,  # Maximum force applied to the system
    'max_step': np.pi / 16,  # Max step size
    'x_e': np.matrix([[0], [0]]),  # Equilibrium States
    'type': 'theta'
}


class Controller:
    """Controller class for the robots that uses observer based techniques"""
    def __init__(self):
        self.control_rate = 30
        self.x_obsv = Observer(xP)
        self.y_obsv = Observer(yP)
        self.th_obsv = Observer(thetaP)
        self.position = Position()
        self.vel = [0.0, 0.0, 0.0]  # (vx, vy, w)
        self.motor_ctrl = MotorController(3, 0.8, 116.2 * 4) #(1.0, 0.2, 116.2 * 4)
        self.wheel_vel = [0.0, 0.0, 0.0]  # (wheel1, wheel2, wheel3)
        self.tick = 0

    def update(self):
        """Updates the observer controller"""
        #if(self.tick == 5):

        self.x_obsv.updateObserver()
        self.y_obsv.updateObserver()
        self.adjust_for_period()  # The rotational movement is periodic with 2pi. It must account for that.
        self.th_obsv.updateObserver()
        self.position.update(self.x_obsv.xhat[0,0], self.y_obsv.xhat[0,0], self.th_obsv.xhat[0,0])
        self.vel = [self.x_obsv.xhat[1,0], self.y_obsv.xhat[1,0], self.th_obsv.xhat[1,0]]
        self.vel_to_wheel_vel()
        self.motor_ctrl.setSpeedRadSec(self.wheel_vel[0], self.wheel_vel[1], self.wheel_vel[2])

    def map_theta(self, theta_c):
        while (theta_c > 2 * np.pi):
            theta_c -= 2 * np.pi
        while (theta_c < 0):
            theta_c += 2 * np.pi
        return theta_c

    def update_cur_pos(self, x_c, y_c, theta_c):
        """Updates the current position from vision data"""
        self.x_obsv.xhat[0, 0] = x_c
        self.y_obsv.xhat[0, 0] = y_c
        self.th_obsv.xhat[0, 0] = theta_c

    def update_des_pos(self, x_d, y_d, theta_d):
        """Update the desired position of the robot"""
        self.x_obsv.d = x_d
        self.y_obsv.d = y_d
        self.th_obsv.d = theta_d

    def get_error(self, x, y, theta):
        """Find the error between the desired and real positions"""
        x_error = abs(abs(x.xhat[0, 0] - x.d))
        y_error = abs(abs(y.xhat[0, 0] - y.d))
        theta_error = abs(abs(theta.xhat[0, 0] - theta.d))
        return x_error, y_error, theta_error

    def adjust_for_period(self):
        """This function accounts for the periodicity of theta. The robot should never need to rotate more than 180 degrees."""
        curr = self.th_obsv.xhat[0, 0] # get theta position
        curr_d = self.th_obsv.d
        i = 0
        while (curr - curr_d) < -1 * np.pi:
            curr_d = curr_d - 2 * np.pi
            i = i + 1
            #if i > 4:
                #print("first", i)
        i = 0
        while (curr - curr_d) > np.pi:
            curr_d = curr_d + 2 * np.pi
            i = i + 1
            #if i > 4:
                #print("second", i)
                #       while (abs(curr - self.d) > np.pi):   # Sees if the distance between the two is greater than 180 degrees
                #           if curr > self.d:              # If the current position is greater than the desired position
                #               curr = curr - 2*np.pi      # Subtract 360 degrees from the current position
                #           else:                          # Else the current position is less than the desired position
                #               curr = curr + 2*np.pi      # Add 360 degrees to the current posistion

        if (curr - curr_d >= (-np.pi)) and (curr - curr_d <= np.pi):
            #self.th_obsv.xhat[0, 0] = curr  # Update current position
            self.th_obsv.d = curr_d
        else:
            print("error")
            print("th_vel", np.rad2deg(self.th_obsv.xhat[0, 0]))
            print("curr", np.rad2deg(curr))
            print("theta d", np.rad2deg(curr_d))
            # print("theta curr", curr*180/np.pi)

    def rotationM(self):
        """Create the rotation matrix to convert to wheel velocities"""
        R = np.matrix([[np.cos(self.position.theta), np.sin(self.position.theta), 0],
                       [-np.sin(self.position.theta), np.cos(self.position.theta), 0],
                       [0, 0, 1]])
        return R

    def vel_to_wheel_vel(self):
        """Convert the robot velocities to wheel velocities"""

        # linear and angular velocity of the center of the body
        v = np.matrix([[self.vel[0]], [self.vel[1]], [self.vel[2]]])

        # r tuples represent wheel positions
        # 0,0 being the center of the robot
        # if theta points in the +x direction
        # y increases as you go to the left of the robot
        # x increases as you go to the front of the robot


        # Robot body 1, configuration
        #r1 = (0.0, -0.0762, 0.0)
        #r2 = (-0.1016, 0.0, 0.0)
        #r3 = (0, 0.0762, 0.0)

        # Robot body 2 configuration (60 degrees)
        r1 = (0.04286, 0.07423, 0.0)
        r2 = (-0.08573, 0.0, 0.0)
        r3 = (0.04286, -0.07423, 0.0)

        # s vectors are unit vectors of wheel rotation
        # s1 is in the negative x direction (backward) right wheel
        # s2 is in the negative y direction (right) rear wheel
        # s3 is in the postive x direction (forward) left wheel

        # Robot body 1 configuration
        #s1 = (-1.0, 0.0, 0.0)
        #s2 = (0.0, -1.0, 0.0)
        #s3 = (1.0, 0.0, 0.0)

        s1 = (0.866025, -0.5, 0.0)
        #s2 = (0.0, -1.0, 0.0) # currently for aimbot2
        s2 = (0.0, 1.0, 0.0) # currently for aimbot1
        s3 = (-0.866025, -0.5, 0.0)

        rad = 0.03  # radius of the wheels

        m = (1 / rad) * np.matrix([[s1[0], s1[1], (s1[1] * r1[0] - s1[0] * r1[1])],
                                 [s2[0], s2[1], (s2[1] * r2[0] - s2[0] * r2[1])],
                                 [s3[0], s3[1], (s3[1] * r3[0] - s3[0] * r3[1])]])

        r = self.rotationM()
        Omega = np.dot(np.dot(m, r), v)
        #Omega *= 5

        self.wheel_vel = [Omega.item(0), Omega.item(1), Omega.item(2)]