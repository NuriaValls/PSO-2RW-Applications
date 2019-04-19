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
        range.min = atof(aux);
        free(aux);

        aux = readFileDescriptor(file);
        range.max = atof(aux);
        free(aux);

        config.param_range = realloc(config.param_range, (size_t) sizeof(range) * (i + 1));
        config.param_range[i] = range;

    }

    aux = readFileDescriptor(file);
    config.vmax = atof(aux);
    free(aux);

    aux = readFileDescriptor(file);
    config.FFTlength = atoi(aux);
    free(aux);

    close(file);

    sprintf(msg, "|%d/%d/%f/%d|\n", config.n, config.d, config.vmax, config.FFTlength);
    debug(msg);

    return config;
}


void createInitialPopulation(Config config, Swarm *swarm,
                             float complex function(float t, float a0, float w0, float a1, float w1, float a2,
                                                    float w2), float *original_modulus) {

    swarm->particles = malloc((size_t) sizeof(Particle) * config.n);

    for (int i = 0; i < config.n; i++) {
        Particle p;

        p.params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            float r = (float) rand();

            p.params[j] =
                    (config.param_range[j].max - config.param_range[j].min) * r / RAND_MAX + config.param_range[j].min;
        }

        //printf("%f,%f\n", p.params[0], p.params[1]);

        p.velocity = malloc((size_t) sizeof(float) * config.d);
        p.best_params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            //p.velocity[j] = ((float) rand()) / RAND_MAX;   // entre 0 i 1 (de moment)
            p.velocity[j] = (float) 0;
            p.best_params[j] = p.params[j];
        }

        p.best_fit = fitFunction(function, original_modulus, config, p, 1);
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


void getFitValues(Config c, Swarm *swarm,
                  float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2),
                  float *original_modulus) {

    for (int i = 0; i < c.n; i++) {
        float fit = 0;

        fit = fitFunction(function, original_modulus, c, swarm->particles[i], 0);

        //printf("%f, %f, %f, %f --> %f\n", swarm->particles[i].params[0], swarm->particles[i].params[1], swarm->particles[i].params[2], swarm->particles[i].params[3], fit);

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


void updateVelocity(Config c, Swarm *swarm,
                    float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2)) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

//    float w;

    for (int i = 0; i < c.n; i++) {
        for (int j = 0; j < c.d; j++) {

//            w = (float) (1.1 - (swarm->particles[i].best_params[j] / swarm->best_params[j]));

            cognitive = 2 * ((float) rand()) / RAND_MAX *
                        (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            /*sprintf(msg, "P: %d :  %f, %f, %f - %f, %f --> %f\n", i, swarm->particles[i].velocity[j], cognitive, social, swarm->particles[i].params[0], swarm->particles[i].params[1],
                    function(swarm->particles[i].params[0], swarm->particles[i].params[1]));
            sprintf(msg, "%f\n",w);
            debug(msg);*/

            velocity = swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


void updateVelocity_vMax(Config c, Swarm *swarm,
                         float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2)) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i = 0; i < c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 2 * ((float) rand()) / RAND_MAX *
                        (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            /*sprintf(msg, "P: %d :  %f, %f, %f - %f, %f --> %f\n", i, swarm->particles[i].velocity[j], cognitive, social, swarm->particles[i].params[0], swarm->particles[i].params[1],
                    function(swarm->particles[i].params[0], swarm->particles[i].params[1]));
            sprintf(msg, "%f\n",w);
            debug(msg);*/

            velocity = swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity > swarm->vmax[j] ? swarm->vmax[j] : velocity;
        }
    }
}


void updateVelocity_fixedWeights(Config c, Swarm *swarm,
                                 float complex function(float t, float a0, float w0, float a1, float w1, float a2,
                                                        float w2)) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i = 0; i < c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 2 * ((float) rand()) / RAND_MAX *
                        (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            /*sprintf(msg, "P: %d :  %f, %f, %f - %f, %f --> %f\n", i, swarm->particles[i].velocity[j], cognitive, social, swarm->particles[i].params[0], swarm->particles[i].params[1],
                    function(swarm->particles[i].params[0], swarm->particles[i].params[1]));
            sprintf(msg, "%f\n",w);
            debug(msg);*/

            velocity = (float) (V_FIXED_WEIGHT_VI * swarm->particles[i].velocity[j] + V_FIXED_WEIGHT_C * cognitive +
                                V_FIXED_WEIGHT_C * social);
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


void updateVelocity_decreasingInertia(Config c, Swarm *swarm,
                                      float complex function(float t, float a0, float w0, float a1, float w1, float a2,
                                                             float w2), int max_t) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;
    float w;

    for (int i = 0; i < c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            w = (float) ((0.9 - 0.4) * (max_t - swarm->iterations) / (max_t + 0.4));

            cognitive = 2 * ((float) rand()) / RAND_MAX *
                        (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            sprintf(msg, "P: %d :  %f, %f, %f - %f --> %f \n", i, swarm->particles[i].velocity[j], cognitive, social,
                    swarm->particles[i].params[j],
                    swarm->particles[i].best_fit);
            //sprintf(msg, "%f\n",w);
            //debug(msg);

            velocity = w * swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}


void select_updateVelocity(int select, Config c, Swarm *swarm,
                           float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2),
                           int max_t) {

    switch (select) {
        case 0:
            updateVelocity(c, swarm, function);
            break;
        case 1:
            updateVelocity_vMax(c, swarm, function);
            break;
        case 2:
            updateVelocity_fixedWeights(c, swarm, function);
            break;
        case 3:
            updateVelocity_decreasingInertia(c, swarm, function, max_t);

        default:
            break;
    }
}


void updateParameters(Config c, Swarm *swarm) {

    float aux = 0;

    for (int i = 0; i < c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            aux = swarm->particles[i].params[j] + swarm->particles[i].velocity[j];

            if (aux >= c.param_range[j].min && aux <= c.param_range[j].max) {
                swarm->particles[i].params[j] = aux;
            }
        }
    }
}


void sampleFunction(fftw_complex *in, fftw_complex *out, Config c,
                    float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2),
                    Particle particle, int best) {

    fftw_plan p;

    p = fftw_plan_dft_1d(c.FFTlength, in, out, FFTW_FORWARD, FFTW_MEASURE);


    for (int idx = 0; idx < c.FFTlength; idx++) {
        float f;

        if (best == 1) {
            f = function(idx, particle.best_params[0], particle.best_params[1], particle.best_params[2],
                         particle.best_params[3], particle.best_params[4], particle.best_params[5]);
        } else {
            f = function(idx, particle.params[0], particle.params[1], particle.params[2], particle.params[3],
                         particle.params[4], particle.params[5]);
        }

        in[idx][0] = creal(f);
        in[idx][1] = cimag(f);
    }

    fftw_execute(p);

    fftw_destroy_plan(p);

}


float *FFTmodulus(fftw_complex *out, int N) {
    float *modulus;

    modulus = malloc(sizeof(float) * N);

    for (int t = 0; t < N; t++) {
        modulus[t] = sqrt((out[t][0] * out[t][0]) + (out[t][1] * out[t][1])) / (N / 2);
    }

    return modulus;
}


float fitFunction(float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2),
                  float *original_modulus, Config c, Particle particle, int best) {

    float difference = 0;
    fftw_complex *in, *out;
    float *modulus;

    in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * c.FFTlength);
    out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * c.FFTlength);

    sampleFunction(in, out, c, function, particle, best);

    modulus = FFTmodulus(out, c.FFTlength);

    fftw_free(in);
    fftw_free(out);


    for (int t = 0; t < c.FFTlength; t++) {
        difference += fabsf(modulus[t] - original_modulus[t]) * 1;
    }

//    printf("%f %f %f %f %f\n", modulus[0], modulus[1], modulus[2], modulus[3], modulus[4]);

//    printf("%f, %f, %f, %f --> %f#\n", particle.params[0], particle.params[1], particle.params[2], particle.params[3],
//           difference);

    return difference;
}