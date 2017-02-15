#!/usr/bin/env python

from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState

class Position:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.theta = 0

    def msgHandler(self, msg,team_side, game_state):
        self.importMsg(msg)
        self.handleFlip(team_side, game_state)
        #print('x:',self.x)
        #print('y:', self.y)
        #print('th:', self.theta)

    def importMsg(self, msg):
        self.x = msg.x
        self.y = msg.y
        self.theta = msg.theta

    def invert(self):
        self.x = -1*self.x
        self.y = -1*self.y
        if self.theta < 180:
            self.theta += 180
        else:
            self.theta -= 180

    def handleFlip(self, team_side, game_state):
        # Invert if away or second half
       if (team_side != 'home') ^ bool(game_state.second_half):
            self.invert()

    def export(self, ):
        msg = Pose2D()

        # Export msg
        msg.x = self.x
        msg.y = self.y
        msg.theta = self.theta
        return msg
