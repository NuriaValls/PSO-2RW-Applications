#ifndef _LOGICA_H_
#define _LOGICA_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <signal.h>

#include "utils.h"

#define D       2   //number of parameters to optimize
#define N       10  //population size

typedef struct {
    float *params;
    float velocity;
    float *fit_values;
    float best_individual;
    float best_global;
} Particle;

typedef struct {
    Particle *particles;
    float solution;
    int iterations;
} Swarm;

typedef struct {
    int n;
    int d;
} Config;


char checkProgramArguments(int argc);

Config readConfigFile(char *filename);

#endif
