
from Position import Position
from PathPlanner import PathPlanner

path_plan = PathPlanner()




waypoints = []
robotopp1 = Position()
opp2 = Position()
thing = Position()

ball = Position()
objects = []
objects1 = []


robotopp1.x = 1
robotopp1.y = 0

opp2.x = 0
opp2.y = 0

thing.x = -.5
thing.y = 0
objects.append(robotopp1)
objects.append(opp2)
objects.append(thing)

objects1.append(thing)
objects1.append(robotopp1)
objects1.append(opp2)


#path_plan.obj_to_avoid.append(thing)

#where we want to go
des_pos = Position()
des_pos.x = 2
des_pos.y = 0
#where we are
pos = Position()
pos.x = -2
pos.y = 0

#path_plan.des_pos.x = 1.5
#path_plan.des_pos.y = -0.5

#path_plan.pos.x = -1
#path_plan.pos.y = -0.5005

#ball.x = 0.1
#ball.y = 0.1
#path_plan.ball = ball

pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos1 is: ", pos.x, pos.y)

pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos2 is: ", pos.x, pos.y)

pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos3 is: ", pos.x, pos.y)

pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos4 is: ", pos.x, pos.y)

pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos5 is: ", pos.x, pos.y)

pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos6 is: ", pos.x, pos.y)

pos = path_plan.calc_waypoints(pos, des_pos, objects)
print ("pos7 is: ", pos.x, pos.y)