import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import math
from scipy.stats import gaussian_kde


file = open('first_approach_data_aux_1.txt')

fit1 = []
ite1 = []
tim1 = []
fit2 = []
ite2 = []
tim2 = []
fit3 = []
ite3 = []
tim3 = []

i = 0

for line in file:
    aux = line.rstrip('\n').split(',')

    for a in aux:
        b = a.split(' ')
        if i==0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i=1
        elif i==1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i==2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file1 = open('first_approach_data_aux_2.txt')

for line in file1:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file2 = open('first_approach_data_aux_3.txt')

for line in file2:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file3 = open('first_approach_data_aux_4.txt')

for line in file3:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file4 = open('first_approach_data_aux_5.txt')

for line in file4:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file5 = open('first_approach_data_aux_6.txt')

for line in file5:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file6 = open('first_approach_data_aux_7.txt')

for line in file6:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file7 = open('first_approach_data_aux_8.txt')

for line in file7:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file8 = open('first_approach_data_aux_9.txt')

for line in file8:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0

file9 = open('first_approach_data_aux_10.txt')

for line in file9:
    aux = line.rstrip('\n').split(',')
    for a in aux:
        b = a.split(' ')
        if i == 0:
            fit1.append(float(b[0]))
            ite1.append(int(b[1]))
            tim1.append(float(b[2]))
            i = 1
        elif i == 1:
            fit2.append(float(b[0]))
            ite2.append(int(b[1]))
            tim2.append(float(b[2]))
            i = 2
        elif i == 2:
            fit3.append(float(b[0]))
            ite3.append(int(b[1]))
            tim3.append(float(b[2]))
            i = 0


print(len(fit1))

print(sum(fit1)/220)
print(np.std(fit1))

print(sum(ite1)/220)
print(np.std(ite1))

t_i = []

for i in range(0, 220):
    t_i.append(tim1[i]/ite1[i])

print(sum(t_i)/220)
print(np.std(t_i))

print('\n')

print(sum(fit2)/220)
print(np.std(fit2))

print(sum(ite2)/220)
print(np.std(ite2))

t_i = []

for i in range(0, 220):
    t_i.append(tim2[i]/ite2[i])

print(sum(t_i)/220)
print(np.std(t_i))

print('\n')

print(sum(fit3)/220)
print(np.std(fit3))

print(sum(ite3)/220)
print(np.std(ite3))

t_i = []

for i in range(0, 220):
    t_i.append(tim3[i]/ite3[i])

print(sum(t_i)/220)
print(np.std(t_i))