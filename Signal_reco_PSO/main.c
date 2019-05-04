#include <stdlib.h>

#include "logica.h"


int select_velocity_method = 3;
int convergence_factor = 10000;


char checkProgramArguments(int argc) {
    return argc == 2;
}

float complex original_function(float t) {

    /* Example function. p.54 of the signal paper. */

    float complex aux;
    float real;
    float imag;

    real =  0.5 * cos(2 * M_PI * 0.1 * t) - cos(2 * M_PI * 0.3 * t) + 0.4 * cos(2 * M_PI * 0.4 * t);
    imag = (0.5 * sin(2 * M_PI * 0.1 * t) - sin(2 * M_PI * 0.3 * t) + 0.4 * sin(2 * M_PI * 0.4 * t)) * I;

    aux = real + imag;

    return aux;
}



float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2) {

    /* Example function. p.54 of the signal paper. */

    float complex aux;
    float real;
    float imag;

    real =  a0 * cos(2 * M_PI * w0 * t) + a1 * cos(2 * M_PI * w1 * t) + a2 * cos(2 * M_PI * w2 * t);
    imag = (a0 * sin(2 * M_PI * w0 * t) + a1 * sin(2 * M_PI * w1 * t) + a2 * sin(2 * M_PI * w2 * t)) * I;

    aux = real + imag;

    return aux;
}

float * solutionInput(Config c) {

    fftw_complex *in, *out;
    fftw_plan p;
    float * modulus;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * c.FFTlength);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * c.FFTlength);

    p = fftw_plan_dft_1d(c.FFTlength, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    for (int idx=0; idx<c.FFTlength; idx++) {
        float f;

        f = function(idx, 0.5, 0.1, -1, 0.3, 0.4, 0.4);

        in[idx][0] = creal(f);
        in[idx][1] = cimag(f);
    }

    fftw_execute(p);

    modulus = FFTmodulus(out, c.FFTlength);

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return modulus;
}

int main(int argc, char **argv) {

    Config config;
    Swarm swarm;
    char msg[LENGTH];
    int iter = 100000000;
    int count_convergence = 0;
    double best_fit_local = 0.0;
    int not_converged = 1;
    double best_fit_compare;
    int N = 32;
    float * modulus;
    float aux;

    srand(time(NULL));

    if (!checkProgramArguments(argc)) {
        print(MSG_WRONG_ARGS);
        exit(EXIT_FAILURE);
    }

    config = readConfigFile(argv[1]);

    modulus = solutionInput(config);

    createInitialPopulation(config, &swarm, function, modulus);

    for (int i = 0; i<config.FFTlength; i++) {
        sprintf(msg, "%f, ", modulus[i]);
        debug(msg);
    }

    while (not_converged) {
//    while (iter > 0) {

        getFitValues(config, &swarm, function, modulus, 0);

        sprintf(msg, "I: %d, Best fit value: %f, Params: a0=%f a1=%f a2=%f w0=%f w1=%f w2=%f\n", swarm.iterations, swarm.best_fit,
                swarm.best_params[0], swarm.best_params[2], swarm.best_params[4], swarm.best_params[1], swarm.best_params[3], swarm.best_params[5]);
        //debug(msg);

        best_fit_compare = trunc(swarm.best_fit * 100000000);

        if (swarm.iterations == 0) {
            best_fit_local = best_fit_compare;
        }

        select_updateVelocity(select_velocity_method, config, &swarm, function, iter);

        updateParameters(config, &swarm);

        if (best_fit_local == best_fit_compare) {
            count_convergence++;
        } else {
            count_convergence = 0;
            sprintf(msg, "\t%f, %f\n",best_fit_local, best_fit_compare);
            //debug(msg);
            best_fit_local = best_fit_compare;
        }

        if (count_convergence == convergence_factor) {
            not_converged = 0;
            sprintf(msg, "\n\nI: %d, Best fit value: %f, Params: a0=%f a1=%f a2=%f w0=%f w1=%f w2=%f\n\n", swarm.iterations, swarm.best_fit,
                    swarm.best_params[0], swarm.best_params[2], swarm.best_params[4], swarm.best_params[1], swarm.best_params[3], swarm.best_params[5]);
            debug(msg);
        }

        iter--;
        swarm.iterations++;
    }


    aux = fitFunction(function, modulus, config, swarm.particles[swarm.best_particle_idx], 0, 1);




    return EXIT_SUCCESS;

}

