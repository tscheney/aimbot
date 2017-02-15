#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState

#from aimbot.nodes.game.Position import Position
from Position import Position

#from Position import Position

_gamestate = GameState()


def _handle_gamestate(msg):
    global _gamestate 
    _gamestate = msg


def main():
    ally1Pos = Position()
    ally2Pos = Position()
    opp1Pos = Position()
    opp2Pos = Position()
    ballPos = Position()

    # are we home or away?
    param_name = rospy.search_param('team_side')
    team_side = rospy.get_param(param_name, 'home')

    # init our vision node
    #rospy.init_node('myvision', anonymous=False)
    rospy.init_node('vision_pub', anonymous=False)

    # subscribe to global vision
    rospy.Subscriber('ally1', Pose2D, lambda msg: ally1Pos.msgHandler(msg, team_side, _gamestate))
    rospy.Subscriber('ally2', Pose2D, lambda msg: ally2Pos.msgHandler(msg, team_side, _gamestate))
    rospy.Subscriber('opp1', Pose2D, lambda msg: opp1Pos.msgHandler(msg, team_side, _gamestate))
    rospy.Subscriber('opp2', Pose2D, lambda msg: opp2Pos.msgHandler(msg, team_side, _gamestate))
    rospy.Subscriber('ball', Pose2D, lambda msg: ballPos.msgHandler(msg, team_side, _gamestate))

    rospy.Subscriber('/game_state', GameState, _handle_gamestate)

    pubAlly1 = rospy.Publisher('ally1pub', Pose2D, queue_size=10)
    pubAlly2 = rospy.Publisher('ally2pub', Pose2D, queue_size=10)
    pubOpp1 = rospy.Publisher('opp1pub', Pose2D, queue_size=10)
    pubOpp2 = rospy.Publisher('opp2pub', Pose2D, queue_size=10)
    pubBall = rospy.Publisher('ballpub', Pose2D, queue_size=10)



    rate = rospy.Rate(100) # 100 Hz
    while not rospy.is_shutdown():

        print('hello world')
        pubAlly1.publish(ally1Pos.export())
        pubAlly2.publish(ally2Pos.export())
        pubOpp1.publish(opp1Pos.export())
        pubOpp2.publish(opp2Pos.export())
        pubBall.publish(ballPos.export())

        rate.sleep()


if __name__ == '__main__':
    # If this file was run from the command line, then do the following:
    main()

