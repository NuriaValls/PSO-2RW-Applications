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
    config.d = atoi(aux);
    free(aux);

    for (int i=0; i<config.d; i++) {
        Range range;

        aux = readFileDescriptor(file);
        range.min = atoi(aux);
        free(aux);

        aux = readFileDescriptor(file);
        range.max = atoi(aux);
        free(aux);

        config.param_range = realloc(config.param_range, (size_t) sizeof(range)*(i + 1));
        config.param_range[i] = range;

    }

    close(file);

    sprintf(msg, "|%d/%d|\n", config.n, config.d);
    debug(msg);

    return config;
}


void createInitialPopulation(Config config, Swarm *swarm) {

    // necessitem saber els limits de l'espai de parametres per donar un valor inicial aleatori a les partícules.
    // modificar el lector de fitxer!!


}
