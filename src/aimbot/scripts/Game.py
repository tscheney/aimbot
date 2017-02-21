import rospy
from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState
from Position import Position
from GameStateObj import GameStateObj


from Moving import Moving

#
class Game:

    """Class that holds all information about the game state as seen by aimbot
    all topics published by the game class are published under the node
    /aimbot_(team_side)/game/"""

    def __init__(self):
        self.ball = Moving()
        self.oppTeam = [Moving(), Moving()]
        self.num_robots = 2
        self.game_state = GameStateObj()
        self.team_side = rospy.get_param(rospy.search_param('team_side'), 'home')
        self.sim_mode = self.sim_mode = rospy.get_param(rospy.search_param('sim_mode'), True)
        self.positions = dict(ally1=Position(), ally2=Position, opp1=Position(), opp2=Position(), ball=Position())
        self.publishers = dict()

    def game_to_team(self):
        """"Returns a dictionary which translates home and away to ally and opp"""
        trans = dict()

        for i in range(0, self.num_robots):
            if (self.team_side == 'home'):
                trans['ally' + str(i)] = 'home' + str(i)
                trans['opp' + str(i)] = 'away' + str(i)
            else:
                trans['ally' + str(i)] = 'away' + str(i)
                trans['opp' + str(i)] = 'home' + str(i)
        trans['ball'] = 'ball'
        return trans

    def game_state_sub(self):
        """Sets up the subscription to the game state"""

        rospy.Subscriber('/game_state', GameState, lambda msg: self.game_state.import_msg(msg))

    def init_game_state_pub(self):
        """Inits publisher for the game state"""
        namespace = "/aimbot_" + self.team_side + "/game/"
        self.publishers['game_state'] = rospy.Publisher(namespace + 'game_state', GameState, queue_size=10)

    def game_state_pub(self):
        """Publishes the game state"""

        self.publishers['game_state'].publish(self.game_state.export())


    def vision_sub(self):
        """Sets up the subscription to the raw vision"""
        if self.sim_mode: # if sim mode, look at the simulation vision
            namespace = "/vision/"
        else: # if not sim mode, look at our vision
            namespace = "/aimbot_" + self.team_side + "/raw_vision/"

        game_to_team = self.game_to_team()
        # Uses the dictionary of position objects to subscribe to all the vision topics
        for name, pos in self.positions.items():
            rospy.Subscriber(namespace + game_to_team[pos], Pose2D, lambda msg: pos.import_msg_raw(msg, self.team_side, self.game_state))

    def init_vision_pub(self):
        """Inits publishers for the correctly oriented positions"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"

        # Adds position publishers to the publishers dictionary
        for name, pos in self.positions.items():
            self.publishers[name] = pubAlly1 = rospy.Publisher(namespace + name, Pose2D, queue_size=10)

    def vision_pub(self):
        """Publishes the correctly oriented positions"""

        # publishes positions
        for name, pos in self.positions.items():
            self.publishers[name].publish(self.positions[name].export())

    def subscribe(self):
        """Subscribe to all nodes necessart for the game node"""
        self.vision_sub()
        self.game_state_sub()

    def init_publsihers(self):
        """Inits all publishers in the game node"""
        self.init_vision_pub()
        self.init_game_state_pub()

    def publish(self):
        """Publishes all publishers in the game node"""
        self.vision_pub()
        self.game_state_pub()
