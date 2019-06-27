#include <stdlib.h>

#include "logica.h"


int select_velocity_method = 3;
int convergence_factor = 10000;


/**
 * Checks that the program arguments are the desired.
 *
 * @param argc  Argument counter.
 * @return      Flag ok or ko.
 */
char checkProgramArguments(int argc) {
    return argc == 3;
}


/**
 * Initial function that the algorithm has to find.
 *
 * @param t     Time
 * @return      Value of the function at time t.
 */
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


/**
 * Function to find without parameters.
 *
 * @param t     Time.
 * @param a0    Amplotude of the first peak.
 * @param w0    Frequency of the first peak.
 * @param a1    Amplitude of the second peak.
 * @param w1    Frequency of the second peak.
 * @param a2    Amplitude if the third peak.
 * @param w2    Frequency of the third peak.
 * @return      Value of the function at time t.
 */
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


/**
 * Evaluates the DFT of the objective function given the parameters.
 *
 * @param c     Configuration infromation.
 * @param a0    Amplotude of the first peak.
 * @param w0    Frequency of the first peak.
 * @param a1    Amplitude of the second peak.
 * @param w1    Frequency of the second peak.
 * @param a2    Amplitude if the third peak.
 * @param w2    Frequency of the third peak.
 * @return
 */
float * solutionInput(Config c, float a0, float w0, float a1, float w1, float a2, float w2) {

    fftw_complex *in, *out;
    fftw_plan p;
    float * modulus;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * c.FFTlength);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * c.FFTlength);

    p = fftw_plan_dft_1d(c.FFTlength, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    for (int idx=0; idx<c.FFTlength; idx++) {
        float f;

        f = function(idx, a0, w0, a1, w1, a2, w2);

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


/**
 * Executes iterations of the swarm until the particles converge.
 *
 * @param config    Configuration infromation.
 * @param swarm     Contians all the information of the swarm and the particles.
 * @param modulus   Values of the modulus of the DFT of the input signal.
 * @param resize    Parameter to indicate the size of the parameters of the particles.
 */
void convergenceLoop (Config config, Swarm *swarm, float *modulus, int resize) {

    int not_converged = 1;
    char msg[LENGTH];
    double best_fit_compare;
    double best_fit_local = 0.0;
    int iter = 10000000;
    int count_convergence = 0;

    while (not_converged) {

        getFitValues(config, swarm, function, modulus, 0, resize);

        sprintf(msg, "I: %d, Best fit value: %f, Params: a0=%f a1=%f a2=%f w0=%f w1=%f w2=%f\n", swarm->iterations, swarm->best_fit,
                swarm->best_params[0], swarm->best_params[2], swarm->best_params[4], swarm->best_params[1], swarm->best_params[3], swarm->best_params[5]);
        //debug(msg);

        best_fit_compare = trunc(swarm->best_fit * 100000000);

        if (swarm->iterations == 0) {
            best_fit_local = best_fit_compare;
        }

        select_updateVelocity(select_velocity_method, config, swarm, function, iter);

        updateParameters(config, swarm);

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
            if (resize == 0) {
                sprintf(msg, "\n\nI: %d, Best fit value: %f, Params: a0=%f a1=%f a2=%f w0=%f w1=%f w2=%f\n\n", swarm->iterations, swarm->best_fit,
                        swarm->best_params[0], swarm->best_params[2], swarm->best_params[4], swarm->best_params[1], swarm->best_params[3], swarm->best_params[5]);
                debug(msg);
            } else {
                sprintf(msg, "\n\nI: %d, Best fit value: %f, Params: a0=%f w0=%f\n\n", swarm->iterations, swarm->best_fit,
                        swarm->best_params[0], swarm->best_params[1]);
                debug(msg);
            }

        }

        iter--;
        swarm->iterations++;
    }
}


/**
 * Orders the input array upwards and fills the index in order.
 *
 * @param myFloatArr    Values to order.
 * @param myFloatIndex  Indexes of the values.
 */
void orderByValue(float myFloatArr[3], int myFloatIndex[3]) {
    int len = 3;
    int switched = 0;

    do
    {
        switched = 0;
        for(int i = 1; i < len; i++)
        {
            if(myFloatArr[myFloatIndex[i - 1]] < myFloatArr[myFloatIndex[i]])
            {
                int temp = myFloatIndex[i];
                myFloatIndex[i] = myFloatIndex[i - 1];
                myFloatIndex[i - 1] = temp;
                switched = 1;
            }
        }
    }
    while(switched);
}


/**
 * Main function
 *
 * @param argc  Argument counter.
 * @param argv  Argument value.
 * @return      Process statul.
 */
int main(int argc, char **argv) {

    Config config;
    Swarm swarm;
    char msg[LENGTH];
    float * modulus;
    float aux;
    Swarm swarm4;
    Config config_2;

    srand(time(NULL));

    if (!checkProgramArguments(argc)) {
        print(MSG_WRONG_ARGS);
        exit(EXIT_FAILURE);
    }

    config = readConfigFile(argv[1]);

    modulus = solutionInput(config, 0.5, 0.1, -1, 0.3, 0.4, 0.4);

    createInitialPopulation(config, &swarm, function, modulus, 0);

    for (int i = 0; i<config.FFTlength; i++) {
        sprintf(msg, "%f, ", modulus[i]);
        //debug(msg);
    }

    convergenceLoop(config, &swarm, modulus, 1);

    // *************************************************

    float *modulus1;
    float *subs_modulus1 = malloc(sizeof(float) * (config.FFTlength));

    modulus1 = solutionInput(config, swarm.best_params[0], swarm.best_params[1], 0, 0, 0, 0);

    sprintf(msg, "\n\n");
    debug(msg);

    for (int i = 0; i<config.FFTlength; i++) {
        subs_modulus1[i] = fabs(fabs(modulus[i]) - fabs(modulus1[i]));
        sprintf(msg, "%f, ", modulus1[i]);
        debug(msg);
    }

    // Busquem el segon pic
    Swarm swarm2;

    createInitialPopulation(config, &swarm2, function, subs_modulus1, 1);

    convergenceLoop(config, &swarm2, subs_modulus1, 1);

    // *************************************************

    float *modulus2;
    float *subs_modulus2 = malloc(sizeof(float) * (config.FFTlength));

    sprintf(msg, "\n\n");
    debug(msg);

    modulus2 = solutionInput(config, swarm2.best_params[0], swarm2.best_params[1], 0, 0, 0, 0);

    for (int i = 0; i<config.FFTlength; i++) {
        subs_modulus2[i] = fabs(fabs(subs_modulus1[i]) - fabs(modulus2[i]));
        sprintf(msg, "%f, ", modulus2[i]);
        debug(msg);
    }

    sprintf(msg, "\n\n");
    debug(msg);

    // Busquem el tercer pic
    Swarm swarm3;

    createInitialPopulation(config, &swarm3, function, subs_modulus2, 1);

    convergenceLoop(config, &swarm3, subs_modulus2, 1);

    Particle p;
    p.params = malloc((size_t) sizeof(float) * 6);
    p.params[0] = swarm.best_params[0];
    p.params[1] = swarm.best_params[1];
    p.params[2] = swarm2.best_params[0];
    p.params[3] = swarm2.best_params[1];
    p.params[4] = swarm3.best_params[0];
    p.params[5] = swarm3.best_params[1];

    float final_fit = fitFunction(function, modulus, config, p, 0, 0, 0);

    sprintf(msg, "\n\n");
    //debug(msg);

    float *modulus3;
    float *subs_modulus3 = malloc(sizeof(float) * (config.FFTlength));

    modulus3 = solutionInput(config, swarm3.best_params[0], swarm3.best_params[1], 0, 0, 0, 0);

    for (int i = 0; i<config.FFTlength; i++) {
        subs_modulus3[i] = fabs(fabs(subs_modulus2[i]) - fabs(modulus3[i]));
        sprintf(msg, "%f, ", modulus3[i]);
        debug(msg);
    }

    sprintf(msg, "\n\n>>>>> Best fit value: %f, Params: a0=%f a1=%f a2=%f w0=%f w1=%f w2=%f\n\n", swarm3.best_fit,
            swarm.best_params[0], swarm2.best_params[0], swarm3.best_params[0], swarm.best_params[1], swarm2.best_params[1], swarm3.best_params[1]);
    debug(msg);

    //***************************************

    float *modulus_final;
    float *subs_modulus_final = malloc(sizeof(float) * (config.FFTlength));

    modulus_final = solutionInput(config, swarm.best_params[0], swarm.best_params[1], swarm2.best_params[0], swarm2.best_params[1], swarm3.best_params[0], swarm3.best_params[1]);

    sprintf(msg, "\n\n");
    debug(msg);

    for (int i = 0; i<config.FFTlength; i++) {
        subs_modulus_final[i] = fabs(fabs(modulus[i]) - fabs(modulus_final[i]));
        sprintf(msg, "%f, ", modulus_final[i]);
        debug(msg);
    }

    sprintf(msg, "\n\n");
    debug(msg);

    for (int i = 0; i<config.FFTlength; i++) {
        sprintf(msg, "%f, ", subs_modulus_final[i]);
        debug(msg);
    }



    return EXIT_SUCCESS;

}

