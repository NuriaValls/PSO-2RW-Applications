#include "logica.h"

/**
 * This function reads the file passed as an argument and creates and fills a Config object with the information in the file.
 *
 * @param filename  Name of the file to read.
 * @return The configuration of the file.
 */
Config readConfigFile(char *filename) {
    int file;
    char msg[LENGTH], *aux;
    Config config;

    file = open(filename, O_RDONLY);
    if (file <= 0) {
        sprintf(msg, MSG_FILE_ERR, filename);
        print(msg);
        exit(EXIT_FAILURE);
    }

    aux = readFileDescriptor(file);
    config.n = atoi(aux);
    free(aux);

    aux = readFileDescriptor(file);
    config.d = atoi(aux);
    free(aux);

    config.param_range = NULL;

    for (int i = 0; i < config.d; i++) {
        Range range;

        aux = readFileDescriptor(file);
        range.min = atoi(aux);
        free(aux);

        aux = readFileDescriptor(file);
        range.max = atoi(aux);
        free(aux);

        config.param_range = realloc(config.param_range, (size_t)
        sizeof(range) * (i + 1));
        config.param_range[i] = range;
    }

    aux = readFileDescriptor(file);
    config.vmax = atof(aux);
    free(aux);

    aux = readFileDescriptor(file);
    config.l = atoi(aux);
    free(aux);

    config.layers = NULL;

    for (int i=0; i < config.l; i++) {
        Layer layer;

        aux = readFileDescriptor(file);
        layer.neurons = atoi(aux);

        free(aux);

        aux = readFileDescriptor(file);
        layer.activ = aux[0];
        free(aux);

        config.layers = realloc(config.layers, (size_t)
        sizeof(layer) * (i + 1));
        config.layers[i] = layer;

    }

    close(file);

    sprintf(msg, "|%d/%d/%f|\n", config.n, config.d, config.vmax);
    debug(msg);

    return config;
}


/**
 * This function reads the file passed as an argument and creates and fills a Matrix object with the information in the file. It must be both the features and the label.
 *
 * @param filename  Name of the file to read.
 * @param nRows     The number of rows of the file.
 * @param nCols     The number of columns of the file.
 * @return A matrix object with the file read.
 */
Matrix readDataFile(char *filename, int nRows, int nCols) {
    int file;
    char msg[LENGTH], *aux;
    Matrix mtx;

    file = open(filename, O_RDONLY);
    if (file <= 0) {
        sprintf(msg, MSG_FILE_ERR, filename);
        print(msg);
        exit(EXIT_FAILURE);
    }

    mtx = MAT_create(nRows, nCols);

    for (int i = 0; i<nRows; i++) {
        aux = readFileDescriptor(file);
        mtx.matrix[i][0] = atof(aux);
        free(aux);

        aux = readFileDescriptor(file);
        mtx.matrix[i][1] = atof(aux);
        free(aux);

        aux = readFileDescriptor(file);
        mtx.matrix[i][2] = atof(aux);
        free(aux);
    }

    close(file);

    return mtx;
}


/**
 * Initializes all the particles of the swarm with random values, and its respective parameters.
 *
 * @param config    Configuration information.
 * @param swarm     Contains all the information of the swarm and the particles.
 * @param data      The training data without labels.
 * @param y         The set of labels of the training data.
 */
void createInitialPopulation(Config config, Swarm *swarm, Matrix data, Matrix y) {

    swarm->particles = malloc((size_t) sizeof(Particle) * config.n);
    char msg[LENGTH];

    for (int i = 0; i < config.n; i++) {
        Particle p;

        p.params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {
            float r = (float) rand();

            p.params[j] = (config.param_range[j].max - config.param_range[j].min) * r / RAND_MAX + config.param_range[j].min;
        }


        p.velocity = malloc((size_t) sizeof(float) * config.d);
        p.best_params = malloc((size_t) sizeof(float) * config.d);

        for (int j = 0; j < config.d; j++) {

            p.velocity[j] = (float) 0;
            p.best_params[j] = p.params[j];
        }

        char msg[300];

        float weights[config.d];
        for (int z = 0; z < config.d; z++){
            weights[z] = p.best_params[z];
        }
        //float weights[9] = {p.best_params[0], p.best_params[1], p.best_params[2], p.best_params[3], p.best_params[4], p.best_params[5], p.best_params[6], p.best_params[7], p.best_params[8]};


        p.best_fit = fit_value(data, y, weights, swarm, config);
        swarm->particles[i] = p;
    }

    swarm->best_fit = swarm->particles[0].best_fit;
    swarm->best_params = malloc((size_t) sizeof(float) * config.d);
    swarm->iterations = 0;
    swarm->vmax = malloc((size_t) sizeof(float) * config.d);
    swarm->best_val_acc = 0;

    for (int j = 0; j < config.d; j++) {
        swarm->vmax[j] = config.vmax * (config.param_range[j].max - config.param_range[j].min);
    }

}

/**
 * Calculates the fit values of all the particles of the swarm.
 *
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 * @param data      The training data without labels.
 * @param y         The set of labels of the training data.
 */
void getFitValues(Config c, Swarm *swarm, Matrix data, Matrix y) {

    for (int i=0; i<c.n; i++) {
        float fit = 0;
        float *acc;

        float weights[c.d];
        for (int z = 0; z < c.d; z++){
            weights[z] = swarm->particles[i].params[z];
        }


        fit = fit_value(data, y, weights, swarm, c);

        if (fit < swarm->particles[i].best_fit) {
            swarm->particles[i].best_fit = fit;
            for (int j = 0; j < c.d; j++) {
                swarm->particles[i].best_params[j] = swarm->particles[i].params[j];
            }
        }

        if (fit < swarm->best_fit) {
            swarm->best_fit = fit;
            for (int j = 0; j < c.d; j++) {
                swarm->best_params[j] = swarm->particles[i].params[j];
            }
        }
    }
}

/**
 * Updates the velocity of all the parameters of all the particles of the swarm.
 *
 * @param c         Configuration information.
 * @param swarm     Contains all the information of the swarm and the particles.
 */
void updateVelocity(Config c, Swarm *swarm) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 1.6 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 1.7 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            velocity = swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}

/**
 * Updates the velocity of all the parameters of all the particles of the swarm controlling the maximum velocity value.
 *
 * @param c         Configuration information.
 * @param swarm     Contains all the information of the swarm and the particles.
 */
void updateVelocity_vMax(Config c, Swarm *swarm) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 1.6 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 1.7 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            velocity = swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity > swarm->vmax[j] ? swarm->vmax[j] : velocity;
        }
    }
}

/**
 * Updates the velocity of all the parameters of all the particles of the swarm using the fixed weights method.
 *
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 */
void updateVelocity_fixedWeights(Config c, Swarm *swarm) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            cognitive = 2 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 2 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            velocity = (float) (V_FIXED_WEIGHT_VI * swarm->particles[i].velocity[j] + V_FIXED_WEIGHT_C * cognitive + V_FIXED_WEIGHT_C * social);
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}

/**
 * Updates the velocity of all the parameters of all the particles of the swarm using the increasing inertia weights method.
 *
 * @param c         Configuration information.
 * @param swarm     Contians all the information of the swarm and the particles.
 * @param max_t     Maximum iterations.
 */
void updateVelocity_decreasingInertia(Config c, Swarm *swarm, int max_t) {

    char msg[LENGTH];
    float cognitive;
    float social;
    float velocity;
    float w;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            w = (float) (0.4 + tanh(swarm->iterations*(0.5/max_t)));

            cognitive = 1.6 * ((float) rand()) / RAND_MAX * (swarm->particles[i].best_params[j] - swarm->particles[i].params[j]);
            social = 1.7 * ((float) rand()) / RAND_MAX * (swarm->best_params[j] - swarm->particles[i].params[j]);

            velocity = w * swarm->particles[i].velocity[j] + cognitive + social;
            swarm->particles[i].velocity[j] = velocity;
        }
    }
}

/**
 * Calls the function to updates velocities according to a selection.
 *
 * @param select    Selection of the velocity method.
 * @param c         Configuration information.
 * @param swarm     Contains all the information of the swarm and the particles.
 * @param max_t     Maximum iterations.
 */
void select_updateVelocity(int select, Config c, Swarm *swarm, int max_t) {

    switch (select) {
        case 0:
            updateVelocity(c, swarm);
            break;
        case 1:
            updateVelocity_vMax(c, swarm);
            break;
        case 2:
            updateVelocity_fixedWeights(c, swarm);
            break;
        case 3:
            updateVelocity_decreasingInertia(c, swarm, max_t);

        default:
            break;
    }
}

/**
 * Updates the the parameters of all the particles of the swarm.
 *
 * @param c         Configuration information.
 * @param swarm     Contains all the information of the swarm and the particles.
 */
void updateParameters(Config c, Swarm *swarm) {

    float aux = 0;

    for (int i=0; i<c.n; i++) {
        for (int j = 0; j < c.d; j++) {

            aux = swarm->particles[i].params[j] + swarm->particles[i].velocity[j];

            if (aux >= c.param_range[j].min && aux <= c.param_range[j].max) {
                swarm->particles[i].params[j] = aux;
            }
        }
    }
}

/**
 * Creates an empty Matrix type object.
 *
 * @param rows      Number of rows of the Matrix.
 * @param cols      Number of columns of the Matrix.
 * @return m        The empty Matrix.
 */
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

/**
 * Creates a Matrix with just the features (without labels).
 *
 * @param data          All observations (features and labels).
 * @return train        The training data without labels in a Matrix.
 */
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

/**
 * Creates a Matrix with just the labels (without features).
 *
 * @param data          All observations (features and labels).
 * @return y            The training labels in a Matrix.
 */
Matrix read_y(Matrix data) {
    Matrix y = MAT_create(data.rows, 1);
    for (int i = 0; i< data.rows; i++){
        y.matrix[i][0] = data.matrix[i][2];
    }
    return y;
}

/**
 * The ReLu activation function applied to a number.
 *
 * @param value         The number.
 * @return value        The number after the ReLu operation.
 */
float relu(float value) {
    if (value >= 0) {
        return value;
    } else {
        return 0;
    }
}

/**
 * The ReLu activation function applied to a number.
 *
 * @param value         The number.
 * @return value        The number after the sigmoid operation.
 */
float sigmoid(float value) {
    return 1 / (1 + exp(-value));
}


/**
 * Performs a dot product between two matrices and applies the activation function element by element.
 *
 * @param arg1              The first matrix to multiply.
 * @param arg2              The second matrix to multiply.
 * @param act_function      Either "r" standing for "ReLu" or "s" standing for "sigmoid".
 * @return result           The resultant matrix already activated.
 */
Matrix matrix_multiplication(Matrix arg1, Matrix arg2, char act_function) {
    Matrix result = MAT_create(arg1.rows, arg2.cols);
    for (int c = 0; c < arg1.rows; c++) {
        for (int d = 0; d < arg2.cols; d++) {
            float sum = 0;
            for (int k = 0; k < arg1.cols; k++) {
                sum += arg1.matrix[c][k] * arg2.matrix[k][d];
            }

            switch (act_function) {
                case 'r':
                    result.matrix[c][d] = relu(sum);
                    break;
                case 's':
                    result.matrix[c][d] = sigmoid(sum);
                    break;
                default:
                    break;
            }
        }
    }
    return result;
}


/**
 * Performs a dot product between two matrices and applies the activation function element by element.
 *
 * @param actual_y      The actual set of labels.
 * @param yhat          The prediction of the labels.
 * @return mse_loss     The value of the mean square error loss function.
 */
float mse_loss(Matrix actual_y, Matrix yhat) {
    float sum = 0;
    for (int i = 0; i < actual_y.rows; i++) {
        sum = sum + pow((yhat.matrix[i][0] - actual_y.matrix[i][0]), 2);
    }
    return sum / actual_y.rows;
}

/**
 * Performs a dot product between two matrices and applies the activation function element by element.
 *
 * @param actual_y      The actual set of labels.
 * @param yhat          The prediction of the labels.
 * @return see_acc      The accuracy measured as a fraction of 1. Proportion of samples that have been classified correctly,
 */
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

/**
 * Performs the forward propagation step.
 *
 * @param data          The data without labels.
 * @param weights       The set of weights that will perform the forward pass.
 * @param config        Configuration information.
 * @return activated    The prediction of the samples after doing the whole forward pass.
 */
Matrix forward_pass(Matrix data, float* weights, Config config) {

    Matrix activated;
    activated = data;
    int initial_value = 0;

    for (int i = 0; i < config.l - 1; i++) {
        Layer actual_layer = config.layers[i];
        Layer next_layer = config.layers[i+1];

        int n_weights = actual_layer.neurons * next_layer.neurons;

        float vector_weights[n_weights];
        Matrix w_matrix = MAT_create(actual_layer.neurons, next_layer.neurons);;


        int count = 0;
        for (int j = initial_value; j < initial_value+n_weights; j++){
            vector_weights[count] = weights[j];
            count = count + 1;
        }

        count = 0;
        for (int j = 0; j < actual_layer.neurons; j++) {
            for (int k = 0; k < next_layer.neurons; k++) {
                w_matrix.matrix[j][k] = vector_weights[count];
                count = count + 1;
            }
        }

        activated = matrix_multiplication(activated, w_matrix, next_layer.activ);
        initial_value = initial_value + n_weights;
    }
    return activated;
}


/**
 * Calculates the fit value of the function.
 *
 * @param data          The data without labels.
 * @param y             The set of labels.
 * @param weights       The set of weights that will perform the forward pass.
 * @param swarm         Contains all the information of the swarm and the particles.
 * @param config        Configuration information.
 * @return loss         The value of the loss function.
 */
float fit_value(Matrix data, Matrix y, float* weights, Swarm *swarm, Config config) {
    float loss;
    Matrix activated;

    activated = forward_pass(data, weights, config);

    loss = mse_loss(y, activated);

    return loss;
}