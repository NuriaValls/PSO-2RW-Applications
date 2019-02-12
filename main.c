#include <stdlib.h>

#include "logica.h"

int main(int argc, char **argv) {

	int pop_size;

	if (!checkProgramArguments(argc)) {
		print(MSG_WRONG_ARGS);
		exit(EXIT_FAILURE);
	}

    readConfigFile(argv[1]);

	return EXIT_SUCCESS;

}
