import rospy
from geometry_msgs.msg import Pose2D
from soccerref.msg import GameState
from Position import Position
from GameStateObj import GameStateObj
import time as time



from Moving import Moving

#
class Game:

    """Class that holds all information about the game state as seen by aimbot
    all topics published by the game class are published under the node
    /aimbot_(team_side)/game/"""

    def __init__(self, team_side, sim_mode):
        self.ball = Moving()
        self.oppTeam = [Moving(), Moving()]
        self.num_robots = 2
        self.num_opp_robots = 2
        self.game_state = GameStateObj()
        self.team_side = team_side
        self.sim_mode = rospy.get_param(rospy.search_param('sim_mode'), True)
        self.positions = dict() #ally1=Position(), ally2=Position, opp1=Position(), opp2=Position(), ball=Position()
        self.publishers = dict()
        self.n = 0
        self.init_pos()
        self.init_publsihers()


    def init_pos(self):
        """Init the position dictionary"""
        # Add ally positions to dictionary
        for i in range(1, self.num_robots + 1):
            name = 'ally' + str(i)
            self.positions[name] = Position()

        # Add opponent positions to dictionary
        for i in range(1, self.num_opp_robots + 1):
            name = 'opp' + str(i)
            self.positions[name] = Position()

        # Add ball position to dictionary
        name = 'ball'
        self.positions[name] = Position()


    def game_to_team(self):
        """"Returns a dictionary which translates home and away to ally and opp"""
        trans = dict()

        # translate for allies
        for i in range(1, self.num_robots + 1):
            if (self.team_side == 'home'):
                trans['ally' + str(i)] = 'home' + str(i)
            else:
                trans['ally' + str(i)] = 'away' + str(i)

        # translate for opponents
        for i in range(1, self.num_opp_robots + 1):
            if (self.team_side == 'home'):
                trans['opp' + str(i)] = 'away' + str(i)
            else:
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
        #self.game_state.second_half = True;
        self.publishers['game_state'].publish(self.game_state.export())

    def vision_sub(self):
        """Sets up the subscription to the raw vision"""
        if self.sim_mode: # if sim mode, look at the simulation vision
            print("is simmode")
            namespace = "/vision/"

            game_to_team = self.game_to_team()

            name = "ally1"
            rospy.Subscriber(namespace + game_to_team[name], Pose2D,
                             lambda msg: self.positions["ally1"].import_msg_raw(msg, self.team_side, self.game_state))

            name = "ally2"
            rospy.Subscriber(namespace + game_to_team[name], Pose2D,
                             lambda msg: self.positions["ally2"].import_msg_raw(msg, self.team_side, self.game_state))

            name = "opp1"
            rospy.Subscriber(namespace + game_to_team[name], Pose2D,
                             lambda msg: self.positions["opp1"].import_msg_raw(msg, self.team_side, self.game_state))

            name = "opp2"
            rospy.Subscriber(namespace + game_to_team[name], Pose2D,
                             lambda msg: self.positions["opp2"].import_msg_raw(msg, self.team_side, self.game_state))

            name = "ball"
            rospy.Subscriber(namespace + game_to_team[name], Pose2D,
                             lambda msg: self.positions["ball"].import_msg_raw(msg, self.team_side, self.game_state))
        else: # if not sim mode, look at our vision
            print("isnot sim mode")
            namespace = "/aimbot_" + self.team_side + "/raw_vision/"


        # Uses the dictionary of position objects to subscribe to all the vision topics
        #print(self.positions)

            name = "ally1"
            rospy.Subscriber(namespace + name, Pose2D,
                             lambda msg: self.positions["ally1"].import_msg_raw(msg, self.team_side, self.game_state))

            name = "ally2"
            rospy.Subscriber(namespace + name, Pose2D,
                             lambda msg: self.positions["ally2"].import_msg_raw(msg, self.team_side, self.game_state))

            name = "opp1"
            rospy.Subscriber(namespace + name, Pose2D,
                             lambda msg: self.positions["opp1"].import_msg_raw(msg, self.team_side, self.game_state))

            name = "opp2"
            rospy.Subscriber(namespace + name, Pose2D,
                             lambda msg: self.positions["opp2"].import_msg_raw(msg, self.team_side, self.game_state))

            name = "ball"
            rospy.Subscriber(namespace + name, Pose2D,
                             lambda msg: self.positions["ball"].import_msg_raw(msg, self.team_side, self.game_state))


        #for name, pos in self.positions.iteritems():
            #exec("name%d = name" % (n)) in locals()
            #eval("name%d" % (n))
        #    rospy.Subscriber(namespace + game_to_team[name], Pose2D, lambda msg: self.positions[name].import_msg_raw(msg, self.team_side, self.game_state))


        ## Apparently Ros can't handle subscriptions that hanppen in a for loop, so we have to do them one-by-one for now
        # the issue is when the self.positions[x] have the same variable x

    def init_vision_pub(self):
        """Inits publishers for the correctly oriented positions"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"

        # Adds position publishers to the publishers dictionary
        for name, pos in self.positions.items():
            self.publishers[name] = rospy.Publisher(namespace + name, Pose2D, queue_size=10)

    def vision_pub(self):
        """Publishes the correctly oriented positions"""
        # publishes positions

        for name, pos in self.positions.items():
            #print(name, pos.x, pos.y, pos.theta)
            #TODO remove debug code
            #if (name != "ally1"):
            self.publishers[name].publish(self.positions[name].export())
            #else:
            #    msg = Pose2D()
            #    msg.x = -1
            #    msg.y = 0


            #    msg.theta = 0
            #    self.publishers["ally1"].publish(msg)


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
