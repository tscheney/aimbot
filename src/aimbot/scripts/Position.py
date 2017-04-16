#!/usr/bin/env python

from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState
import shapely.geometry as geo

#class Position(geo.Point):
class Position:
    def __init__(self):
        #geo.Point.__init__(self, 0, 0)
        #self._set_coords(0,0)
        self.x = 0
        self.y = 0
        self.theta = 0
        self.init = False

    def import_msg_raw(self, msg, team_side, game_state):
        self.import_msg(msg)
        self.handleFlip(team_side, game_state)

    def import_msg(self, msg):
        self.init = True
        #self._set_coords(msg.x, msg.y)
        self.x = msg.x
        self.y = msg.y
        self.theta = msg.theta

    def invert(self):
        #self._set_coords(-1*self.x, -1*self.y)
        self.x = -1*self.x
        self.y = -1*self.y
        if self.theta < 180:
            self.theta += 180
        else:
            self.theta -= 180

    def handleFlip(self, team_side, game_state):
        # Invert if away or second half
        if (team_side != 'home') != bool(game_state.second_half): #xor
            self.invert()

    def update(self, x, y, theta):
        """Updates position values"""
        #self._set_coords(x, y)
        self.x = x
        self.y = y
        self.theta = theta

    def export(self):
        msg = Pose2D()

        # Export msg
        msg.x = self.x
        msg.y = self.y
        msg.theta = self.theta
        return msg

    def to_Point(self):
        """Returns a shapely point at these coordinates"""
        return geo.Point(self.x, self.y)

    # def intersect(self, poly):
    #     """Returns true if the given polygon contains this point"""
    #     return self.within(poly)
