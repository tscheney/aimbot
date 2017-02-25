#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist, Pose2D
import numpy as np
from Game import Game
import sys

def main():
    rospy.init_node('game', anonymous=False)
    game.subscribe()

    rate = rospy.Rate(100)  # 100 Hz
    while not rospy.is_shutdown():
        game.publish()
        rate.sleep()

if __name__ == '__main__':
    # If this file was run from the command line, then do the following:
    game = Game(sys.argv[1], sys.argv[2])  # sys.argv[1] is the team_side arg from the game.launch file, sys.argv[2] is sim_mode
    main()