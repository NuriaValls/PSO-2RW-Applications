#ifndef _NEURAL_NET_H_
#define _NEURAL_NET_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"


#define N_LAYERS 3

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

Matrix MAT_create(int rows, int cols);

Matrix X_train(Matrix data);

Matrix read_y(Matrix data);
//Matrix y_train(Matrix data);

float relu(float value);

float sigmoid(float value);

Matrix matrix_multiplication(Matrix arg1, Matrix arg2, char act_function);

float mse_loss(Matrix actual_y, Matrix yhat);

float accuracy(Matrix actual_y, Matrix yhat);

//Matrix forward_pass(Matrix data, float weights[3]);
Matrix forward_pass(Matrix data, float weights[9]);

//float fit_value(Matrix data, Matrix y, float weights[3], Swarm *swarm);
float fit_value(Matrix data, Matrix y, float weights[9], Swarm *swarm);


#endif