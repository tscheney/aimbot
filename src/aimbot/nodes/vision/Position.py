

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
        self.theta = self.theta - 180