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

    aux = readFileDescriptor(file);
    config.vmax = atof(aux);
    free(aux);

    close(file);

    sprintf(msg, "|%d/%d/%f|\n", config.n, config.d, config.vmax);
    debug(msg);

    return config;
}


Matrix readDataFile(char *filename, int nRows, int nCols) {
    int file;
    char msg[LENGTH], *aux;
    Matrix mtx;

    file = open(filename, O_RDONLY);
    if (file <= 0) {
        sprintf(msg, MSG_FILE_ERR, filename);
        print(msg);
        exit(EXIT_FAILURE);
    }

    mtx = MAT_create(nRows, nCols);

    for (int i = 0; i<nRows; i++) {
        aux = readFileDescriptor(file);
        mtx.matrix[i][0] = atof(aux);
        free(aux);

        aux = readFileDescriptor(file);
        mtx.matrix[i][1] = atof(aux);
        free(aux);

        aux = readFileDescriptor(file);
        mtx.matrix[i][2] = atof(aux);
        free(aux);
    }

    close(file);

    return mtx;
}


void createInitialPopulation(Config config, Swarm *swarm, Matrix data, Matrix y) {

    swarm->particles = malloc((size_t) sizeof(Particle) * config.n);
    char msg[LENGTH];

    for (int i = 0; i < config.n; i++) {
        Particle p;

        p.params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            float r = (float) rand();

            p.params[j] = (config.param_range[j].max - config.param_range[j].min) * r / RAND_MAX + config.param_range[j].min;
        }

        //sprintf("%f,%f\n", p.params[0], p.params[1]);

        p.velocity = malloc((size_t) sizeof(float) * config.d);
        p.best_params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            //p.velocity[j] = ((float) rand()) / RAND_MAX;   // entre 0 i 1 (de moment)
            p.velocity[j] = (float) 0;
            p.best_params[j] = p.params[j];
        }

        //p.best_fit = function(p.best_params[0], p.best_params[1]);
        char msg[300];
        float weights[9] = {p.best_params[0], p.best_params[1], p.best_params[2], p.best_params[3], p.best_params[4], p.best_params[5], p.best_params[6], p.best_params[7], p.best_params[8]};
        //float weights[18] = {p.best_params[0], p.best_params[1], p.best_params[2], p.best_params[3], p.best_params[4], p.best_params[5], p.best_params[6], p.best_params[7], p.best_params[8],p.best_params[9],p.best_params[10],p.best_params[11],p.best_params[12],p.best_params[13],p.best_params[14],p.best_params[15],p.best_params[16],p.best_params[17]};
        //float weights[3] = {p.best_params[0], p.best_params[1], p.best_params[2]};//, p.best_params[3], p.best_params[4], p.best_params[5]};

        //sprintf(msg, "%f\n", p.best_params[0]);
        //debug(msg);

        p.best_fit = fit_value(data, y, weights, swarm);
        swarm->particles[i] = p;
    }

    swarm->best_fit = swarm->particles[0].best_fit;
    swarm->best_params = malloc((size_t) sizeof(float) * config.d);
    swarm->iterations = 0;
    swarm->vmax = malloc((size_t) sizeof(float) * config.d);
    swarm->best_val_acc = 0;

    for (int j = 0; j < config.d; j++) {
        swarm->vmax[j] = config.vmax * (config.param_range[j].max - config.param_range[j].min);
    }

}


void getFitValues(Config c, Swarm *swarm, Matrix data, Matrix y) {

    for (int i=0; i<c.n; i++) {
        float fit = 0;
        float *acc;
        //float m1[2][3];
        //float m2[3][1];

        //float weights[3] = {swarm->particles[i].params[0], swarm->particles[i].params[1], swarm->particles[i].params[2]};//, swarm->particles[i].params[3], swarm->particles[i].params[4], swarm->particles[i].params[5]};
        float weights[9] = {swarm->particles[i].params[0], swarm->particles[i].params[1], swarm->particles[i].params[2], swarm->particles[i].params[3], swarm->particles[i].params[4], swarm->particles[i].params[5], swarm->particles[i].params[6], swarm->particles[i].params[7], swarm->particles[i].params[8]};
        //float weights[18] = {swarm->particles[i].params[0], swarm->particles[i].params[1], swarm->particles[i].params[2], swarm->particles[i].params[3], swarm->particles[i].params[4], swarm->particles[i].params[5], swarm->particles[i].params[6], swarm->particles[i].params[7], swarm->particles[i].params[8],swarm->particles[i].params[9],swarm->particles[i].params[10],swarm->particles[i].params[11],swarm->particles[i].params[12],swarm->particles[i].params[13],swarm->particles[i].params[14],swarm->particles[i].params[15],swarm->particles[i].params[16],swarm->particles[i].params[17]};

        fit = fit_value(data, y, weights, swarm);

        //char msg[100];
        //sprintf(msg, "%f\t", fit);
        //debug(msg);

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

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

//    float w;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

//            w = (float) (1.1 - (swarm->particles[i].best_params[j] / swarm->best_params[j]));

            cognitive = 1.6 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 1.7 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            /*sprintf(msg, "P: %d :  %f, %f, %f - %f, %f --> %f\n", i, swarm->particles[i].velocity[j], cognitive, social, swarm->particles[i].params[0], swarm->particles[i].params[1],
                    function(swarm->particles[i].params[0], swarm->particles[i].params[1]));
            sprintf(msg, "%f\n",w);
            debug(msg);*/

            velocity = swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


void updateVelocity_vMax(Config c, Swarm *swarm) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 1.6 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 1.7 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            /*sprintf(msg, "P: %d :  %f, %f, %f - %f, %f --> %f\n", i, swarm->particles[i].velocity[j], cognitive, social, swarm->particles[i].params[0], swarm->particles[i].params[1],
                    function(swarm->particles[i].params[0], swarm->particles[i].params[1]));
            sprintf(msg, "%f\n",w);
            debug(msg);*/

            velocity = swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity > swarm->vmax[j] ? swarm->vmax[j] : velocity;
        }
    }
}


void updateVelocity_fixedWeights(Config c, Swarm *swarm) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 2 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            /*sprintf(msg, "P: %d :  %f, %f, %f - %f, %f --> %f\n", i, swarm->particles[i].velocity[j], cognitive, social, swarm->particles[i].params[0], swarm->particles[i].params[1],
                    function(swarm->particles[i].params[0], swarm->particles[i].params[1]));
            sprintf(msg, "%f\n",w);
            debug(msg);*/

            velocity = (float) (V_FIXED_WEIGHT_VI * swarm->particles[i].velocity[j] + V_FIXED_WEIGHT_C * cognitive + V_FIXED_WEIGHT_C * social);
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


void updateVelocity_decreasingInertia(Config c, Swarm *swarm, int max_t) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;
    float w;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            //w = (float) ((0.9 - 0.4) * (max_t - swarm->iterations) / (max_t + 0.4));
            w = (float) (0.4 + tanh(swarm->iterations*(0.5/max_t)));

            cognitive = 1.6 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 1.7 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            /*sprintf(msg, "P: %d :  %f, %f, %f - %f, %f --> %f\n", i, swarm->particles[i].velocity[j], cognitive, social, swarm->particles[i].params[0], swarm->particles[i].params[1],
                    function(swarm->particles[i].params[0], swarm->particles[i].params[1]));
            sprintf(msg, "%f\n",w);
            debug(msg);*/

            velocity = w * swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


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