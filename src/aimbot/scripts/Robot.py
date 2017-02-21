import numpy as np
from Moving import Moving

class Robot(Moving):
    def __init__(self):
        Moving.__init__()





    def follow_ball_on_line(self, ball, x_c):
        y_c = ball.pos.y
        theta_c = 0
        return (x_c, y_c, theta_c)

    def rush_goal(self, me, ball):
        # Use numpy to create vectors
        ballvec = np.array([[ball.x], [ball.y]])
        mevec = np.array([[me.x], [me.y]])
        goalvec = np.array([[field_width / 2], [0]])

        # unit vector from ball to goal
        uv = goalvec - ballvec
        uv = uv / np.linalg.norm(uv)

        # compute a position 20cm behind ball, but aligned with goal
        p = ballvec - 0.20 * uv

        # If I am sufficiently close to the point behind the ball,
        # or in other words, once I am 21cm behind the ball, just
        # drive to the goal.
        if np.linalg.norm(p - mevec) < 0.21:
            cmdvec = goalvec
        else:
            cmdvec = p

        return (cmdvec.flatten()[0], cmdvec.flatten()[1], 0)


    def go_to(self, x, y):
        return (x, y, 0)

    def move_to_center(self):
        return self.go_to(0, 0)

    def move_square(self, me):
        global state

        print('state is: ',state)
        # actions
        if (state == 0):  # going to upper right
            state = 1
            return self.go_to(0.15, 0.15)
        elif (state == 1):  # going to lower right
            state = 2
            return self.go_to(0.15, -0.15)
        elif (state == 2):  # going to lower left
            state = 3
            return self.go_to(-0.15, -0.15)
        elif (state == 3):  # going to upper left
            state = 0
            return self.go_to(-0.15, 0.15)

    def tolerance(self, pos, desired, error):
        # if(pos >= 0):
        return pos >= (desired - error) and pos <= (desired + error)
        # else:
        # return pos <= (desired - error) and pos >= (desired + error)

    def rotate(self, me):
        theta = me.theta
        if (theta + 10) < 360:
            theta += 40
        else:
            theta = 0
        return (me.x, me.y, theta)
