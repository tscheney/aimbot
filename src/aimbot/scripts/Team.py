import rospy
from Robot import Robot
from GameStateObj import GameStateObj
from Position import Position
from soccerref.msg import GameState
from geometry_msgs.msg import Pose2D

class Team:

    """Class that determines team strategy based on game state and publishes
    that information to the robots
    all topics published by the game class are published under the node
    /aimbot_(team_side)/team/"""

    def __init__(self):
        self.robots = [Robot(), Robot()]
        self.game_state = GameStateObj()
        self.team_side = rospy.get_param(rospy.search_param('team_side'), 'home')
        self.positions = dict(ally1=Position(), ally2=Position, opp1=Position(), opp2=Position(), ball=Position())

    def game_state_sub(self):
        """Sets up the subscription to the game state"""
        namespace = "/aimbot_" + self.team_side + "/game/"
        rospy.Subscriber(namespace + 'game_state', GameState, lambda msg: self.game_state.import_msg(msg))

    def vision_sub(self):
        """Sets up the subscription to the aimbot vision"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"

        # Uses the dictionary of position objects to subscribe to all the vision topics
        for name, pos in self.positions.items():
            rospy.Subscriber(namespace + pos, Pose2D, lambda msg: pos.import_msg_raw(msg, self.team_side, self.game_state))

    # Need to set up a way to publish roles or strategies to robots


    def subscribe(self):
        """Subscribe to all nodes necessart for the team node"""
        self.game_state_sub()

    def init_publsihers(self):
        """Inits all publishers in the game node"""

    def publish(self):
        """Publishes all publishers in the team node"""




