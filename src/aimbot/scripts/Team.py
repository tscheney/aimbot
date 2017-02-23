import rospy
from Robot import Robot
from GameStateObj import GameStateObj
from Position import Position
from soccerref.msg import GameState
from geometry_msgs.msg import Pose2D
from std_msgs.msg import Int16

class Team:

    """Class that determines team strategy based on game state and publishes
    roles to the robots
    all topics published by the game class are published under the node
    /aimbot_(team_side)/team/"""

    def __init__(self):
        self.robots = [Robot(), Robot()]
        self.num_robots = 2
        self.num_opp_robots = 2
        self.game_state = GameStateObj()
        self.team_side = rospy.get_param(rospy.search_param('team_side'), 'home')
        self.positions = dict() #ally1=Position(), ally2=Position, opp1=Position(), opp2=Position(), ball=Position())
        self.publishers = dict()
        self.roles = dict()
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

    def game_state_sub(self):
        """Sets up the subscription to the game state"""
        namespace = "/aimbot_" + self.team_side + "/game/"
        rospy.Subscriber(namespace + 'game_state', GameState, lambda msg: self.game_state.import_msg(msg))

    def vision_sub(self):
        """Sets up the subscription to the aimbot vision"""
        namespace = "/aimbot_" + self.team_side + "/game/vision/"

        # Uses the dictionary of position objects to subscribe to all the vision topics
        #for name, pos in self.positions.items():
        #    rospy.Subscriber(namespace + name, Pose2D, lambda msg: pos.import_msg(msg))


        ## Apparently Ros can't handle subscriptions that hanppen in a for loop, so we have to do them one-by-one for now
        # the issue is when the self.positions[x] have the same variable x
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

    def init_roles_pub(self):
        """Inits publishers for team member roles"""

        namespace = "/aimbot_" + self.team_side + "/team/roles/"
        # Create dicationary of robots for roles and publishers
        for i in range(1, self.num_robots + 1):
            name = 'ally' + str(i)
            self.roles[name] = 0
            self.publishers[name] = rospy.Publisher(namespace + name, Int16, queue_size=10)

    def roles_pub(self):
        """Publishes the roles for the team members"""
        for name, role in self.roles.items():
            role = Int16()
            role.data = self.roles[name]
            self.publishers[name].publish(role)

    def subscribe(self):
        """Subscribe to all nodes necessary for the team node"""
        self.game_state_sub()
        self.vision_sub()

    def init_publsihers(self):
        """Inits all publishers in the game node"""
        self.init_roles_pub()

    def publish(self):
        """Publishes all publishers in the team node"""
        self.roles_pub()

    def update(self):
        """Update the teams strategy and roles"""
        if(self.game_state.play):
            self.roles['ally1'] = 1 # these are just test roles
            self.roles['ally2'] = 2
        else:
            self.roles['ally1'] = 0  # these are just test roles
            self.roles['ally2'] = 0

    def determine_game_state(self):
        """Based on known positions and score, determine which state the game is in"""




