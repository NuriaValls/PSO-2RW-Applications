#include <stdlib.h>

#include "logica.h"

#include "imports/Python.h"

int select_velocity_method = 3;
int convergence_factor = 100;


char checkProgramArguments(int argc) {
    return argc == 2;
}

float function(float x, float y) {

    /* Parabola with minimum in 0: */
    return x*x + y*y;

    /* Ackleys function: */
    //return (float) (-20 * exp(-0.2 * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + 20 + M_E);

    /* Multiple global and local minima function: */
    //return (float) (x * x * (4 - 2.1 * x * x + (1 / 3) * x * x * x * x) + x * y + y * y * (-4 + 4 * y * y));
}

int main(int argc, char **argv) {

    Config config;
    Swarm swarm;
    char msg[LENGTH];
    int iter = 1000;
    int count_convergence = 0;
    double best_fit_local = 0.0;
    int not_converged = 1;
    double best_fit_compare;

    /*Py_Initialize();
    PyObject* myModuleString = PyString_FromString((char*)"pyfunction");
    PyObject* myModule = PyImport_Import(myModuleString);
    PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"absolute");
    PyObject* args=PyTuple_Pack(1,PyFloat_FromDouble(2.0));
    PyObject* myResult = PyObject_CallObject(myFunction,args);
    double result = PyFloat_AsDouble(myResult);

    sprintf(msg, "%f\n", result);
    debug(msg);*/

    srand(time(NULL));

    if (!checkProgramArguments(argc)) {
        print(MSG_WRONG_ARGS);
        exit(EXIT_FAILURE);
    }

    config = readConfigFile(argv[1]);

    createInitialPopulation(config, &swarm, function);

    while (not_converged) {

        getFitValues(config, &swarm, function);

        sprintf(msg, "I: %d, Best fit value: %f, Params: x=%f y=%f\n", swarm.iterations, swarm.best_fit,
                swarm.best_params[0], swarm.best_params[1]);
        debug(msg);

        best_fit_compare = trunc(swarm.best_fit * 100000000);

        if (swarm.iterations == 0) {
            best_fit_local = best_fit_compare;
        }

        select_updateVelocity(select_velocity_method, config, &swarm, function, iter);

        updateParameters(config, &swarm);

        if (best_fit_local == best_fit_compare) {
            count_convergence++;
        } else {
            count_convergence = 0;
            sprintf(msg, "\t%f, %f\n",best_fit_local, best_fit_compare);
            debug(msg);
            best_fit_local = best_fit_compare;
        }

        if (count_convergence == convergence_factor) {
            not_converged = 0;
        }

        iter--;
        swarm.iterations++;
    }

    return EXIT_SUCCESS;
}

