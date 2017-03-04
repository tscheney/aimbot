
import numpy as np
from Position import Position

class PathPlanner:
    def __init__(self):
        self.pos = Position() # the current position of the robot
        self.des_pos = Position() # the eventual position the robot wants to get to, will come from the Robot class
        self.intm_pos = Position() # the intermediate position that this class calculates for the robot to head to in order to
                                    # avoid objects and get the desired position
        self.obj_to_avoid = [] # this is a list of obejcts to avoid

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

        def calc_intm_pos(self):
            """Calculates intermiate position to head to"""


