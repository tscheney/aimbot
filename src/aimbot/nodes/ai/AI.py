import numpy as np
import time as time

field_width = 3.53

state = 0

class AI(object):
    def __init__(self, team_side, ally_number):
        super(AI, self).__init__()

        # Which team side (home/away) am I on?
        self.team_side = team_side

        # Am I ally1?
        self.ally1 = (ally_number == 1)
        

    def strategize(self, me, ally, opp1, opp2, ball, game_state):
        if self.ally1:
            # rush ball
            cmds = self.rush_goal(me, ball)
            cmds = self.move_square(me)
        else:
            # be a goalie (i.e., follow line on ball)
            if ball.x < 0:
                cmds = self.follow_ball_on_line(ball, -1.25)
            else:
                cmds = self.follow_ball_on_line(ball, 0)

        return cmds

    def follow_ball_on_line(self, ball, x_c):
        y_c = ball.y
        theta_c = 0
        return (x_c, y_c, theta_c)


    def rush_goal(self, me, ball):
        # Use numpy to create vectors
        ballvec = np.array([[ball.x], [ball.y]])
        mevec = np.array([[me.x], [me.y]])
        goalvec = np.array([[field_width/2], [0]])

        # unit vector from ball to goal
        uv = goalvec - ballvec
        uv = uv/np.linalg.norm(uv)

        # compute a position 20cm behind ball, but aligned with goal
        p = ballvec - 0.20*uv

        # If I am sufficiently close to the point behind the ball,
        # or in other words, once I am 21cm behind the ball, just
        # drive to the goal.
        if np.linalg.norm(p - mevec) < 0.21:
            cmdvec = goalvec
        else:
            cmdvec = p

        return (cmdvec.flatten()[0], cmdvec.flatten()[1], 0)

    def go_to(self,x,y):
        return (x,y,0)


    def move_to_center(self):
        return self.go_to(0,0)

    def move_square(self, me):
        desired = 0.15
        error = 0.03
        print(me.x, ',', me.y)
        global state


        # transistions
        time.sleep(3)
        #if (self.tolerance(me.x, -desired, error) and self.tolerance(me.y, -desired, error)):
        #    state = 3
        #elif (self.tolerance(me.x, -desired, error) and self.tolerance(me.y, desired, error)):
        #    state = 0
        #elif (self.tolerance(me.x, desired, error) and self.tolerance(me.y, desired, error)):
        #    state = 1
        #elif (self.tolerance(me.x, desired, error) and self.tolerance(me.y, -desired, error)):
        #    state = 2

        #if(state <3):
        #    state += 1
        #else:
        #    state = 0


        print(state)
        # actions
        if(state == 0): # going to upper right
            state = 1
            return self.go_to(0.15, 0.15)
        elif(state == 1): # going to lower right
            state = 2
            return self.go_to(0.15, -0.15)
        elif(state == 2): # going to lower left
            state = 3
            return self.go_to(-0.15, -0.15)
        elif(state == 3): # going to upper left
            state = 0
            return self.go_to(-0.15, 0.15)


    def tolerance(self, pos, desired, error):
        #if(pos >= 0):
            return pos >= (desired - error) and pos <= (desired + error)
        #else:
            #return pos <= (desired - error) and pos >= (desired + error)




