
from soccerref.msg import GameState

class GameStateObj():
    """Handles game state messages"""

    def __init__(self):
        self.home_score = 0
        self.away_score = 0
        self.home_bot_count = 0
        self.away_bot_count = 0
        self.remaining_seconds = 0
        self.play = False
        self.reset_field = False
        self.second_half = False

    def import_msg(self, msg):
        """Imports data from GameState msg into self"""

        self.home_score = msg.home_score
        self.away_score = msg.away_score
        self.home_bot_count = msg.home_bot_count
        self.away_bot_count = msg.away_bot_count
        self.remaining_seconds = msg.remaining_seconds
        self.play = msg.play
        self.reset_field = msg.reset_field
        self.second_half = msg.second_half

    def export(self):
        """Exports the data to a GameState msg"""

        msg = GameState()

        msg.home_score = self.home_score
        msg.away_score = self.away_score
        msg.home_bot_count = self.home_bot_count
        msg.away_bot_count = self.away_bot_count
        msg.remaining_seconds = self.remaining_seconds
        msg.play = self.play
        msg.reset_field = self.reset_field
        msg.second_half = self.second_half

        return msg