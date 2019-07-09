#include "logica.h"

/**
 * This function reads the file passed as an argument and creates and fills a Config object with the information in the file.
 *
 * @param filename  Name of the file to read.
 * @return The configuration of the file.
 */
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

    aux = readFileDescriptor(file);
    config.vmax = atof(aux);
    free(aux);

    close(file);

    sprintf(msg, "|%d/%d/%f|\n", config.n, config.d, config.vmax);
    debug(msg);

    return config;
}


/**
 * Initializes all the particles of the swarm with random values, and its respective parameters.
 *
 * @param config    Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 * @param function  Function to optimize.
 */
void createInitialPopulation(Config config, Swarm *swarm, float function(float x, float y)) {

    swarm->particles = malloc((size_t) sizeof(Particle) * config.n);

    for (int i = 0; i < config.n; i++) {
        Particle p;

        p.params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            float r = (float) rand();

            p.params[j] = (config.param_range[j].max - config.param_range[j].min) * r / RAND_MAX + config.param_range[j].min;
        }

        p.velocity = malloc((size_t) sizeof(float) * config.d);
        p.best_params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            p.velocity[j] = (float) 0;
            p.best_params[j] = p.params[j];
        }

        p.best_fit = function(p.best_params[0], p.best_params[1]);
        swarm->particles[i] = p;
    }

    swarm->best_fit = swarm->particles[0].best_fit;
    swarm->best_params = malloc((size_t) sizeof(float) * config.d);
    swarm->iterations = 0;
    swarm->vmax = malloc((size_t) sizeof(float) * config.d);

    for (int j = 0; j < config.d; j++) {
        swarm->vmax[j] = config.vmax * (config.param_range[j].max - config.param_range[j].min);
    }

}


/**
 * Calculates the fit values of all the particles of the swarm.
 *
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 * @param function  Function to optimize.
 */
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


/**
 * Updates the velocity of all the parameters of all the particles of the swarm.
 *
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 */
void updateVelocity(Config c, Swarm *swarm) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 2 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            velocity = swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


/**
 * Updates the velocity of all the parameters of all the particles of the swarm controling the maximum velocity value.
 *
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 */
void updateVelocity_vMax(Config c, Swarm *swarm) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 2 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            velocity = swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity > swarm->vmax[j] ? swarm->vmax[j] : velocity;
        }
    }
}


/**
 * Updates the velocity of all the parameters of all the particles of the swarm using the fixed weights method.
 *
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 */
void updateVelocity_fixedWeights(Config c, Swarm *swarm) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 2 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            velocity = (float) (V_FIXED_WEIGHT_VI * swarm->particles[i].velocity[j] + V_FIXED_WEIGHT_C * cognitive + V_FIXED_WEIGHT_C * social);
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


/**
 * Updates the velocity of all the parameters of all the particles of the swarm using the increasing inertia weights method.
 *
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 * @param max_t     Maximum iterations.
 */
void updateVelocity_decreasingInertia(Config c, Swarm *swarm, int max_t) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;
    float w;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            w = (float) ((0.9 - 0.4) * (max_t - swarm->iterations) / (max_t + 0.4));

            cognitive = 2 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            velocity = w * swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


/**
 * Calls the function to updates vlocities according to a selection.
 *
 * @param select    Selection of the velocity method.
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 * @param max_t     Maximum iterations.
 */
void select_updateVelocity(int select, Config c, Swarm *swarm, int max_t) {

    switch (select) {
        case 0:
            updateVelocity(c, swarm);
            break;
        case 1:
            updateVelocity_vMax(c, swarm);
            break;
        case 2:
            updateVelocity_fixedWeights(c, swarm);
            break;
        case 3:
            updateVelocity_decreasingInertia(c, swarm, max_t);

        default:
            break;
    }
}


/**
 * Updates the the parameters of all the particles of the swarm.
 *
 * @param c         Configuration information.
 * @param swarm     Contains all the information of the swarm and the particles.
 */
void updateParameters(Config c, Swarm *swarm) {

    float aux = 0;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            aux = swarm->particles[i].params[j] + swarm->particles[i].velocity[j];

            if (aux >= c.param_range[j].min && aux <= c.param_range[j].max) {
                swarm->particles[i].params[j] = aux;
            }
        }
    }
}