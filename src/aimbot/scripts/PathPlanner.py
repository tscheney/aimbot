
import numpy as np
from Position import Position

class PathPlanner:
    """This class uses the position of the robot and of the all the objects necessary to avoid and
    calculates a new intermediate point as part of an overall path that will guide the robot
    to its desired position without running into anything"""

    def __init__(self):
        self.pos = Position() # the current position of the robot
        self.des_pos = Position() # the eventual position the robot wants to get to, will come from the Robot class
        self.intm_pos = Position() # the intermediate position that this class calculates for the robot to head to in order to
                                    # avoid objects and get the desired position
        self.obj_to_avoid = [] # this is a list of objects to avoid
        self.waypoints = [] #list of points to pass through
        self.ball = Position() #need the ball position
        self.goal = Position() #the position of the goal
        self.hand = Position() #center most foward point on robot
        self.point = Position() #intermediate point with angle to face

    def update_pos(self, pos):
        """Updates to the latest current position of the Robot"""
        self.pos = pos

    def update_des_pos(self, des_pos):
        """Updates to the latest desired position from the Robot class"""
        self.des_pos = des_pos

    def update_obj_avoid(self, obj_to_avoid):
        """Updates the objects to avoid list with a list passed as a parameter"""
        self.obj_to_avoid.clear() # clears the old positions from the list before adding the new ones
        for pos in obj_to_avoid:
            self.obj_to_avoid.append(pos)  # add all the new positions

    #TODO here down is new or modified
    def update_goal(self):
        self.goal.x = 1.5 #not sure on the coordinates might be 2
        self.goal.y = 0 #center of the goal
        self.goal.theta = 0 #doesnt really matter

    def calc_waypoints(self):
        """Calculates intermediate position to head to"""
        self.update_goal()
        rad = np.radians(self.pos.theta)
        values = self.hand.x*(np.matrix([[np.cos(rad)], np.sin(rad)]))

        self.point.x = values[0, 0]
        self.point.y = values[0, 1]
        print(self.point.x,self.point.y)
        print(self.goal.x, self.goal.y)
        # add the point to the list
        self.add_waypoint()
        #calculate the x and y of our next point
        self.point.x = self.ball.x - 2*self.hand.x*(self.goal.x-self.ball.x)/(abs(self.goal.x+self.ball.x))
        self.point.y = self.ball.y - 2*self.hand.y*(self.goal.y-self.ball.y)/(abs(self.goal.y+self.ball.y))
        self.add_waypoint()
        self.point = self.goal
        self.add_waypoint()

    def add_waypoint(self):
        self.waypoints.append(self.point)

    def waypoint_clear(self):
        """clears the waypoints from the list"""
        self.waypoints.clear()

    def print_waypoints(self, waypoints):
        """prints out the waypoints for debugging purposes"""
        for self.point in waypoints:
            print(self.point.x, self.point.y)

# notes on waypoint path finding from http://rwbclasses.groups.et.byu.net/lib/exe/fetch.php?media=robot_soccer:material:waypoints.pdf
#   1.) He suggests having a few functions, one that will clear the waypoint
#       stack, another that will push a new waypoint onto the stack, another
#       that will remove one (pop) from the stack, and a debugging one that
#       lets us see what points are on the stack.
#
#   2.) For now im not doing object avoidance, ill try to make it so that
#       it can get behind the ball and push towards the goal, from there
#       we can add in the abilty to avoid the objects in our way as we
#       go to score
#   Effectivly this would be a smarter version of rush goal, it will try to
#   get behind the ball then push to the goal.