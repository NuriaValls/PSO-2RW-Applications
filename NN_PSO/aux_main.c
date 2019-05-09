#include <stdlib.h>

#include "logica.h"

#include <math.h>

#define ROWS_DATA 50
#define COLS_DATA 2
#define INIT_WEIGHTS 9
#define ROWS_W1 2
#define COLS_W1 3
#define ROWS_W2 3
#define COLS_W2 1
#define ROWS_DOT1 50
#define COLS_DOT1 3
#define ROWS_DOT2 50
#define COLS_DOT2 1
#define ROWS_Y 50
#define COLS_Y 1


void init_weights(float weights[INIT_WEIGHTS]){
    for (int i = 0; i < INIT_WEIGHTS; i++) {
            float r = (float) rand();
            weights[i] = r / RAND_MAX;
        }
    }

void X_train(float data[ROWS_DATA][COLS_DATA]) {
    for (int i = 0; i < ROWS_DATA; i++) {
        for (int j = 0; j < COLS_DATA; j++) {
            float r = (float) rand();
            data[i][j] = 2 * r / RAND_MAX + -1;
        }
    }
}

void y_train(float data[ROWS_DATA][COLS_DATA], float y[ROWS_Y]){
    for (int i = 0; i < ROWS_DATA; i++) {
        if (data[i][1]>=0){
            y[i] = 1;
        }
        else {
            y[i] = 0;
        }
    }
}

float relu(float value){
    if (value>=0){
        return value;
    }
    else {
        return 0;
    }
}

float sigmoid(float value){
    return 1/(1+exp(-value));
}

void arrange_weights(float individual[INIT_WEIGHTS], float matrix1[ROWS_W1][COLS_W1], float matrix2[ROWS_W2][COLS_W2]){
    matrix1[0][0] = individual[0];
    matrix1[1][0] = individual[1];
    matrix1[0][1] = individual[2];
    matrix1[1][1] = individual[3];
    matrix1[0][2] = individual[4];
    matrix1[1][2] = individual[5];

    matrix2[0][0] = individual[6];
    matrix2[1][0] = individual[7];
    matrix2[2][0] = individual[8];
}

void matrix_multiplication1(float first[ROWS_DATA][COLS_DATA], float second[ROWS_W1][COLS_W1], float multiply[ROWS_DOT1][COLS_DOT1]){
    float sum = 0;
    for (int c = 0; c < ROWS_DATA; c++) {
        for (int d = 0; d < COLS_DOT1; d++) {
            for (int k = 0; k < COLS_DATA; k++) {
                sum = sum + first[c][k]*second[k][d];
            }
            //sprintf(msg, "%f\n", sum);
            //debug(msg);
            multiply[c][d] = sum;
            sum = 0;
        }
    }
}

void matrix_multiplication2(float first2[ROWS_DOT1][COLS_DOT1], float second2[ROWS_W2][COLS_W2], float multiply2[ROWS_DOT2][COLS_DOT2]){
    float sum = 0;
    for (int c = 0; c < ROWS_DOT1; c++) {
        for (int d = 0; d < COLS_DOT2; d++) {
            for (int k = 0; k < COLS_DOT1; k++) {
                sum = sum + first2[c][k]*second2[k][d];
            }

            multiply2[c][d] = sigmoid(sum);
            sum = 0;
        }
    }
}

float mse_loss(float yhat[ROWS_Y][COLS_Y], float actual_y[ROWS_Y]){
    float sum = 0;
    for (int i=0;i<ROWS_Y;i++){
        sum = sum + pow((yhat[i][0]-actual_y[i]),2);
    }
    return sum/ROWS_DATA;
}

float accuracy(float yhat[ROWS_Y][COLS_Y], float actual_y[ROWS_Y], float yhat_binary[ROWS_Y]) {
    float sum = 0;
    for (int i=0;i<ROWS_Y;i++){
        if (yhat[i][0]>=0.5){
            yhat_binary[i] = 1;
            }
        }
    for (int i=0;i<ROWS_Y;i++){
        if (yhat_binary[i] == actual_y[i]){
            sum += 1;
        }
    }
    return sum/ROWS_Y;
}

void fit_value(float multiply2[ROWS_DOT2][COLS_DOT2]){//float weights[9], float matrix1[2][3],float matrix2[3][1],float data[50][2],float y[50],float multiply[50][3],float multiply2[50][1]){
    float weights[INIT_WEIGHTS];
    float matrix1[ROWS_W1][COLS_W1];
    float matrix2[ROWS_W2][COLS_W2];
    float data[ROWS_DATA][COLS_DATA];
    float y[ROWS_Y];
    float yhat_binary[ROWS_Y];
    float multiply[ROWS_DOT1][COLS_DOT1];
    float loss;
    float acc;
    char msg[300];

    debug(msg);
    X_train(data);
    y_train(data, y);
    init_weights(weights);
    arrange_weights(weights, matrix1, matrix2);
    matrix_multiplication1(data, matrix1, multiply);
    matrix_multiplication2(multiply, matrix2, multiply2);
    loss = mse_loss(multiply2,y);
    acc = accuracy(multiply2,y,yhat_binary);
    /*for (int l=0; l<50; l++) {
        sprintf(msg, "%f\t%f\t%f\n", data[l][1], y[l], multiply2[l][0]);
        debug(msg);
    }*/
    sprintf(msg, "The loss is %f with an accuracy of %f\n", loss, acc);
    debug(msg);
}


int main(){
    char msg[100];
    float multiply2[ROWS_DOT2][COLS_DOT2];
    srand(time(NULL));
    fit_value(multiply2);
}

