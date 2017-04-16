import rospy
from Robot import Robot
from GameStateObj import GameStateObj
from Position import Position
from soccerref.msg import GameState
from geometry_msgs.msg import Pose2D
from std_msgs.msg import Int16
import numpy as np
import constants
import roles
import shapely.geometry as geo

class Team:

    """Class that determines team strategy based on game state and publishes
    roles to the robots
    all topics published by the game class are published under the node
    /aimbot_(team_side)/team/"""

    def __init__(self, team_side):
        #self.robots = [Robot(), Robot()]
        self.num_robots = 2
        self.num_opp_robots = 2
        self.game_state = GameStateObj()
        self.team_side = team_side
        self.positions = dict() #ally1=Position(), ally2=Position, opp1=Position(), opp2=Position(), ball=Position())
        self.publishers = dict()
        self.state = dict()
        self.roles = dict()
        self.init_pos()
        self.init_publsihers()
        self.init_state()
        self.debug = False
        self.play_without_soccerref = False

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

    def init_state(self):
        """Init the state dicationary"""
        self.state["attacker"] = "ally1"
        self.state["defender"] = "ally2"

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
                         lambda msg: self.positions["ally1"].import_msg(msg))

        name = "ally2"
        rospy.Subscriber(namespace + name, Pose2D,
                         lambda msg: self.positions["ally2"].import_msg(msg))

        name = "opp1"
        rospy.Subscriber(namespace + name, Pose2D,
                         lambda msg: self.positions["opp1"].import_msg(msg))

        name = "opp2"
        rospy.Subscriber(namespace + name, Pose2D,
                         lambda msg: self.positions["opp2"].import_msg(msg))

        name = "ball"
        rospy.Subscriber(namespace + name, Pose2D,
                         lambda msg: self.positions["ball"].import_msg(msg))

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
        #print("penalty for home is", self.game_state.home_penalty)
        #print("team side is", self.team_side)
        #print("penalty for away is", self.game_state.away_penalty)
        if (self.debug == True):
            self.roles['ally1'] = roles.SCORE
            self.roles['ally2'] = roles.DEFEND_GOAL
        elif(self.game_state.play or self.play_without_soccerref):
            self.determine_game_state()

        #penalty stuff must be here due to the way it is set up the the ref code.
            #if you are not in penalty they send the reset field signal and if this is after the elif for reset field
            #then it takes that branch and just resets the field. So leave this section before the reset field branch.
        elif (self.game_state.home_penalty and self.team_side == 'home'):
            self.roles['ally1'] = 105
            self.roles['ally2'] = 106
        elif (self.game_state.away_penalty and self.team_side == 'away'):
            self.roles['ally1'] = 107
            self.roles['ally2'] = 108
        #reset field branch here
        elif(self.game_state.reset_field):
            self.roles[self.state["attacker"]] = 103
            self.roles[self.state["defender"]] = 104
        else:
            self.roles['ally1'] = roles.STAY_PUT  # these are just test roles
            self.roles['ally2'] = roles.STAY_PUT

    def set_roles_balanced(self):
        """Use balanced strategy"""
        self.switch_on_balanced()
        self.roles[self.state["attacker"]] = roles.SCORE
        self.roles[self.state["defender"]] = roles.FOLLOW_BALL
        if (not self.robot_clear_of_ball("defender")):  # if the ball is not behind the defender
            self.roles[self.state["defender"]] = roles.GET_BEHIND_BALL

    def set_roles_offense(self):
        """Use offensive strategy"""
        self.switch_on_offense()
        self.roles[self.state["attacker"]] = roles.SCORE
        self.roles[self.state["defender"]] = roles.BACKUP_OFFENSE
        if (not self.robot_clear_of_ball("defender")):  # if the ball is not behind the defender
            self.roles[self.state["defender"]] = roles.GET_BEHIND_BALL

    def set_roles_defense(self):
        """Use defensive stragegy"""
        self.switch_on_defense()
        self.roles[self.state["attacker"]] = roles.SCORE
        self.roles[self.state["defender"]] = roles.DEFEND_GOAL
        if (self.ball_behind_robot("defender", constants.ball_behind_thresh)):  # if the ball is not behind the defender
            self.roles[self.state["defender"]] = roles.GET_BEHIND_BALL

    def get_distance_between_points(self, pos1, pos2):
        """Gets the distance between two points"""
        a = pos2.x - pos1.x
        b = pos2.y - pos1.y
        c = np.sqrt(a ** 2 + b ** 2)
        return c

    def determine_game_state(self):
        """Based on known positions and score, determine which state the game is in
        Determines the roles that should be used during game play"""
        if(self.ball_behind_robot("attacker", constants.ball_behind_thresh) and self.ball_behind_robot("defender", constants.ball_behind_thresh)): # ball is behind both robots
            if(self.positions[self.state["attacker"]].x >= self.positions[self.state["defender"]].x):
                self.switch_roles()
            self.roles[self.state["attacker"]] = roles.BOTH_GET_BEHIND_BALL
            self.roles[self.state["defender"]] = roles.BOTH_GET_BEHIND_BALL
        else:
            ball_x = self.positions["ball"].x
            if ball_x > constants.field_width / 3:
                self.set_roles_offense()
            # if the ball is futher down the field than the follow distance plus half the width of the robot
            elif ball_x < (-1 * ((constants.field_width / 2) - (constants.follow_distance + (constants.robot_width / 2)))):
                self.set_roles_defense()
            else:
                self.set_roles_balanced()



    # def switch_Roles(self, defender):
    #     """If defender is close enough to the ball, become the attacker
    #         and have the attacker go to defense"""
    #     # Use numpy to create vectors
    #     if defender == 2:
    #         ballvec = np.array([[self.positions["ball"].x], [self.positions["ball"].y]])
    #         mevec = np.array([[self.positions["ally2"].x], [self.positions["ally2"].y]])
    #     else:
    #         ballvec = np.array([[self.positions["ball"].x], [self.positions["ball"].y]])
    #         mevec = np.array([[self.positions["ally1"].x], [self.positions["ally1"].y]])
    #     field_width = 3.53
    #     goalvec = np.array([[field_width / 2], [0]])
    #
    #     # unit vector from ball to goal
    #     uv = goalvec - ballvec
    #     uv = uv / np.linalg.norm(uv)
    #
    #     # compute a position 20cm behind ball, but aligned with goal
    #     p = ballvec - 0.1 * uv
    #
    #     # If I am sufficiently close to the point behind the ball,
    #     # or in other words, once I am 21cm behind the ball, just
    #     # drive to the goal.
    #     if np.linalg.norm(p - mevec) < 0.09:
    #         return True
    #     else:
    #         return False

    def switch_roles(self):
        """Switches the attacker to the defender and the defender to the attacker"""
        if(self.state["attacker"] == "ally1"):
            self.state["attacker"] = "ally2"
            self.state["defender"] = "ally1"
        else:
            self.state["attacker"] = "ally1"
            self.state["defender"] = "ally2"

    def switch_on_offense(self):
        """Determine whether we should switch on offense"""
        attacker_to_ball = self.get_distance_between_points(self.positions[self.state["attacker"]], self.positions['ball'])
        defender_to_ball = self.get_distance_between_points(self.positions[self.state["defender"]], self.positions['ball'])
        if((attacker_to_ball > defender_to_ball) or self.ball_behind_robot("attacker", constants.ball_behind_thresh)): # defender is the closest opponent, or ball is behind attacker
            self.switch_roles()

    def switch_on_balanced(self):
        """Determine whether we should switch while balanced"""
        if(self.ball_behind_robot("attacker", constants.ball_behind_thresh)):
            self.switch_roles()

    def switch_on_defense(self):
        """Determine whether we should switch on defense"""
        #if in goalies wheels house
        if(self.ball_in_robot_wheelhouse("defender")):
            print("Ball in def wheel")
            self.switch_roles()

    def ball_in_robot_wheelhouse(self, robot):
        """Returns true of the ball is in the robot wheel house"""
        pos = self.positions[self.state[robot]]
        ball_pos = self.positions["ball"]
        ball_pos_Point = geo.Point(ball_pos.x, ball_pos.y)
        y_low = pos.y - constants.goalie_wheelhouse_width / 2 #left side of the wheelhouse if standing behind robot
        y_high = pos.y + constants.goalie_wheelhouse_width / 2
        x_high = (pos.x + constants.goalie_wheelhouse_length)
        x_low = pos.x
        wheelhouse = geo.Polygon([(x_low, y_low), (x_high, y_low), (x_high, y_high), (x_low, y_high)])
        return ball_pos_Point.within(wheelhouse)

    def ball_behind_robot(self, robot, thresh):
        """Return true if ball is far enough behind the attacker/defender"""
        robot_x = self.positions[self.state[robot]].x
        ball_x = self.positions["ball"].x
        return (robot_x - thresh) > ball_x

    def robot_clear_of_ball(self, robot):
        """Return true if the robot is clear of the ball"""
        robot_x = self.positions[self.state[robot]].x
        ball_x = self.positions["ball"].x
        return (robot_x + constants.robot_clear_thresh) < ball_x






