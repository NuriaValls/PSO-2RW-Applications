#ifndef _LOGICA_H_
#define _LOGICA_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <fftw3.h>
#include <complex.h>

#include "utils.h"

#define V_FIXED_WEIGHT_C 1.49445
#define V_FIXED_WEIGHT_VI 0.729

typedef struct {
    float *params;          // paràmetres actuals
    float *velocity;        // velocitat actual
    float *best_params;     // millors paràmetres individuals
    float best_fit;
} Particle;

typedef struct {
    Particle *particles;
    float *best_params;
    float best_fit;
    int iterations;
    float *vmax;
    int best_particle_idx;
} Swarm;

typedef struct {
    float max;
    float min;
} Range;

typedef struct {
    int n;
    int d;
    Range *param_range;
    float vmax;
    int FFTlength;
} Config;


Config readConfigFile(char *filename);

void createInitialPopulation(Config config, Swarm *swarm, float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2), float * original_modulus, int resize);

void getFitValues(Config c, Swarm *swarm, float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2), float * original_modulus, int print, int resize);

void updateVelocity(Config c, Swarm *swarm);

void updateVelocity_vMax(Config c, Swarm *swarm);

void updateVelocity_fixedWeights(Config c, Swarm *swarm);

void updateVelocity_decreasingInertia(Config c, Swarm *swarm, int max_t);

void select_updateVelocity(int select, Config c, Swarm *swarm, int max_t);

void updateParameters(Config c, Swarm *swarm);

void sampleFunction(fftw_complex* in, fftw_complex* out, Config c, float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2), Particle particle, int best, int resize);

float * FFTmodulus(fftw_complex* out, int N);

float fitFunction(float complex function(float t, float a0, float w0, float a1, float w1, float a2, float w2), float * original_modulus, Config c, Particle particle, int best, int print, int resize);


#endif
