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
#include "neural_net.h"

#define V_FIXED_WEIGHT_C 1.49445
#define V_FIXED_WEIGHT_VI 0.729


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

void createInitialPopulation(Config config, Swarm *swarm, Matrix data, Matrix y);

void getFitValues(Config c, Swarm *swarm, Matrix data, Matrix y);

void updateVelocity(Config c, Swarm *swarm);

void updateVelocity_vMax(Config c, Swarm *swarm);

void updateVelocity_fixedWeights(Config c, Swarm *swarm);

void updateVelocity_decreasingInertia(Config c, Swarm *swarm, int max_t);

void select_updateVelocity(int select, Config c, Swarm *swarm, int max_t);

void updateParameters(Config c, Swarm *swarm);

#endif
