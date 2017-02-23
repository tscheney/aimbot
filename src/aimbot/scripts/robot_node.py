#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist, Pose2D
import numpy as np
from Robot import Robot
import sys

def main():
    rospy.init_node('robot', anonymous=False)
    robot.subscribe()

    rate = rospy.Rate(robot.controller.control_rate)  # 100 Hz
    while not rospy.is_shutdown():
        robot.update()
        robot.publish()
        rate.sleep()

if __name__ == '__main__':
    # If this file was run from the command line, then do the following:
    print()
    robot = Robot(sys.argv[1]) # sys.argv[1] is the num arg from the robot.launch file
    main()