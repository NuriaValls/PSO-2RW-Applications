#include "logica.h"

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

    config.param_range = NULL;

    for (int i = 0; i < config.d; i++) {
        Range range;

        aux = readFileDescriptor(file);
        range.min = atoi(aux);
        free(aux);

        aux = readFileDescriptor(file);
        range.max = atoi(aux);
        free(aux);

        config.param_range = realloc(config.param_range, (size_t) sizeof(range) * (i + 1));
        config.param_range[i] = range;

    }

    close(file);

    sprintf(msg, "|%d/%d|\n", config.n, config.d);
    debug(msg);

    return config;
}


void createInitialPopulation(Config config, Swarm *swarm, float function(float x, float y)) {

    swarm->particles = malloc((size_t) sizeof(Particle) * config.n);

    for (int i = 0; i < config.n; i++) {
        Particle p;

        p.params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            float r = (float) rand();

            p.params[j] = (config.param_range[j].max - config.param_range[j].min) * r / RAND_MAX + config.param_range[j].min;
        }

        //printf("%f,%f\n", p.params[0], p.params[1]);

        p.velocity = malloc((size_t) sizeof(float) * config.d);
        p.best_params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            p.velocity[j] = ((float) rand()) / RAND_MAX;   // entre 0 i 1 (de moment)
            p.best_params[j] = p.params[j];
        }

        p.best_fit = function(p.best_params[0], p.best_params[1]);
        swarm->particles[i] = p;
    }

    swarm->best_fit = swarm->particles[0].best_fit;
    swarm->best_params = malloc((size_t) sizeof(float) * config.d);
    swarm->iterations = 0;
}


void getFitValues(Config c, Swarm *swarm, float function(float x, float y)) {

    for (int i=0; i<c.n; i++) {
        float fit = 0;

        fit = function(swarm->particles[i].params[0], swarm->particles[i].params[1]);

        if (fit < swarm->particles[i].best_fit) {
            swarm->particles[i].best_fit = fit;
            for (int j = 0; j < c.d; j++) {
                swarm->particles[i].best_params[j] = swarm->particles[i].params[j];
            }
        }

        if (fit < swarm->best_fit) {
            swarm->best_fit = fit;
            for (int j = 0; j < c.d; j++) {
                swarm->best_params[j] = swarm->particles[i].params[j];
            }
        }
    }
}


void updateVelocity(Config c, Swarm *swarm) {

    for (int i=0; i<c.n; i++) {
        float cognitive;
        float social;

        for (int j = 0; j < c.d; j++) {
            cognitive = 2 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            swarm->particles[i].velocity[j] += cognitive + social;
        }
    }
}


void updateParameters(Config c, Swarm *swarm) {

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            swarm->particles[i].params[j] += swarm->particles[i].velocity[j];
        }
    }
}