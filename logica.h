#ifndef _LOGICA_H_
#define _LOGICA_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#include "utils.h"

#define D       2   //number of parameters to optimize
#define N       10  //population size

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
} Swarm;

typedef struct {
    int max;
    int min;
} Range;

typedef struct {
    int n;
    int d;
    Range *param_range;
    float vmax;
} Config;


Config readConfigFile(char *filename);

void createInitialPopulation(Config config, Swarm *swarm, float function(float x, float y));

void getFitValues(Config c, Swarm *swarm, float function(float x ,float y));

void updateVelocity(Config c, Swarm *swarm, float function(float x ,float y));

void updateVelocity_vMax(Config c, Swarm *swarm, float function(float x, float y));

void updateVelocity_fixedWeights(Config c, Swarm *swarm, float function(float x, float y));

void updateVelocity_decreasingInertia(Config c, Swarm *swarm, float function(float x, float y), int max_t);

void select_updateVelocity(int select, Config c, Swarm *swarm, float function(float x, float y), int max_t);

void updateParameters(Config c, Swarm *swarm);

#endif
