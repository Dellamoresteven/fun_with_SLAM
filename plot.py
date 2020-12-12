# import matplotlib as mpl
# from mpl_toolkits.mplot3d import Axes3D
# import numpy as np
# import matplotlib.pyplot as plt
# import itertools

# fig = plt.figure()
# ax = fig.gca(projection='3d')

# f = open("data.txt", "r")
# pts = f.read().split("\n")

# # plotting the points
# for p in pts:
    # pp = p.split(" ")
    # if(pp[0] is not ""):
        # print(pp)
        # ax.scatter(int(pp[0]), int(pp[1]), int(float(0)), zdir='z', c='r')

# ax.legend()
# ax.set_xlim3d(0, 1500)
# ax.set_ylim3d(0, 1500)
# ax.set_zlim3d(0, 1500)

# plt.show()


import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

f = open("data.txt", "r")
pts = f.read().split("\n")

data = []
for p in pts:
    if(p == ""):
        break
    pp = p.split(" ")
    data.append([int(pp[0]), int(pp[1])])

print(data)

x, y = zip(*data)

plt.gca().invert_yaxis()

plt.scatter(x,y)
plt.show()
