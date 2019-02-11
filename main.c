#include <stdlib.h>

#include "logica.h"

int main(int argc, char **argv) {

	int pop_size;

	if (!checkProgramArguments(argc)) {
		print(MSG_WRONG_ARGS);
		exit(EXIT_FAILURE); //???
	}

	println(argv[1]);

	pop_size = atoi(argv[1]);
	//print(pop_size);

	return EXIT_SUCCESS;

}
