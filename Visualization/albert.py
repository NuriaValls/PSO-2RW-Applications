import matplotlib.pyplot as plt

file = open('pop_fraction_parametrization.txt')

pop_size = []
v_fraction = []
mean_iterations = []
mean_fit = []
mean_time = []

for line in file:
    aux = line.rstrip('\n').split('/')
    aux = aux[:-1]
    iterations = []
    fit = []
    time = []

    for iter in aux:
        value = iter.split(' ')
        iterations.append(int(value[2]))
        fit.append(float(value[3]))
        time.append(float(value[4]))

    aux2 = aux[0].split(' ')
    pop_size.append(int(aux2[0]))
    v_fraction.append(float(aux2[1]))
    mean_iterations.append(sum(iterations)/len(iterations))
    mean_fit.append(sum(fit)/len(fit))
    mean_time.append(sum(time)/len(time))