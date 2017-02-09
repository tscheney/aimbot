#!/usr/bin/env python

import rospy

from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState

import numpy as np

_gamestate = GameState()

_team_side = None

_pub_ally1 = None
_pub_ally2 = None
_pub_opp1 = None
_pub_opp2 = None
_pub_ball = None


def _handle_gamestate(msg):
    global _gamestate 
    _gamestate = msg


def _handle_vision(msg, obj):

    if (_team_side != 'home') ^ bool(_gamestate.second_half):
        msg.x = -1*msg.x
        msg.y = -1*msg.y

        if msg.theta < 180:
            msg.theta += 180
        else:
            msg.theta -= 180


    if obj == 'ally1':
        _pub_ally1.publish(msg)
    elif obj == 'ally2':
        _pub_ally2.publish(msg) 
    elif obj == 'opp1':
        _pub_opp1.publish(msg)
    elif obj == 'opp2':
        _pub_opp2.publish(msg)
    elif obj == 'ball':
        _pub_ball.publish(msg)


def main():
    rospy.init_node('vision', anonymous=False)

    global _team_side
    param_name = rospy.search_param('team_side')
    _team_side = rospy.get_param(param_name, 'home')

    rospy.Subscriber('ally1', Pose2D, lambda msg: _handle_vision(msg, 'ally1'))
    rospy.Subscriber('ally2', Pose2D, lambda msg: _handle_vision(msg, 'ally2'))
    rospy.Subscriber('opp1', Pose2D, lambda msg: _handle_vision(msg, 'opp1'))
    rospy.Subscriber('opp2', Pose2D, lambda msg: _handle_vision(msg, 'opp2'))
    rospy.Subscriber('ball', Pose2D, lambda msg: _handle_vision(msg, 'ball'))

    rospy.Subscriber('/game_state', GameState, _handle_gamestate)

    # This is our publisher that tells the controller where we want to be
    global _pub_ally1, _pub_ally2, _pub_opp1, _pub_opp2, _pub_ball 
    _pub_ally1 = rospy.Publisher('ally1_oriented', Pose2D, queue_size=10)
    _pub_ally2 = rospy.Publisher('ally2_oriented', Pose2D, queue_size=10)
    _pub_opp1 = rospy.Publisher('opp1_oriented', Pose2D, queue_size=10)
    _pub_opp2 = rospy.Publisher('opp2_oriented', Pose2D, queue_size=10)
    _pub_ball = rospy.Publisher('ball_oriented', Pose2D, queue_size=10)

    rate = rospy.Rate(100) # 100 Hz
    while not rospy.is_shutdown():
        pass



if __name__ == '__main__':
    # If this file was run from the command line, then do the following:
    main()

