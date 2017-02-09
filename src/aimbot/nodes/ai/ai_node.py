#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState
from Position import Position

import numpy as np

from AI import AI

#### Global vars ####

# which team/robot am I running on?
_team_side = 'home'
_ally_number = 1

# create a blank GameState message
# to keep track of current state
_game_state = GameState()

# initialize vision positions
_me = Pose2D()
_ally = Pose2D()
_opp1 = Pose2D()
_opp2 = Pose2D()
_ball = Pose2D()

# -------------------

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


def _handle_game_state(msg):
    global _game_state
    _game_state = msg


me = Position()
ally = Position()
opp1 = Position()
opp2 = Position()
ball = Position()


def main():
    rospy.init_node('ai', anonymous=False)

    # are we home or away?
    global _team_side
    param_name = rospy.search_param('team_side')
    _team_side = rospy.get_param(param_name, 'home')

    # which ally are we?
    global _ally_number
    # An exteremely brittle way of getting the robot number
    # Try setting as a rosparam instead?
    _ally_number = int(rospy.get_namespace().split('/')[-2][-1])

    # Subscribe to Robot and Ball positions
    rospy.Subscriber('me',   Pose2D, lambda msg: me.importMsg(msg))
    rospy.Subscriber('ally', Pose2D, lambda msg: ally.importMsg(msg))
    rospy.Subscriber('opp1', Pose2D, lambda msg: opp1.importMsg(msg))
    rospy.Subscriber('opp2', Pose2D, lambda msg: opp2.importMsg(msg))
    rospy.Subscriber('ball', Pose2D, lambda msg: ball.importMsg(msg))

    #rospy.Subscriber('ally1_oriented', Pose2D, _handle_me)
    #rospy.Subscriber('ally2_oriented', Pose2D, _handle_ally)
    #rospy.Subscriber('opp1_oriented', Pose2D, _handle_opp1)
    #rospy.Subscriber('opp2_oriented', Pose2D, _handle_opp2)
    #rospy.Subscriber('ball_oriented', Pose2D, _handle_ball)



    # This message comes from the soccerref and
    # tells us if we should be playing or not
    rospy.Subscriber('/game_state', GameState, _handle_game_state)

    # This is our publisher that tells the controller where we want to be
    pub = rospy.Publisher('desired_position', Pose2D, queue_size=10)

    # Create the AI object
    ai = AI(_team_side, _ally_number)

    rate = rospy.Rate(100) # 100 Hz
    while not rospy.is_shutdown():

        # Based on the state of the game and the positions of the players,
        # run the AI and return commanded positions for this robot
        cmds = ai.strategize(me.export(), ally.export(), opp1.export(), opp2.export(), ball.export(), _game_state)

        # Get a message ready to send
        msg = Pose2D()           

        if _game_state.reset_field:
            # Send robot to home
            if _ally_number == 1:
                msg.x = -0.5
                msg.y = 0
                msg.theta = 0

            elif _ally_number == 2:
                msg.x = -1
                msg.y = 0
                msg.theta = 0

        else:
            # Run AI as normal
            #if (AI.team_side == "away")

            msg.x = cmds[0]
            msg.y = cmds[1]
            msg.theta = cmds[2]


        # If we shouldn't play and the field doesn't need to be
        # reset, then the AI node is out of a job.
        if _game_state.play or _game_state.reset_field:
            #print('hellow world')
            pub.publish(msg)

        # Wait however long it takes to make this tick at 100Hz
        rate.sleep()



if __name__ == '__main__':
    # If this file was run from the command line, then do the following:
    main()