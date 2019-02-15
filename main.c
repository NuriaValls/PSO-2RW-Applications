#include <stdlib.h>

#include "logica.h"

int main(int argc, char **argv) {

	Config config;
	Swarm swarm;

	if (!checkProgramArguments(argc)) {
		print(MSG_WRONG_ARGS);
		exit(EXIT_FAILURE);
	}

    config = readConfigFile(argv[1]);

	createInitialPopulation(config, &swarm);

	return EXIT_SUCCESS;

}
