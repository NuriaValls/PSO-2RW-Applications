import matplotlib.pyplot as plt
from labellines import labelLine, labelLines

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

v_fraction_list = []
pop_size_list = []
mean_iterations_list = []
mean_fit_list = []
mean_time_list = []

for i in range(10):
    line1 = []
    line2 = []
    line3 = []
    line4 = []
    line5 = []
    for j in range(9):
        line1.append(pop_size[i * 9 + j])
        line2.append(mean_iterations[i * 9 + j])
        line3.append(mean_fit[i * 9 + j])
        line4.append(mean_time[i * 9 + j])
        line5.append(v_fraction[i * 9 + j])
    pop_size_list.append(line1)
    mean_iterations_list.append(line2)
    mean_fit_list.append(line3)
    mean_time_list.append(line4)
    v_fraction_list.append(line5)


fig = plt.figure(figsize=(10,7))
plt.title("Test Parameters")
plt.xlabel('max velocity fraction')
plt.ylabel('iterations')

for i in range(10):
    plt.plot(v_fraction_list[i], mean_iterations_list[i], label=pop_size_list[i][0])

plt.legend(bbox_to_anchor=(1.05, 0.5), loc=6, borderaxespad=1., title='pop_size:')
fig.tight_layout()
plt.savefig('images/iterations_per_vfraction.png')
plt.show()


fig = plt.figure(figsize=(10,7))
plt.title("Test Parameters")
plt.xlabel('max velocity fraction')
plt.ylabel('execution time')

for i in range(10):
    plt.plot(v_fraction_list[i], mean_time_list[i], label=pop_size_list[i][0])

plt.legend(bbox_to_anchor=(1.05, 0.5), loc=6, borderaxespad=1., title='pop_size:')
fig.tight_layout()
plt.savefig('images/time_per_vfraction.png')
plt.show()


fig = plt.figure(figsize=(10,7))
plt.title("Test Parameters")
plt.xlabel('max velocity fraction')
plt.ylabel('fit value')

for i in range(10):
    plt.plot(v_fraction_list[i], mean_fit_list[i], label=pop_size_list[i][0])

plt.legend(bbox_to_anchor=(1.05, 0.5), loc=6, borderaxespad=1., title='pop_size:')
fig.tight_layout()
plt.savefig('images/fit_per_vfraction.png')
plt.show()
