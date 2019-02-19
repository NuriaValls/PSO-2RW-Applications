#include <stdlib.h>

#include "logica.h"

char checkProgramArguments(int argc) {
    return argc == 2;
}

float function(float x, float y) {

    //return x*x + y*y; //  parabola with minimum in 0

    return (float) (-20 * exp(-0.2 * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + 20 + M_E);
}

int main(int argc, char **argv) {

	Config config;
	Swarm swarm;
    char msg[LENGTH];
    int iter = 10000;

	srand(time(NULL));

	if (!checkProgramArguments(argc)) {
		print(MSG_WRONG_ARGS);
		exit(EXIT_FAILURE);
	}

    config = readConfigFile(argv[1]);

	createInitialPopulation(config, &swarm, function);

	while (iter > 0) {

        getFitValues(config, &swarm, function);

        sprintf(msg, "I: %d, Best fit value: %f\n", swarm.iterations, swarm.best_fit);
        debug(msg);

        updateVelocity(config, &swarm, function);

        updateParameters(config, &swarm);

        iter--;
        swarm.iterations++;
	}

	return EXIT_SUCCESS;

}

//PREGUNTES:

//-> Com podem passar com a paràmetre de la llibreria la funció a optimitzar?
//-> Què passa si tenim paràmetres restringits discrets?
//-> Guardo com a inicialització del valor màxim el punt aleatori inicial?
//-> Convergeix amb poques iteracions i a valors poc acurats

// fixar velocitat màxima a x i y
