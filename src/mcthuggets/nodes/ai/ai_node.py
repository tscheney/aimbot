#!/usr/bin/env python
import sys

import rospy
from geometry_msgs.msg import Pose2D
from std_msgs.msg import Bool

from mcthuggets.msg import BallState, RobotState

import numpy as np

import Strategy, Constants
from GameObjects import Ball, Robot

_me = None
_ally = None
_opp1 = None
_opp2 = None

_ball = None

_team_side = None

# We have a dependency on the soccerref package. This
# GameState message comes from the referee.
from soccerref.msg import GameState
_game_state = GameState()

def _handle_robot_state(msg, which_robot):
    # Update the given robot's current and future positions
    if which_robot == 'me' and _me is not None:
        _me.update_state(msg)
    elif which_robot == 'ally' and _ally is not None:
        _ally.update_state(msg)
    elif which_robot == 'opp1' and _opp1 is not None:
        _opp1.update_state(msg)
    elif which_robot == 'opp2' and _opp2 is not None:
        _opp2.update_state(msg)

def _handle_ball_state(msg):
    _ball.update_state(msg)

def _handle_game_state(msg):
    global _game_state

    _game_state = msg

def _create_robots():
    """Create Robots
    This function uses the namespace of the node to know whether
    this code is running on ally1 (Nugget) or ally2 (Fry).
    Since `am_i_ally1` and `am_i_ally2` are mutually exclusive,
    after creating the `_me` robot object, `_ally` has the
    opposite designation than `_me`.

    For example, if `am_i_ally1` is True, then `am_i_ally2` must
    be False. Thus, `_me` will be ally1 and `_ally` will be ally2.
    """
    global  _me, _ally, _opp1, _opp2
    am_i_ally1 = 'ally1' in rospy.get_namespace()
    am_i_ally2 = 'ally2' in rospy.get_namespace()

    _me = Robot(ally1=am_i_ally1, ally2=am_i_ally2)
    _ally = Robot(ally1=(not am_i_ally1), ally2=(not am_i_ally2))
    _opp1 = Robot()
    _opp2 = Robot()

def main():
    rospy.init_node('ai', anonymous=False)

    # are we home or away?
    global _team_side
    param_name = rospy.search_param('team_side')
    _team_side = rospy.get_param(param_name, 'home')

    # Create robot objects that store that current robot's state
    _create_robots()
 
    global _ball
    _ball = Ball()

    # Subscribe to Robot States
    rospy.Subscriber('my_state', RobotState, lambda msg: _handle_robot_state(msg, 'me'))
    rospy.Subscriber('ally_state', RobotState, lambda msg: _handle_robot_state(msg, 'ally'))
    rospy.Subscriber('opponent1_state', RobotState, lambda msg: _handle_robot_state(msg, 'opp1'))
    rospy.Subscriber('opponent2_state', RobotState, lambda msg: _handle_robot_state(msg, 'opp2'))

    rospy.Subscriber('ball_state', BallState, _handle_ball_state)

    # This message will tell us if we are to be playing or not right now
    rospy.Subscriber('/game_state', GameState, _handle_game_state)

    pub = rospy.Publisher('desired_position', Pose2D, queue_size=10)

    rate = rospy.Rate(100) #100 Hz
    while not rospy.is_shutdown():

        (x_c, y_c, theta_c) = Strategy.choose_strategy(_me, _ally, _opp1, _opp2, \
                                     _ball, _game_state, _team_side)

        # Get a message ready to send
        msg = Pose2D()           

        if _game_state.reset_field:
            # Send robot to home
            if _me.ally1:
                msg.x = Constants.ally1_start_pos[0]
                msg.y = Constants.ally1_start_pos[1]
                msg.theta = Constants.ally1_start_pos[2]
            elif _me.ally2:
                msg.x = Constants.ally2_start_pos[0]
                msg.y = Constants.ally2_start_pos[1]
                msg.theta = Constants.ally2_start_pos[2]
        else:
            # Run AI as normal
            msg.x = x_c
            msg.y = y_c
            msg.theta = theta_c


        # If we shouldn't play and the field doesn't need to be
        # reset, then the AI node is out of a job.
        if _game_state.play or _game_state.reset_field:
            pub.publish(msg)

        rate.sleep()

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    main()
