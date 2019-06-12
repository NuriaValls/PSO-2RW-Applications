import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import math
from scipy.stats import gaussian_kde


file = open('first_approach_data.txt')

fit = []
ite = []
tim = []

for line in file:
    aux = line.rstrip('\n').split(' ')
    fit.append(float(aux[0]))
    ite.append(int(aux[1]))
    tim.append(float(aux[2]))

file = open('first_approach_data_2.txt')

for line in file:
    aux = line.rstrip('\n').split(' ')
    fit.append(float(aux[0]))
    ite.append(int(aux[1]))
    tim.append(float(aux[2]))

file = open('first_approach_data_3.txt')

for line in file:
    aux = line.rstrip('\n').split(' ')
    fit.append(float(aux[0]))
    ite.append(int(aux[1]))
    tim.append(float(aux[2]))

file = open('first_approach_data_4.txt')

for line in file:
    aux = line.rstrip('\n').split(' ')
    fit.append(float(aux[0]))
    ite.append(int(aux[1]))
    tim.append(float(aux[2]))

print(sum(fit)/200)
print(np.std(fit))

print(sum(ite)/200)
print(np.std(ite))

t_i = []

for i in range(0, 200):
    t_i.append(tim[i]/ite[i])

print(sum(t_i)/200)
print(np.std(t_i))