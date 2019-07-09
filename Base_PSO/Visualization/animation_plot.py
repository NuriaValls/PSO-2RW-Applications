import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.animation as animation
import numpy as np


file = open('execution_02.txt')

particles = np.random.rand(300, 2, 10)
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
x = np.arange(-1.5, 1.5, delta)
y = np.arange(-1.5, 1.5, delta)
X, Y = np.meshgrid(x, y)
Z = (-20 * np.exp(-0.2 * np.sqrt(0.5 * (X * X + Y * Y))) - np.exp(0.5 * (np.cos(2 * np.pi * X) + np.cos(2 * np.pi * Y))) + 20 + np.e)

CS = ax.contour(X, Y, Z, 20, cmap='RdGy')
ax.clabel(CS, inline=1, fontsize=10)
ax.set_title('Simplest default with labels')


plt.xlim(-1.5, 1.5)
plt.ylim(-1.5, 1.5)

def animate(i):

    ax.cla()

    CS = ax.contour(X, Y, Z, 20, cmap='viridis')
    ax.clabel(CS, inline=1, fontsize=10)
    ax.scatter(particles[i, 0], particles[i, 1], c='blue')

    plt.xlim(-1.5, 1.5)
    plt.ylim(-1.5, 1.5)

ani = FuncAnimation(fig, animate, frames=300, interval=500, repeat_delay=5000)

ani.save('ackley_animation.gif', writer="ffmpeg")


#plt.show()

