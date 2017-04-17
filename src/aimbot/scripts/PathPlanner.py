
import numpy as np
from numpy import ones,vstack
from numpy.linalg import lstsq
from Position import Position
import math as math
#import dubins

d = { 'x': 192, 'y': 85}

arr = [ [5,1], [5,5] ]
arr.append(d)

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
        self.distances = [] #list of points to pass through
        self.ball = Position() #need the ball position
        self.point = Position() #intermediate point with angle to face
        self.previous = Position() #temporary storage of previous point on the line we are checking.
        self.first_pass = False #True when we dont go into while loop of the check line function.

    def update_pos(self, pos):
        """Updates to the latest current position of the Robot"""
        self.pos = pos

    def update_des_pos(self, des_pos):
        """Updates to the latest desired position from the Robot class"""
        self.des_pos = des_pos

    def update_obj_avoid(self, obj_to_avoid):
        """Updates the objects to avoid list with a list passed as a parameter"""
        i = 0
        del self.obj_to_avoid[:] # clears the old positions from the list before adding the new ones
        for obj in obj_to_avoid:
            print("AVOID THIS STUFF: ",obj.x, obj.y)
            dist = math.hypot(self.pos.x - obj.x, self.pos.y - obj.y)
            print(dist)
            self.obj_to_avoid.append([obj, dist])  # add all the new positions

        if (len(self.obj_to_avoid) > 1):
            self.obj_to_avoid = sorted(self.obj_to_avoid, key = lambda x: x[1]) #sort the list by the distance values
            for obj in obj_to_avoid:
                print("(ordered by distance) x: y: dist: : ", obj_to_avoid[i].x, obj_to_avoid[i].y)
                i = i + 1
    def calc_waypoints(self, curr_position, des_position, obj_to_avoid):
        """Calculates intermediate position to head to"""
        #need to calculate where to go next one point at a time, pass this back and go there then call this again
        #model robots as circles to make it easier
        #this site seems useful http://devmag.org.za/2009/04/13/basic-collision-detection-in-2d-part-1/

        #update where we are and where we want to go
        self.update_pos(curr_position)
        self.update_des_pos(des_position)
        #update things to avoid
        self.update_obj_avoid(obj_to_avoid)

        #check to see if there is anything in our way on our way to our desired position
        for avoid in obj_to_avoid:
            print("avoid this spot: ", avoid.x, avoid.y)
            if (self.check_line(avoid.x, avoid.y)): #pass in x,y values of thing to avoid
                #make new path
                #idea from other team, they say it works.
                #move the line that we are trying to go down to one side or the other
                # then check it and move down it till you can go downt the original line that you wanted.
                if (self.first_pass):
                    #we are very close to an object to avoid so we need to move the movement vector
                    print("very close to object")
                    #pick a new des_position
                    #go up 2 inches from thing to avoid
                    self.point.x = 100
                    self.point.y = 100
                    #call check_line on the new point and ideally it will work.
                    return self.point
                else:
                    print("not too close, get closer ;)")
                    self.point = self.previous
                    return self.point
            else:
                print("here")
                self.point = des_position
        return self.point

    def check_line(self, avoid_x, avoid_y):
        '''This will move allong the our line of desired movement and check points every radius
        down the line to see if they are less than 2*Radius of the thing to avoid.
        Returns a boolean'''
        #calculate the line of movement
        points = [(self.pos.x, self.pos.y), (self.des_pos.x, self.des_pos.y)]
        x_coords, y_coords = zip(*points)
        A = vstack([x_coords, ones(len(x_coords))]).T
        m, b = lstsq(A, y_coords)[0]
        print "Line Solution is y = {m}x + {b}".format(m=m, b=b)

        #radius of robots assuming they are as wide as the rules allow
        robot_radius = 0.1016

        #calculate x and y to plug into our line equation to move down it 1 radius of length
        x_next = self.pos.x + (robot_radius/(math.sqrt(robot_radius**2 + (robot_radius*m)**2)))*(robot_radius)
        y_next = self.pos.y + ((robot_radius*m)/(math.sqrt(robot_radius**2 + (robot_radius*m)**2)))*(robot_radius)
        print("x:", x_next)
        print("y:", y_next)
        self.first_pass = True
        #store previous values so that if we need to we can use them as a way of generating a new path
        self.previous.x = self.pos.x
        self.previous.y = self.pos.y
        while (x_next <= self.des_pos.x):
            #grab distance between x and y next values and the thing to avoid.
            dist = math.hypot(x_next - avoid_x, y_next - avoid_y)
            if dist < 2*robot_radius:
                print("distance is: ", dist)
                return True
            else:
                print("distance is fine: ", dist)
                #update the previous values before calculating the next point down the line
                self.previous.x = x_next
                self.previous.y = y_next

                self.first_pass = False
                #update our new points to check
                # making it so it doenst move 1 meter down the line multiply by radius of robot
                x_next = x_next + (robot_radius / (math.sqrt(robot_radius ** 2 + (robot_radius * m) ** 2)))*(robot_radius)
                y_next = y_next + ((robot_radius * m) / (math.sqrt(robot_radius ** 2 + (robot_radius * m) ** 2)))*(robot_radius)
                print("x1:", x_next)
                print("y1:", y_next)

        dist = math.hypot(self.des_pos.x - avoid_x, self.des_pos.y - avoid_y)
        #print("distance is: ", dist)
        #print("robot_radius*2: ", 2*robot_radius)
        if (dist < 2*robot_radius):
            return True
        return False #no collision

    def truncate(self, f, n):
        '''Truncates/pads a float f to n decimal places without rounding'''
        s = '{}'.format(f)
        if 'e' in s or 'E' in s:
            return '{0:.{1}f}'.format(f, n)
        i, p, d = s.partition('.')
        return '.'.join([i, (d + '0' * n)[:n]])

    def add_waypoint(self):
        self.waypoints.append(self.point)
        #print("added the following point")
        #print(self.point.x, self.point.y, self.point.theta)

    def waypoint_clear(self):
        """clears the waypoints from the list"""
        del self.waypoints[:]

    def print_waypoints(self, waypoints):
        """prints out the waypoints for debugging purposes"""
        i = 0
        print("list starts here, this only prints the last item the number of items in the list times.")
        for point in waypoints:
            print(point.x, point.y, point.theta, "index ->", i)
            i = i +1

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





