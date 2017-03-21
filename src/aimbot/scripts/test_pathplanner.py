
from Position import Position
from PathPlanner import PathPlanner

path_plan = PathPlanner()




waypoints = []
robotopp1 = Position()
opp2 = Position()

ball = Position()
objects = []


robotopp1.x = 1
robotopp1.y = -0.5

opp2.x = 0
opp2.y = 1.2
objects.append(robotopp1)
objects.append(opp2)

#path_plan.obj_to_avoid.append(robotopp1)

#where we want to go
des_pos = Position()
des_pos.x = 1.5
des_pos.y = -0.5
#where we are
pos = Position()
pos.x = -1
pos.y = -0.5

#path_plan.des_pos.x = 1.5
#path_plan.des_pos.y = -0.5

#path_plan.pos.x = -1
#path_plan.pos.y = -0.5005

#ball.x = 0.1
#ball.y = 0.1
#path_plan.ball = ball

path_plan.calc_waypoints(pos, des_pos, objects)
waypoints = path_plan.waypoints

path_plan.print_waypoints(waypoints)