
from Moving import Moving

class Game:
    def __init__(self):
        self.ball = Moving()
        self.oppTeam = [Moving(), Moving()]


    def updateBall(self, msg):
        self.ball.pos.x = msg.x
        self.ball.pos.y = msg.y

