
#include "neural_net.h"


Matrix MAT_create(int rows, int cols){
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.matrix = malloc(sizeof(float *) * m.rows);

    for (int i = 0; i < rows; i++){
        m.matrix[i] = malloc(sizeof(float)*cols);
    }
    return m;
}

Matrix X_train() {
    Matrix data;
    data = MAT_create(ROWS_DATA, COLS_DATA);

    for (int i = 0; i < data.rows; i++) {
        for (int j = 0; j < data.cols; j++) {
            float r = (float) rand();
            data.matrix[i][j] = 2 * r / RAND_MAX + -1;

        }
    }
    return data;
}

/*void X_test(float test[ROWS_TEST][COLS_TEST]) {
    for (int i = 0; i < ROWS_TEST; i++) {
        for (int j = 0; j < COLS_TEST; j++) {
            float r = (float) rand();
            test[i][j] = 2 * r / RAND_MAX + -1;
        }
    }
}*/

/*void y_train(float data[ROWS_DATA][COLS_DATA], float y[ROWS_Y]) {
    for (int i = 0; i < ROWS_DATA; i++) {
        if (data[i][1] >= 0) {
            y[i] = 1;
        } else {
            y[i] = 0;
        }
    }
}*/

Matrix y_train(Matrix data) {
    Matrix y;
    char msg[LENGTH];
    y = MAT_create(data.rows, 1);
    for (int i = 0; i < data.rows; i++) {
        if (data.matrix[i][1] >= 0) {
            y.matrix[i][0] = 1;
        } else {
            y.matrix[i][0] = 0;
        }
    }
    return y;
}


/*void make_y_test(float test[ROWS_TEST][COLS_TEST], float y_test[ROWS_TEST]) {
    for (int i = 0; i < ROWS_TEST; i++) {
        if (test[i][1] >= 0) {
            y_test[i] = 1;
        } else {
            y_test[i] = 0;
        }
    }
}*/

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

Matrix matrix_multiplication(Matrix arg1, Matrix arg2, char act_function[1]) {
    Matrix result = MAT_create(arg1.rows, arg2.cols);
    for (int c = 0; c < arg1.rows; c++) {
        for (int d = 0; d < arg2.cols; d++) {
            float sum = 0;
            for (int k = 0; k < arg1.cols; k++) {
                sum += arg1.matrix[c][k] * arg2.matrix[k][d];
            }
            if (act_function == "r"){
                result.matrix[c][d] = relu(sum);
            }
            else {
                result.matrix[c][d] = sigmoid(sum);
            }

        }
    }
    return result;
}

float mse_loss(Matrix actual_y, Matrix yhat) {
    float sum = 0;
    for (int i = 0; i < actual_y.rows; i++) {
        sum = sum + pow((yhat.matrix[i][0] - actual_y.matrix[i][0]), 2);
    }
    return sum / ROWS_DATA;
}

float accuracy(float yhat[ROWS_Y][COLS_Y], float actual_y[ROWS_Y], float yhat_binary[ROWS_Y]) {
    float sum = 0;
    float see_acc;
    char msg[100];

    for (int i = 0; i < ROWS_Y; i++) {
        if (yhat[i][0] >= 0.5) {
            yhat_binary[i] = 1;
        }
        else {
            yhat_binary[i] = 0;
        }
    }
    for (int i = 0; i < ROWS_Y; i++) {
        if (yhat_binary[i] == actual_y[i]) {
            sum += 1;
        }
        else {
            sum += 0;
        }
    }

    /*for (int i=0;i<ROWS_Y;i++){
        sprintf(msg, "%f\t%f\n", yhat_binary[i], actual_y[i]);
        debug(msg);
    }*/

    see_acc = sum/ROWS_Y;
    //sprintf(msg, "%f\n", see_acc);
    //debug(msg);
    return see_acc;
}

/*void predict(float input[2], float matrix1[ROWS_W1][COLS_W1], float matrix2[ROWS_W2][COLS_W2],
             float dot1[ROWS_DOT1][COLS_DOT1], float dot2[1]) {
    //float matrix1[ROWS_W1][COLS_W1];
    //float matrix2[ROWS_W2][COLS_W2];

    matrix_multiplication1(input, matrix1, dot1);
    matrix_multiplication1(dot1, matrix2, dot2);
}*/

float fit_value(float weights[9], float *train_acc, float *val_acc) { //float weights[9], float matrix1[2][3],float matrix2[3][1],float data[50][2],float y[50],float multiply[50][3],float multiply2[50][1]){
    Matrix output;
    Matrix data;
    data = MAT_create(ROWS_DATA, COLS_DATA);
    Matrix y = MAT_create(data.rows, 1);
    float test[ROWS_TEST][COLS_TEST];
    float y_test[ROWS_TEST];

    float yhat_binary[ROWS_Y];
    float loss;


    data = X_train();
    y = y_train(data);

    //init_weights(weights);
    int architecture[N_LAYERS] = {2,3,1};
    char acts[2] = "rs";

    int dim_weights[N_LAYERS-1];

    for (int i = 0; i < N_LAYERS-1; i++){
        dim_weights[i] = architecture[i]*architecture[i+1];
    }
    Matrix activated;
    int initial_value = 0;
    activated = data;

    for (int i = 0; i < N_LAYERS-1; i++){
        char act_f = acts[i];
        int count = 0;
        int n_weights = dim_weights[i];

        float vector_weights[n_weights];
        Matrix w_matrix;

        w_matrix = MAT_create(architecture[i], architecture[i+1]);

        for (int j = initial_value; j < initial_value+n_weights; j++){
            vector_weights[count] = weights[j];
            count = count + 1;
        }

        count = 0;

        initial_value = initial_value+n_weights;

        for (int j = 0; j < architecture[i]; j++){
            for (int k = 0; k < architecture[i+1]; k++){
                w_matrix.matrix[j][k] = vector_weights[count];
                count = count + 1;
            }
        }

        activated = matrix_multiplication(activated, w_matrix, act_f);
    }

    loss = mse_loss(y, activated);
    //*train_acc = accuracy(output, y, yhat_binary);

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