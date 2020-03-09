import matplotlib.pyplot as plt
import numpy as np
import csv

import matplotlib.pyplot as plt
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

mpl.rcParams["font.family"] = "FreeSerif"
plt.rc("text", usetex=True)

x = []
y = []
z =[]

with open("gammaMeshEdepNeutron.csv", "r") as f:
    reader = csv.reader(f, delimiter=',')
    count = 0
    for row in reader:
        if (count > 2):
            x.append(row[0])
            y.append(row[1])
            z.append(float(row[3]))
        count+= 1

x_array = np.array(x)
y_array = np.array(y)
z_array = np.array(z)

Z = np.reshape(z, (-1, 75))
Z = np.rot90(Z)

ax = plt.axes()
ax.imshow(Z, cmap = "hot")
ax.yaxis.set_major_locator(plt.NullLocator())
ax.xaxis.set_major_formatter(plt.NullFormatter())
plt.title("$\gamma$ Energy Deposition")
# plt.savefig("[Gamma][xbin250][ybin75][500000Events].png")
plt.show()