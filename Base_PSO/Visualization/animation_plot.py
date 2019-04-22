import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np


file = open('execution_01.txt')

particles = np.random.rand(140, 2, 10)
p = 0

for line in file:
    aux = line.rstrip('\n').split('|')

    x = aux[0].split(',')
    y = aux[1].split(',')

    for i in range(0, 10):
        particles[p, 0, i] = x[i]
        particles[p, 1, i] = y[i]

    p += 1

print(p)
print(particles.shape)

# create some random data
data = np.random.rand(25, 2, 10)

fig = plt.figure(figsize=(15,10))
ax = fig.add_subplot(111)


delta = 0.025
x = np.arange(-1.0, 1.0, delta)
y = np.arange(-2.0, 2.0, delta)
X, Y = np.meshgrid(x, y)
Z = -X**2 - Y**2

CS = ax.contour(X, Y, Z, 20, cmap='RdGy')
ax.clabel(CS, inline=1, fontsize=10)
ax.set_title('Simplest default with labels')


plt.xlim(-1, 1)
plt.ylim(-2, 2)

def animate(i):

    ax.cla()

    CS = ax.contour(X, Y, Z, 20, cmap='viridis')
    ax.clabel(CS, inline=1, fontsize=10)
    ax.scatter(particles[i, 0], particles[i, 1], c='blue')

    plt.xlim(-1, 1)
    plt.ylim(-2, 2)

ani = FuncAnimation(fig, animate, frames=140, interval=500, repeat_delay=5000)


plt.show()

