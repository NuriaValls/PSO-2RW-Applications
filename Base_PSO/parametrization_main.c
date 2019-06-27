#include <stdlib.h>
#include <time.h>

#include "logica.h"


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
    char msg[LENGTH];
    //float v_fraction[] = {0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};
    //float v_fraction[] = {0.9, 0.7, 0.5, 0.3, 0.1};
    //int pop_fraction[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50};
    //int pop_fraction[] = {4, 10, 20, 50};
    FILE *fp;
    //clock_t start, stop = 0;


    //fp=fopen("simpler_version2.txt","w");
    fp = fopen("mmaxmmin.txt", "w");
    srand(time(NULL));

    if (!checkProgramArguments(argc)) {
        print(MSG_WRONG_ARGS);
        exit(EXIT_FAILURE);
    }

    config = readConfigFile(argv[1]);

    //for (int m = 1; m<4; m++) {
    //   int select_velocity_method = m;
    //for (int p = 0; p < 4; p++) {

    //    config.n = pop_fraction[p];

    //    for (int v = 0; v < 9; v++) {

    //       config.vmax = v_fraction[v];

            for (int i = 0; i < 1000; i++) {

                Swarm swarm;
                int count_convergence = 0;
                double best_fit_local = 0.0;
                int not_converged = 1;
                double best_fit_compare;
                int iter = 1000;

                //start = clock();

                createInitialPopulation(config, &swarm, function);

                while (not_converged) {

                    getFitValues(config, &swarm, function);

                    //sprintf(msg, "%f %f %d/", swarm.best_params[0], swarm.best_params[1], swarm.iterations);
                    //fprintf(fp,msg);

                    best_fit_compare = trunc(swarm.best_fit * 100000000);

                    if (swarm.iterations == 0) {
                        best_fit_local = best_fit_compare;
                    }

                    select_updateVelocity(3, config, &swarm, function, iter);

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

                sprintf(msg, "%f %f %d\n", swarm.best_params[0], swarm.best_params[1], swarm.iterations);
                fprintf(fp,msg);


                //sprintf(msg, "\n");
                //fprintf(fp,msg);
                //sprintf(msg, "%d %f %d %f %lf/", pop_fraction[p], v_fraction[v], swarm.iterations, swarm.best_fit, (double)(stop - start) / CLOCKS_PER_SEC);
                //fprintf(fp, msg);

            }

            //sprintf(msg, "\nv");
            //fprintf(fp, msg);

        //}

        //sprintf(msg, "%d\n", p);
        //debug(msg);
    //}

    fclose(fp);

    return EXIT_SUCCESS;

}

