import numpy as np

#constants
#outer
eta_out_psuedo = 0.7
om_n_diff_out = 8
a_out = np.matrix([[16, 16, 16], [7.2, 7.2, 7.2]])
eta_out_damping = [0.9, 0.9, 0.9]
om_n_out = [4, 4, 4]

#inner
eta_in_psuedo = [0.7, 0.7, 0.7]
om_n_diff_in = [40, 40, 40]
a_in = np.matrix([[400, 400, 400], [36, 36, 36]])
eta_in_damping = [0.9, 0.9, 0.9]
om_n_in =  [20, 20, 20]

# actual x, y, theta
x = 0
y = 0
phi = 0


# world fram commanded positions
x_des = 1 # the only difference between this and x_com is that x_com includes x_des_dot
y_des = 1
phi_des = 0

com = [x_des, y_des, phi_des]



# calculate approximate derivative
rate = 100
Ts = 1 / rate

#x
x_prev = 1

x_dot = (x_des - x_prev) / Ts

x_com = np.matrix([x_des], [x_dot])

#y
y_prev = 1

y_dot = (y_des - y_prev) / Ts

y_com = np.matrix([y_des], [y_dot])

#z
phi_prev = 1

phi_dot = (phi_des - phi_prev) / Ts

phi_com = np.matrix([phi_des], [phi_dot])


# I guess the x, y, and phi are supposed to be low pass filtered

x_com_dot = np.dot(
    np.matrix([[0, 1], [-om_n_diff_out^2, -(2*eta_out_psuedo*om_n_diff_out)]]),
    x_com) + np.dot(np.matrix([0], [om_n_diff_out^2]), np.matrix([x_des]))

y_com_dot = np.dot(
    np.matrix([[0, 1], [-om_n_diff_out^2, -(2*eta_out_psuedo*om_n_diff_out)]]),
    y_com) + np.dot(np.matrix([0], [om_n_diff_out^2]), np.matrix([y_des]))

phi_com_dot = np.dot(
    np.matrix([[0, 1], [-om_n_diff_out^2, -(2*eta_out_psuedo*om_n_diff_out)]]),
    phi_com) + np.dot(np.matrix([0], [om_n_diff_out^2]), np.matrix([phi_des]))

error = np.matrix([[x], [y], [phi]]) - np.matrix([[x_com[0,0]], [y_com[0,0]], [phi_com[0,0]]])





