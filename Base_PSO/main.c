#include <stdlib.h>

#include "logica.h"


int select_velocity_method = 3;
int convergence_factor = 100;


/**
 * Checks that the program arguments are the desired.
 *
 * @param argc  Argument counter.
 * @return      Flag ok or ko.
 */
char checkProgramArguments(int argc) {
    return argc == 2;
}


/**
 * Function to minimize.
 *
 * @param x     X value of the function at the evaluation point.
 * @param y     Y value of the function at the evaluation point.
 * @return      Value of the function at the point X,Y.
 */
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


/**
 * Main function
 *
 * @param argc  Argument counter.
 * @param argv  Argument value.
 * @return      Process statul.
 */
int main(int argc, char **argv) {

    Config config;
    Swarm swarm;
    char msg[LENGTH];
    int iter = 1000;
    int count_convergence = 0;
    double best_fit_local = 0.0;
    int not_converged = 1;
    double best_fit_compare;


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

