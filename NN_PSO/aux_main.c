#include <stdlib.h>

#include "logica.h"

#include <math.h>
void init_weights(float weights[9]){
    size_t n_rows = sizeof(weights) / sizeof(weights[0]);
    char msg[100];
    sprintf(msg, "%f\n", 3.4);
    debug(msg);
    for (int i = 0; i < n_rows; i++) {
            float r = (float) rand();
            weights[i] = r / RAND_MAX;
        }
    }


void X_train(float data[50][2]) {
    size_t n_rows = sizeof(data) / sizeof(data[0]);
    size_t n_cols = sizeof(data[0]) / sizeof(data[0][0]);
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; i < n_cols; j++) {
            float r = (float) rand();
            data[i][j] = 2 * r / RAND_MAX + -1;
        }
    }
}

void y_train(float data[50][2], float y[50]){
    size_t n_rows = sizeof(data) / sizeof(data[0]);
    for (int i = 0; i < n_rows; i++) {
        if (data[i][1]){
            y[i] = 1;
        }
        else {
            y[i] = 0;
        }
    }
}

float relu(float value){
    if (value>0){
        return value;
    }
    else {
        return 0;
    }
}

float sigmoid(float value){
    return 1/(1+exp(-value));
}

void arrange_weights(float individual[9], float matrix1[2][3], float matrix2[3][1]){
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


void matrix_multiplication1(float first[50][2], float second[2][3], float multiply[50][3]){
    int sum = 0;
    for (int c = 0; c < 50; c++) {
        for (int d = 0; d < 2; d++) {
            for (int k = 0; k < 2; k++) {
                sum = sum + first[c][k]*second[k][d];
            }

            multiply[c][d] = relu(sum);
            sum = 0;
        }
    }
}

void matrix_multiplication2(float first2[50][3], float second2[3][1], float multiply2[50][1]){
    int sum = 0;
    for (int c = 0; c < 50; c++) {
        for (int d = 0; d < 1; d++) {
            for (int k = 0; k < 1; k++) {
                sum = sum + first2[c][k]*second2[k][d];
            }

            multiply2[c][d] = sigmoid(sum);
            sum = 0;
        }
    }
}
//void mse_loss(float yhat, float actual_y)

void fit_value(float multiply2[50][1]){//float weights[9], float matrix1[2][3],float matrix2[3][1],float data[50][2],float y[50],float multiply[50][3],float multiply2[50][1]){
    float weights[9];
    float matrix1[2][3];
    float matrix2[3][1];
    float data[50][2];
    float y[50];
    float multiply[50][3];
    //float multiply2[50][1];
    char msg[100];
    sprintf(msg, "(%f)\n", 3.3);
    debug(msg);

    X_train(data);
    y_train(data, y);
    init_weights(weights);
    arrange_weights(weights, matrix1, matrix2);

    matrix_multiplication1(data, matrix1, multiply);
    matrix_multiplication2(multiply, matrix2, multiply2);

    /*X_train(data[50][2]);
    y_train(data[50][2], y[50]);
    init_weights(weights[9]);
    arrange_weights(weights[9], matrix1[2][3], matrix2[3]);

    matrix_multiplication1(data[50][2], matrix1[2][3], multiply[50][3])
    matrix_multiplication2(multiply[50][3], matrix2[3], multiply2[50][1])*/
    //arrange_weights() //ha d'anar el vector de weights


    //matrix_multiplication1(matrix1, matrix2, );
}


int main(){
    float multiply2[50][1];
    srand(time(NULL));
    /*float weights[9];
    float matrix1[2][3];
    float matrix2[3][1];
    float data[50][2];
    float y[50];
    float multiply[50][3];
    float multiply2[50][1];*/

    char msg[100];
    //float arrange_ arrange_weights()
    //float multiply[2][3];
    //float multiply2[50][1];
    fit_value(multiply2);//weights, matrix1, matrix2, data, y, multiply, multiply2
    sprintf(msg, "%f\n", exp(2));
    debug(msg);
    sprintf(msg, "%f\n", multiply2[0][0]);
    debug(msg);

    //matrix_multiplication(first,second, multiply);
    //sprintf(msg, "%f\n", multiply[1][0]);
    //size_t shape = sizeof(multiply)/ sizeof(multiply[0]);
    //size_t shape2 = sizeof(multiply[0])/ sizeof(multiply[0][0]);

    //float r = (float) rand();
    //sprintf(msg, "%f\n", 2 * r / RAND_MAX + -1);
    //debug(msg);

    //sprintf(msg, "%f\n", exp(2));
    //debug(msg);
    //sprintf(msg, "%d\n", shape2);
    //debug(msg);
}

