from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState

class Position:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.theta = 0

    def importMsg(self, msg):
        self.x = msg.x
        self.y = msg.y
        self.theta = msg.theta

    def invert(self):
        self.x = -self.x
        self.y = -self.y
        if self.theta < 180:
            self.theta += 180
        else:
            self.theta -= 180

    def handleFlip(self, team_side, game_state):
        # Invert if away or second half
        if ((team_side == 'away') ^ (game_state.second_half == 'True')):
            self.invert()

    def export(self, ):
        msg = Pose2D()

        # Export msg
        msg.x = self.x
        msg.y = self.y
        msg.theta = self.theta
        return msg
