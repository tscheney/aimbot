#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState

from Position import Position



# which team/robot am I running on?
_team_side = 'home'
_ally_number = 1

# create a blank GameState message
# to keep track of current state
_game_state = GameState

# initialize vision positions
_me = Pose2D()
_ally = Pose2D()
_opp1 = Pose2D()
_opp2 = Pose2D()
_ball = Pose2D()


def _handle_me(msg):
    global _me
    #msg.x = -msg.x
    #msg.y = -msg.y
    #msg.theta = msg.theta - 1.57
    _me = msg


def _handle_ally(msg):
    global _ally
    #msg.x = -msg.x
    #msg.y = -msg.y
    #msg.theta = msg.theta - 1.57
    _ally = msg


def _handle_opp1(msg):
    global _opp1
    #msg.x = -msg.x
    #msg.y = -msg.y
    #msg.theta = msg.theta - 1.57
    _opp1 = msg


def _handle_opp2(msg):
    global _opp2
    #msg.x = -msg.x
    #msg.y = -msg.y
    #msg.theta = msg.theta - 1.57
    _opp2 = msg


def _handle_ball(msg):
    global _ball
    #msg.x = -msg.x
    #msg.y = -msg.y
    #msg.theta = msg.theta - 1.57
    _ball = msg


mePos = Position()
allyPos = Position()
opp1Pos = Position()
opp2Pos = Position()
ballPos = Position()



# Subscribe to Robot and Ball positions
    #rospy.Subscriber('me',   Pose2D, mePos.importMsg )
    #rospy.Subscriber('ally', Pose2D, allyPos.importMsg)
    #rospy.Subscriber('opp1', Pose2D, opp1Pos.importMsg)
    #rospy.Subscriber('opp2', Pose2D, opp2Pos.importMsg)
    #rospy.Subscriber('ball', Pose2D, ballPos.importMsg)

rospy.init_node('myvision', anonymous=True)

rospy.Subscriber('me',   Pose2D, _handle_me)
rospy.Subscriber('ally', Pose2D, _handle_ally)
rospy.Subscriber('opp1', Pose2D, _handle_opp1)
rospy.Subscriber('opp2', Pose2D, _handle_opp2)
rospy.Subscriber('ball', Pose2D, _handle_ball)





pubMe = rospy.Publisher('mepub', Pose2D, queue_size=10)
pubAlly = rospy.Publisher('allypub', Pose2D, queue_size=10)
pubOpp1 = rospy.Publisher('opp1pub', Pose2D, queue_size=10)
pubOpp2 = rospy.Publisher('opp2pub', Pose2D, queue_size=10)
pubBall = rospy.Publisher('ballpub', Pose2D, queue_size=10)


pubMe.publish(_me)
pubAlly.publish(_ally)
pubOpp1.publish(_opp1)
pubOpp2.publish(_opp2)
pubBall.publish(_ball)

