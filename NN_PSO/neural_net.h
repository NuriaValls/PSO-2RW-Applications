#ifndef _NEURAL_NET_H_
#define _NEURAL_NET_H_

#include <math.h>
#include <stdlib.h>

#define ROWS_DATA 50
#define COLS_DATA 2
#define ROWS_TEST 20
#define COLS_TEST 2
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

void init_weights(float weights[INIT_WEIGHTS]);
void X_train(float data[ROWS_DATA][COLS_DATA]);
void X_test(float test[ROWS_TEST][COLS_TEST]);
void y_train(float data[ROWS_DATA][COLS_DATA], float y[ROWS_Y]);
void make_y_test(float test[ROWS_TEST][COLS_TEST], float y_test[ROWS_TEST]);
float relu(float value);
float sigmoid(float value);
void arrange_weights(float individual[INIT_WEIGHTS], float matrix1[ROWS_W1][COLS_W1], float matrix2[ROWS_W2][COLS_W2]);
void matrix_multiplication1(float first[ROWS_DATA][COLS_DATA], float second[ROWS_W1][COLS_W1], float multiply[ROWS_DOT1][COLS_DOT1]);
void matrix_multiplication2(float first2[ROWS_DOT1][COLS_DOT1], float second2[ROWS_W2][COLS_W2], float multiply2[ROWS_DOT2][COLS_DOT2]);
float mse_loss(float yhat[ROWS_Y][COLS_Y], float actual_y[ROWS_Y]);
float accuracy(float yhat[ROWS_Y][COLS_Y], float actual_y[ROWS_Y], float yhat_binary[ROWS_Y]);
void predict(float input[2], float matrix1[ROWS_W1][COLS_W1], float matrix2[ROWS_W2][COLS_W2], float dot1[ROWS_DOT1][COLS_DOT1], float dot2[1]);
float fit_value(float weights[9]);

#endif