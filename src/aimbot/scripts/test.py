
from shapely.geometry import Point
import numpy as np

class newPoint(Point):
    def __init__(self):
        Point.__init__(self, 0, 0)
        self.theta = 0

    def update(self, a, b, c):
        self._set_coords(a,b)
        self.theta = c


poo = newPoint()
poo2 = newPoint()
poo3 = newPoint()

poo3.update(0, 10, 0)

print("x: ", poo.x)
print("y: ", poo.y)
print("th: ", poo.theta)

poo.update(2, 2, 30)

print("x: ", poo.x)
print("y: ", poo.y)
print("th: ", poo.theta)

print(poo.xy)


def dis_from_point_to_line(point_pos, pos1, pos2):
    """Gets the distance from (x0, y0) to the line that goes through (x1,y1) and (x2,y2)"""
    # return abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1) / np.sqrt((y2 - y1) ^ 2 + (x2 - x1) ^ 2)
    return abs((pos2.y - pos1.y) * point_pos.x - (
    pos2.x - pos1.x) * point_pos.y + pos2.x * pos1.y - pos2.y * pos1.x) / np.sqrt(
        np.power(pos2.y - pos1.y, 2) + np.power(pos2.x - pos1.x, 2))


print(dis_from_point_to_line(poo, poo2, poo3))