
from Position import Position
from PathPlanner import PathPlanner

path_plan = PathPlanner()




waypoints = []
robotopp1 = Position()
opp2 = Position()
thing = Position()

ball = Position()
objects = []


robotopp1.x = 1
robotopp1.y = -0.5

opp2.x = 0
opp2.y = 1.2

thing.x = 1
thing.y = -1.3
objects.append(robotopp1)
objects.append(opp2)
objects.append(thing)

#path_plan.obj_to_avoid.append(thing)

#where we want to go
des_pos = Position()
des_pos.x = 1.5
des_pos.y = -1.5
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

pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos is: ", pos.x, pos.y)

print("run code again")
pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos is: ", pos.x, pos.y)
