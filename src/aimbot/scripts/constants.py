import roles

NAMESPACE = '/aimbot/'
field_width = 3.53
field_height = 2.39
robot_width = 0.08573 * 2
goal_box_width = 0.8
dis_from_ball = 0


#
goal_y_thresh = goal_box_width / 2 - 0.2
follow_distance = 1
goalie_wheelhouse_width = robot_width / 2
goalie_wheelhouse_length = 3* robot_width / 2
ball_behind_thresh = robot_width
robot_clear_thresh = robot_width * 2
get_behind_ball_space = 0.25
backup_off_switch_thresh = field_height / 8