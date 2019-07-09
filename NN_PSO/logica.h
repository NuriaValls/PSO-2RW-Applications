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
//#include "neural_net.h"

#define V_FIXED_WEIGHT_C 1.49445
#define V_FIXED_WEIGHT_VI 0.729

typedef struct {
    int neurons;
    char activ;
} Layer;

typedef struct {
    int max;
    int min;
} Range;

typedef struct {
    int n;
    int d;
    Range *param_range;
    float vmax;
    int l;
    Layer *layers;
} Config;

typedef struct {
    float **matrix;
    int cols;
    int rows;
} Matrix;

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
    float best_train_acc;
    float best_val_acc;
} Swarm;

Config readConfigFile(char *filename);

Matrix readDataFile(char *filename, int nRows, int nCols);

void createInitialPopulation(Config config, Swarm *swarm, Matrix data, Matrix y);

void getFitValues(Config c, Swarm *swarm, Matrix data, Matrix y);

void updateVelocity(Config c, Swarm *swarm);

void updateVelocity_vMax(Config c, Swarm *swarm);

void updateVelocity_fixedWeights(Config c, Swarm *swarm);

void updateVelocity_decreasingInertia(Config c, Swarm *swarm, int max_t);

void select_updateVelocity(int select, Config c, Swarm *swarm, int max_t);

void updateParameters(Config c, Swarm *swarm);

Matrix MAT_create(int rows, int cols);

Matrix X_train(Matrix data);

Matrix read_y(Matrix data);

float relu(float value);

float sigmoid(float value);

Matrix matrix_multiplication(Matrix arg1, Matrix arg2, char act_function);

float mse_loss(Matrix actual_y, Matrix yhat);

float accuracy(Matrix actual_y, Matrix yhat);

int count_parameters(Config config);

//Matrix forward_pass(Matrix data, float weights[3]);
Matrix forward_pass(Matrix data, float* weights, Config config);

//float fit_value(Matrix data, Matrix y, float weights[3], Swarm *swarm);
float fit_value(Matrix data, Matrix y, float* weights, Swarm *swarm, Config config);

#endif
