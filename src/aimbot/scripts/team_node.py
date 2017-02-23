#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist, Pose2D
import numpy as np
from Team import Team

def main():
    rospy.init_node('team', anonymous=False)
    team = Team()
    team.subscribe()

    rate = rospy.Rate(100)  # 100 Hz
    while not rospy.is_shutdown():
        team.update()
        team.publish()
        rate.sleep()

if __name__ == '__main__':
    # If this file was run from the command line, then do the following:
    main()