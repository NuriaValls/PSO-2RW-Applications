#include "logica.h"

char checkProgramArguments(int argc) {
	return argc == 2;
}

Config readConfigFile(char *filename) {
    int file;
    char msg[LENGTH], *aux;
    Config config;

    file = open(filename, O_RDONLY);
    if (file <= 0) {
        sprintf(msg, MSG_FILE_ERR, filename);
        print(msg);
        exit(EXIT_FAILURE);
    }

    aux = readFileDescriptor(file);
    config.n = atoi(aux);
    free(aux);

    aux = readFileDescriptor(file);
    config.d = atoi(aux); // NOLINT
    free(aux);

    close(file);

    sprintf(msg, "|%d/%d|\n", config.n, config.d);
    debug(msg);

    return config;
}
