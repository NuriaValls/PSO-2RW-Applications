import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import math
from scipy.stats import gaussian_kde


#file = open('parabola.txt')
#file = open('ackley.txt')
#file = open('mmaxmmin.txt')
file = open('rosenbrock.txt')

x = []
y = []
it = []
p = 0

for line in file:
    aux = line.rstrip('\n').split(' ')


    x.append(float(aux[0]))
    y.append(float(aux[1]))
    it.append(int(aux[2]))

    p += 1

fig = plt.figure(figsize=(10,6.66))
ax = fig.add_subplot(111)


delta = 0.025
x_ = np.arange(-3.0, 3.0, delta)
y_ = np.arange(-3.0, 3.0, delta)
X, Y = np.meshgrid(x_, y_)
#Z = -X**2 - Y**2
#Z = (-20 * np.exp(-0.2 * np.sqrt(0.5 * (X * X + Y * Y))) - np.exp(0.5 * (np.cos(2 * np.pi * X) + np.cos(2 * np.pi * Y))) + 20 + np.e)
#Z = (X * X * (4 - 2.1 * X * X + (1 / 3) * X * X * X * X) + X * Y + Y * Y * (-4 + 4 * Y * Y))
Z = (1-X)*(1-X) + 100*(Y-X*X)*(Y-X*X)


CS = ax.contour(X, Y, Z, 20, cmap='RdGy')
ax.clabel(CS, inline=1, fontsize=10)
#ax.set_title('Parabola contour plot')
#ax.set_title('Ackley contour plot')
#ax.set_title('Multiple global and local minima contour plot')
ax.set_title('Rosenbrock contour plot')


plt.xlim(-3, 3)
plt.ylim(-3, 3)

xy = np.vstack([x,y])
z = gaussian_kde(xy)(xy)

# Sort the points by density, so that the densest points are plotted last
idx = z.argsort()
x = [x[i] for i in idx]
y = [y[i] for i in idx]
z = [z[i] for i in idx]
#x, y, z = x[idx], y[idx], z[idx]

ax.scatter(x, y, c=z, s=100, edgecolor='')

#plt.savefig('parabola_contour.png', dpi=300)
#plt.savefig('ackley_contour.png', dpi=300)
#plt.savefig('mgml_contour.png', dpi=300)
plt.savefig('rosenbrock_contour.png', dpi=300)
plt.show()

