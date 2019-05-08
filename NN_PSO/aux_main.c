#include <stdlib.h>

#include "logica.h"

#include <math.h>
void init_weights1(float weights1[2][3]){
    size_t n_rows = sizeof(weights1) / sizeof(weights1[0]);
    size_t n_cols = sizeof(weights1[0]) / sizeof(weights1[0][0]);
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; i < n_cols; j++) {
            float r = (float) rand();
            weights1[i][j] = r / RAND_MAX
        }
    }
}

void init_weights2(float weights2[3]){
    size_t n_rows = sizeof(weights2) / sizeof(weights2[0]);
    for (int i = 0; i < n_rows; i++) {
        float r = (float) rand();
        weights1[i] = r / RAND_MAX
        }
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

void arrange_weights(float individual[9], float matrix1[2][3], float matrix2[3]){
    matrix1[0][0] = individual[0];
    matrix1[1][0] = individual[1];
    matrix1[0][1] = individual[2];
    matrix1[1][1] = individual[3];
    matrix1[0][2] = individual[4];
    matrix1[1][2] = individual[5];

    matrix2[0] = individual[6];
    matrix2[1] = individual[7];
    matrix2[2] = individual[8];
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

void fit_value(){
    w1 = float weights1[2][3];
    w2 = float weights2[3];
    //arrange_weights() //ha d'anar el vector de weights
    init_weights1(w1);
    init_weights2(w2);
    //matrix_multiplication1(matrix1, matrix2, );
}


int main(){
    srand(time(NULL));

    char msg[100];
    float first[2][2];
    first[0][0] = 1;
    first[0][1] = 1;
    first[1][0] = 1;
    first[1][1] = 1;

    float second[2][2];
    second[0][0] = 2;
    second[0][1] = 2;
    second[1][0] = 2;
    second[1][1] = 2;

    //float arrange_ arrange_weights()
    float multiply[2][3];
    //matrix_multiplication(first,second, multiply);
    //sprintf(msg, "%f\n", multiply[1][0]);
    size_t shape = sizeof(multiply)/ sizeof(multiply[0]);
    size_t shape2 = sizeof(multiply[0])/ sizeof(multiply[0][0]);

    float r = (float) rand();
    sprintf(msg, "%f\n", 2 * r / RAND_MAX + -1);
    debug(msg);

    sprintf(msg, "%f\n", exp(2));
    debug(msg);
    sprintf(msg, "%d\n", shape2);
    debug(msg);
}

