# PSO and two real world applications

Particle Swarm Optimization (PSO) belongs to a powerful family of optimization techniques inspired by the collective behaviour of social animals. This method has shown promising results in a wide range of applications, especially in computer science. Despite this, a great popularity of such method has not been achieved. Since we believe in the potential of PSO, we propose the following scheme to be able to take advantage of its properties. First, an implementation from scratch in C language of the method has been done, as well as an analysis of its parameters and its performance in function minimization. Then, a second more specific part of this thesis is devoted to the adaptation of the method for solving two real-world applications. The first one, in the field of signal analysis, consists of an optimization method for the numerical analysis of Fourier functions, whereas the second, in the field of computer science, comprises the optimization of neural networks weights' for some small architectures.

## Basic PSO implementation

In the folder [Base_PSO](https://github.com/NuriaValls/TFM_PSO/tree/master/Base_PSO) there are all the files needed to execute the PSO alforithm to minimize a function.

The functions already implemented are:
- Parabola
- Ackley's function
- Multiple global and local minima function
- Rosenbrock's function

The function to optimize can be choosen in the [main](https://github.com/NuriaValls/TFM_PSO/blob/master/Base_PSO/main.c) file in the following function by commenting or uncommenting each of the lines:

```python
float function(float x, float y) {

    /* Parabola with minimum in 0: */
    //return x*x + y*y;

    /* Ackleys function: */
    //return (float) (-20 * exp(-0.2 * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + 20 + M_E);

    /* Multiple global and local minima function: */
    return (float) (x * x * (4 - 2.1 * x * x + (1 / 3) * x * x * x * x) + x * y + y * y * (-4 + 4 * y * y));

    /* Rosenbrock function: */
    //return (1-x)*(1-x) + 100*(y-x*x)*(y-x*x);
}

```

Also the [config_file](https://github.com/NuriaValls/TFM_PSO/blob/master/Base_PSO/config_file.txt) can be changed to modify the execution parameters.

### Usage

In order to execute the algorithm compile using the [makefile](https://github.com/NuriaValls/TFM_PSO/blob/master/Base_PSO/makefile) and execute with the following commands:

```bash
make
psoAlgo config_file.txt
```


## Numerical Fourier analysis application

In the folder [Signal_reco_PSO](https://github.com/NuriaValls/TFM_PSO/tree/master/Signal_reco_PSO) there are all the files needed to execute the adaptation of the PSO algorithm to solve the identification problem of frequencies and amplitudes of an input DFT signal.


### First approach usage

In order to execute the first approach of the implementation, compile the project using the [makefile](https://github.com/NuriaValls/TFM_PSO/blob/master/Signal_reco_PSO/makefile) making sure line 2 states as follows:

```bash
MAIN = main.c
```

Remember that execution parameters can also be changed in the [config_file](https://github.com/NuriaValls/TFM_PSO/blob/master/Signal_reco_PSO/config_file_2.txt).

Finally, execute the following commands:

```bash
make
psoAlgo config_file_2.txt
```

### Second approach usage

In order to execute the second approach of the implementation, compile the project using the [makefile](https://github.com/NuriaValls/TFM_PSO/blob/master/Signal_reco_PSO/makefile) making sure line 2 states as follows:

```bash
MAIN = main_aux.c
```
Remember that execution parameters can also be changed in the [config_file](https://github.com/NuriaValls/TFM_PSO/blob/master/Signal_reco_PSO/config_file.txt).

Finally, execute the following commands:

```bash
make
psoAlgo config_file.txt
```


## Neural Network weights optimization

In the folder [NN_PSO](https://github.com/NuriaValls/TFM_PSO/tree/master/NN_PSO) there are all the files needed to execute the optimization of the weights of a neural network given an architecture using the PSO algorithm.

### Usage

In order to execute the second approach of the implementation, compile the project using the [makefile](https://github.com/NuriaValls/TFM_PSO/blob/master/NN_PSO/makefile).

Remember that execution parameters can also be changed in the [config_file](https://github.com/NuriaValls/TFM_PSO/blob/master/NN_PSO/config_file.txt).

Finally, execute the following commands:

```bash
make
psoAlgo config_file.txt
```


## Contributions

Contributions are welcome :) For any bug report or request please [submit an issue](https://github.com/NuriaValls/TFM_PSO/issues/new).

## Contact

Feel free to contact us for questions and comments:
- GitHub: [NuriaValls](https://github.com/NuriaValls), [praat6](https://github.com/praat6)

## BibTex reference format for Thesis citations

```bash
@misc{PSO_Prat_Valls_thesis,
title={Particle Swarm Optimization (PSO) and two real world applications},
url={https://github.com/NuriaValls/TFM_PSO},
author={Albert Prat, Núria Valls},
year={2019},
}
```

## BibTex reference format for code cotations

```bash
@misc{PSO_Prat_Valls,
title={Particle Swarm Optimization (PSO) and two real world applications},
url={https://github.com/NuriaValls/TFM_PSO},
author={Albert Prat, Núria Valls},
year={2019},
}
```




