import numpy as np
import matplotlib.pyplot as plt


file1 = open('parabola.txt')
file2 = open('ackley.txt')
file3 = open('mmaxmmin.txt')
file4 = open('rosenbrock.txt')


it1 = []
it2 = []
it3 = []
it4 = []
p = 0

for line in file1:
    aux = line.rstrip('\n').split(' ')
    it1.append(int(aux[2]))
    p += 1

p = 0

for line in file2:
    aux = line.rstrip('\n').split(' ')
    it2.append(int(aux[2]))
    p += 1

p = 0

for line in file3:
    aux = line.rstrip('\n').split(' ')
    it3.append(int(aux[2]))
    p += 1

p = 0

for line in file4:
    aux = line.rstrip('\n').split(' ')
    it4.append(int(aux[2]))
    p += 1


data = [it1, it2, it3, it4]

fig = plt.figure(figsize=(10,7))
ax = fig.add_subplot(111)
ax.set_title('Basic Plot')
ax.boxplot(data)
ax.set_title('Boxplot comparison of the number of iterations per function')
ax.set_xlabel('function index')
ax.set_ylabel('iterations until convergence')

plt.savefig('boxplot.png', dpi=300)
plt.show()
