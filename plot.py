import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt
import itertools

fig = plt.figure()
ax = fig.gca(projection='3d')

f = open("data.txt", "r")
pts = f.read().split("\n")

# plotting the points
for p in pts:
    pp = p.split(" ")
    if(pp[0] is not ""):
        print(pp)
        ax.scatter(int(pp[0]), int(pp[1]), int(float(pp[2])), zdir='z', c='r')

# plotting lines for each point pair
# for a, b in itertools.product(pts, pts):
    # x = np.linspace(a[0], b[0], 100)
    # y = np.linspace(a[1], b[1], 100)
    # z = np.linspace(a[2], b[2], 100)
    # ax.plot(x, y, z)

ax.legend()
ax.set_xlim3d(0, 1500)
ax.set_ylim3d(0, 1500)
ax.set_zlim3d(0, 1500)

plt.show()
