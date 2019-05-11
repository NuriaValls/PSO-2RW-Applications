
#include "neural_net.h"

//typedef struct {
//    float **matrix;
//    int cols;
//    int rows;
//} Matrix;


void init_weights(float weights[INIT_WEIGHTS]) {
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

void X_test(float test[ROWS_TEST][COLS_TEST]) {
    for (int i = 0; i < ROWS_TEST; i++) {
        for (int j = 0; j < COLS_TEST; j++) {
            float r = (float) rand();
            test[i][j] = 2 * r / RAND_MAX + -1;
        }
    }
}

void y_train(float data[ROWS_DATA][COLS_DATA], float y[ROWS_Y]) {
    for (int i = 0; i < ROWS_DATA; i++) {
        if (data[i][1] >= 0) {
            y[i] = 1;
        } else {
            y[i] = 0;
        }
    }
}

void make_y_test(float test[ROWS_TEST][COLS_TEST], float y_test[ROWS_TEST]) {
    for (int i = 0; i < ROWS_TEST; i++) {
        if (test[i][1] >= 0) {
            y_test[i] = 1;
        } else {
            y_test[i] = 0;
        }
    }
}

float relu(float value) {
    if (value >= 0) {
        return value;
    } else {
        return 0;
    }
}

float sigmoid(float value) {
    return 1 / (1 + exp(-value));
}

void arrange_weights(float individual[INIT_WEIGHTS], float matrix1[ROWS_W1][COLS_W1], float matrix2[ROWS_W2][COLS_W2]) {
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

//void matrix_multiplication(float **first, float **second, float **result, int first_rows, int first_cols, int second_rows, int second_cols) {
//    float sum;
//
//    if (first_cols != second_rows)
//        return;
//
//    for (int c = 0; c < first_rows; c++) {
//        for (int d = 0; d < second_cols; d++) {
//            for (int k = 0, sum = 0; k < first_cols; k++) {
//                sum += first[c][k] * second[k][d];
//            }
//            multiply[c][d] = sum;
//        }
//    }
//}

//Matrix matrix_multiplication(Matrix arg1, Matrix arg2) {
//    float sum;
//    Matrix result = MATRIX_CREATE(arg1.rows, arg2.cols);
//
//    if (arg1.cols != arg2.rows)
//        return NULL;
//
//    for (int c = 0; c < arg1.rows; c++) {
//        for (int d = 0; d < arg2.cols; d++) {
//            for (int k = 0, sum = 0; k < arg1.cols; k++) {
//                sum += arg1.matrix[c][k] * arg2.matrix[k][d];
//            }
//            result.matrix[c][d] = sum;
//        }
//    }
//
//    return result;
//}

void matrix_multiplication1(float first[ROWS_DATA][COLS_DATA], float second[ROWS_W1][COLS_W1], float multiply[ROWS_DOT1][COLS_DOT1]) {
    float sum = 0;
    for (int c = 0; c < ROWS_DATA; c++) {
        for (int d = 0; d < COLS_DOT1; d++) {
            for (int k = 0; k < COLS_DATA; k++) {
                sum = sum + first[c][k] * second[k][d];
            }
            //sprintf(msg, "%f\n", sum);
            //debug(msg);
            multiply[c][d] = sum;
            sum = 0;
        }
    }
}

void matrix_multiplication2(float first2[ROWS_DOT1][COLS_DOT1], float second2[ROWS_W2][COLS_W2],
                            float multiply2[ROWS_DOT2][COLS_DOT2]) {
    float sum = 0;
    for (int c = 0; c < ROWS_DOT1; c++) {
        for (int d = 0; d < COLS_DOT2; d++) {
            for (int k = 0; k < COLS_DOT1; k++) {
                sum = sum + first2[c][k] * second2[k][d];
            }

            multiply2[c][d] = sigmoid(sum);
            sum = 0;
        }
    }
}

float mse_loss(float yhat[ROWS_Y][COLS_Y], float actual_y[ROWS_Y]) {
    float sum = 0;
    for (int i = 0; i < ROWS_Y; i++) {
        sum = sum + pow((yhat[i][0] - actual_y[i]), 2);
    }
    return sum / ROWS_DATA;
}

float accuracy(float yhat[ROWS_Y][COLS_Y], float actual_y[ROWS_Y], float yhat_binary[ROWS_Y]) {
    float sum = 0;
    for (int i = 0; i < ROWS_Y; i++) {
        if (yhat[i][0] >= 0.5) {
            yhat_binary[i] = 1;
        }
    }
    for (int i = 0; i < ROWS_Y; i++) {
        if (yhat_binary[i] == actual_y[i]) {
            sum += 1;
        }
    }
    return sum / ROWS_Y;
}

void predict(float input[2], float matrix1[ROWS_W1][COLS_W1], float matrix2[ROWS_W2][COLS_W2],
             float dot1[ROWS_DOT1][COLS_DOT1], float dot2[1]) {
    //float matrix1[ROWS_W1][COLS_W1];
    //float matrix2[ROWS_W2][COLS_W2];

    matrix_multiplication1(input, matrix1, dot1);
    matrix_multiplication1(dot1, matrix2, dot2);
}

float fit_value(float weights[9]) { //float weights[9], float matrix1[2][3],float matrix2[3][1],float data[50][2],float y[50],float multiply[50][3],float multiply2[50][1]){
    float multiply2[ROWS_DOT2][COLS_DOT2];
    //float weights[INIT_WEIGHTS];
    float matrix1[ROWS_W1][COLS_W1];
    float matrix2[ROWS_W2][COLS_W2];
    float data[ROWS_DATA][COLS_DATA];
    float y[ROWS_Y];
    float test[ROWS_TEST][COLS_TEST];
    float y_test[ROWS_TEST];
    float yhat_binary[ROWS_Y];
    float multiply[ROWS_DOT1][COLS_DOT1];
    float loss;
    float acc;
    char msg[300];
    float dot1[ROWS_DOT1][COLS_DOT1];
    float dot2[1];
    float input[2];


    X_train(data);
    y_train(data, y);
    X_test(test);
    make_y_test(test, y_test);

    //init_weights(weights);
    arrange_weights(weights, matrix1, matrix2);
    matrix_multiplication1(data, matrix1, multiply);
    matrix_multiplication2(multiply, matrix2, multiply2);
    loss = mse_loss(multiply2, y);
    acc = accuracy(multiply2, y, yhat_binary);
    /*for (int i=0;i<ROWS_TEST;i++){
        predict(test[i], matrix1, matrix2, dot1, dot2);
        sprintf(msg, "%f\n", dot2[0]);
        debug(msg);
    }*/


    /*for (int l=0; l<20; l++) {
        sprintf(msg, "%f\t%f\t%f\n", data[l][1],test[l][1], y_test[l]);
        debug(msg);
    }*/
    return loss;
}