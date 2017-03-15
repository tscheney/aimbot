import numpy as np

class P:
    Ts = 0.01
    tau = 0.05

class Observer:
    """Class to determine unknown states of motions"""
    def __init__(self, sP):
        self.A = sP['A']  # A matrix for observer
        self.B = sP['B']  # B matrix for observer
        self.C = sP['C']  # C matrix for observer
        self.K = sP['K']  # Observer coefficient
        self.ki = sP['ki']  # Observer coefficient
        self.kr = sP['kr']  # Observer coefficient
        self.L = sP['L']  # Correction coefficient used by xhat
        self.Ld = sP['Ld']  # Correction coefficient used by dhat
        self.F_max = sP['F_max']  # Maximum force applied to the system
        self.max_step = sP['max_step']  # Max step between current position and desired position.
        self.x_e = sP['x_e']  # Equilibrium states
        self.type = sP['type']
        self.xhat = np.matrix([[0.0], [0.0]])  # Estimated state (for observer)
        self.dhat = np.matrix([[0.0]])  # Estimate disturbance
        self.F = 0  # Current force being supplied to the system
        self.F_e = 0  # Equilibrium force
        self.error_dl = 0  # Computed error from the last time step
        self.integrator = 0  # Integrator used for error correction
        self.acceleration = 0.0  # Acceleration state
        self.d = 0  # desired position
        self.d_step = 0  # desired step towards the desired position

    def updateObserver(self):
        """Updates the observer"""

        self.getNextStep()

        N = 1

        for i in range(0, N):
            # the following line causes the x velocity to flip when the scale factor is not 1
            self.xhat = self.xhat + P.Ts / N * (
            self.A * (self.xhat - self.x_e) + self.B * (self.F - self.F_e + self.dhat))  # +m.L*(x_c-m.C*xhat))





            self.dhat = 0  # m.dhat + P.Ts/N*m.Ld*(m.xdot-m.C2*m.xhat)

        xhat_dot = (self.A * (self.xhat - self.x_e) + self.B * (self.F - self.F_e + self.dhat))  # Calculate xhat_dot
        self.acceleration = xhat_dot[1, 0]  # Grab acceleration from xhat_dot

        error = self.xhat[0, 0] - self.d  # Calculate the positional error

        #if np.abs(self.xhat[
        #              1, 0]) < 0.5 and self.type != 'theta':  # If the velocity is small the integrator will be triggered
        #    self.integrator = self.integrator + (P.Ts / 2) * (error + self.error_dl)

        self.error_dl = error  # Update the error

        # compute the state feedback controller
        F_tilde = -self.K * (self.xhat - self.x_e) + self.kr * (self.d_step) - self.ki * self.integrator  # - dhat
        F_unsat = self.F_e + F_tilde
        self.F = self.sat(F_unsat, self.F_max)
        # print(self.F)
        # self.Fq.put(self.F)
        # integrator anti-windup
        # if self.ki!=0 and self.type != 'theta':
        # self.integrator = self.integrator + P.Ts/self.ki*(self.F-F_unsat)
        # print('integrator',self.integrator)

    def sat(self, u, limit):
        """Saturates the force if it goes beyond the rails"""
        if u > limit:
            out = limit
        elif u < -limit:
            out = -limit
        else:
            out = u
        return out

    def getNextStep(self):
        """Calculates the next step location depending on current location and desired location."""
        curr = self.xhat[0, 0]
        if abs(
            curr - self.d) > self.max_step:  # If the difference between the current and desired position is greater than
            if curr < self.d:  # the max step. The path to get to the desired position is broken down
                self.d_step = curr + self.max_step  # into smaller steps according to the max step size. This allows for smoother
            else:  # and quicker control.
                self.d_step = curr - self.max_step
        else:
            self.d_step = self.d
