
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

Matrix X_train(Matrix data) {
    Matrix train;
    train = MAT_create(data.rows, data.cols-1);

    for (int i = 0; i < train.rows; i++) {
        for (int j = 0; j < train.cols; j++) {
            float r = (float) rand();
            train.matrix[i][j] = data.matrix[i][j];

        }
    }
    return train;
}

Matrix read_y(Matrix data) {
    Matrix y = MAT_create(data.rows, 1);
    for (int i = 0; i< data.rows; i++){
        y.matrix[i][0] = data.matrix[i][2];
    }
    return y;
}

/*Matrix y_train(Matrix train) {
    Matrix y;
    char msg[LENGTH];
    y = MAT_create(data.rows, 1);
    for (int i = 0; i < data.rows; i++) {
        if (train.matrix[i][1] >= 0) {
            y.matrix[i][0] = 1;
        } else {
            y.matrix[i][0] = 0;
        }
    }
    return y;
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

Matrix matrix_multiplication(Matrix arg1, Matrix arg2, char act_function) {
    Matrix result = MAT_create(arg1.rows, arg2.cols);
    for (int c = 0; c < arg1.rows; c++) {
        for (int d = 0; d < arg2.cols; d++) {
            float sum = 0;
            for (int k = 0; k < arg1.cols; k++) {
                sum += arg1.matrix[c][k] * arg2.matrix[k][d];
            }
            if (act_function == 'r'){
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

float accuracy(Matrix actual_y, Matrix yhat) {
    float sum = 0;


    Matrix yhat_binary = MAT_create(actual_y.rows, 1);
    float see_acc;

    for (int i = 0; i < actual_y.rows; i++) {
        if (yhat.matrix[i][0] >= 0.5) {
            yhat_binary.matrix[i][0] = 1;
        }
        else {
            yhat_binary.matrix[i][0] = 0;
        }
    }
    for (int i = 0; i < yhat_binary.rows; i++) {
        if (yhat_binary.matrix[i][0] == actual_y.matrix[i][0]) {
            sum += 1;
        }
        else {
            sum += 0;
        }
    }
    see_acc = sum/yhat_binary.rows;
    return see_acc;
}

Matrix forward_pass(Matrix data, float weights[9]){

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
        /*switch (act_f){
            case "r":
                relu();
                break;
            case "s":
                sigmoid();
                break;
            default:
                break;
        }*/
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
    return activated;
}

float fit_value(Matrix data, Matrix y, float weights[9], Swarm *swarm) {
    float loss;
    Matrix activated;

    activated = forward_pass(data, weights);

    loss = mse_loss(y, activated);

    return loss;
}