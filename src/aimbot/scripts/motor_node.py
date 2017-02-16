#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist, Pose2D
import numpy as np
from Position import Position

import Controller
#### Global vars ####

_ctrl_period = 1.0/100

# linear and angular velocity of the center of the body
v = np.matrix([[0],[0],[0]])


# wheel positions (these are guesses)
# 0,0 being the center of the robot
# y increases as you go to the front of the robot
# x increases as you go to the right of the robot

r1 = (-0.2, 0, 0)
r2 = (0.2, 0, 0)
r3 = (0, -0.1, 0)

# unit vectors of wheel rotation
#s1 is in the forward y direction (forward) left wheel
#s2 is in the forward y direction (forward) right wheel
#s3 is in the forward x direction (right) rear wheel

s1 = (0, 1, 0)
s2 = (0, 1, 0)
s3 = (1, 0, 0)

R = 0.04 # radius of the wheels (guess)


M = (1/R) * np.matrix([[ s1[0], s1[1], (s1[1]*r1[0] - s1[0]*r1[1]) ] ,
                          [ s2[0], s2[1], (s2[1]*r2[0] - s2[0]*r2[1]) ] ,
                          [ s3[0], s3[1], (s3[1]*r3[0] - s3[0]*r3[1]) ] ])


me = Position()


def rotationM(theta):
    rad = np.radians(theta)
    R = np.matrix([ [ np.cos(rad), np.sin(rad), 0 ],
                    [ -np.sin(rad), np.cos(rad), 0],
                    [0, 0, 1] ])
    return R
# -------------------

def _handle_vel(msg):
    global v
    vx = msg.linear.x
    vy = msg.linear.y
    vth= msg.angular.z
    v[0,0] = vx
    v[1,0] = vy
    v[2,0] = vth
    #print(v)


def _handle_desired_position(msg):
    Controller.set_commanded_position(msg.x, msg.y, msg.theta)


def main():
    rospy.init_node('motor', anonymous=False)

    # Subscribe to my current state (from the vision node)
    # and my desired state (from the ai node)
    rospy.Subscriber('vel_cmds', Twist, _handle_vel)
    rospy.Subscriber('me', Pose2D, lambda msg: me.importMsg(msg))

    rate = rospy.Rate(int(1/_ctrl_period))
    while not rospy.is_shutdown():
        R = rotationM(me.export().theta)
        Omega = np.dot(np.dot(M, R),v)

        print(Omega)
        # Wait however long it takes to make this tick at proper control period
        rate.sleep()



if __name__ == '__main__':
    # If this file was run from the command line, then do the following:
    main()