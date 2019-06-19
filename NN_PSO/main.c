#include <stdlib.h>

#include "logica.h"


int select_velocity_method = 3;
int convergence_factor = 100;


char checkProgramArguments(int argc) {
    return argc == 4;
}

int main(int argc, char **argv) {

    Config config;
    Swarm swarm;
    char msg[LENGTH];
    int iter = 500;
    int count_convergence = 0;
    double best_fit_local = 0.0;
    int not_converged = 1;
    double best_fit_compare;

    Matrix data;
    Matrix data2;

    Matrix train;
    Matrix y;
    Matrix yhat;

    Matrix test;
    Matrix y_test;
    Matrix yhat_test;

    srand(time(NULL));

    if (!checkProgramArguments(argc)) {
        print(MSG_WRONG_ARGS);
        exit(EXIT_FAILURE);
    }

    config = readConfigFile(argv[1]);
    data = readDataFile(argv[2], 2500, 3);
    data2 = readDataFile(argv[3], 400, 3);

    train = X_train(data);
    y = read_y(data);

    test = X_train(data2);

    y_test = read_y(data2);

    //y = y_train(data);

    //test = X_train();
    //y_test = y_train(test);

    createInitialPopulation(config, &swarm, train, y);


    while (not_converged) {

        getFitValues(config, &swarm, train, y);


        best_fit_compare = trunc(swarm.best_fit * 100000000);

        if (swarm.iterations == 0) {
            best_fit_local = best_fit_compare;
        }

        select_updateVelocity(select_velocity_method, config, &swarm, iter);

        updateParameters(config, &swarm);

        if (best_fit_local == best_fit_compare) {
            count_convergence++;
        } else {
            count_convergence = 0;
            best_fit_local = best_fit_compare;
        }

        if (count_convergence == convergence_factor) {
            not_converged = 0;
        }

        yhat = forward_pass(train, swarm.best_params);

        /*for (int t=0; t<yhat.rows;t++){
            char msg[100];
            sprintf(msg, "%f\t", yhat.matrix[t][0]);
            debug(msg);
        }*/
        float b_train_acc = accuracy(y, yhat);

        yhat_test = forward_pass(test, swarm.best_params);

        float val_loss = mse_loss(y_test, yhat_test);
        float b_val_acc = accuracy(y_test, yhat_test);

        swarm.best_train_acc = b_train_acc;
        swarm.best_val_acc = b_val_acc;

        sprintf(msg, "I: %d, Best fit value: %f, train_acc = %f, val_loss = %f, val_acc = %f weights = %f, %f, %f, %f, %f, %f, %f, %f, %f\n", swarm.iterations, swarm.best_fit, swarm.best_train_acc, val_loss, swarm.best_val_acc,swarm.best_params[0],swarm.best_params[1],swarm.best_params[2],swarm.best_params[3],swarm.best_params[4],swarm.best_params[5],swarm.best_params[6],swarm.best_params[7],swarm.best_params[8]);
        //sprintf(msg, "I: %d, Best fit value: %f, train_acc = %f, val_acc = %f weights = %f, %f, %f, %f, %f, %f, %f, %f, %f\n", swarm.iterations, swarm.best_fit, swarm.best_train_acc, swarm.best_val_acc,swarm.best_params[0],swarm.best_params[1],swarm.best_params[2],swarm.best_params[3],swarm.best_params[4],swarm.best_params[5],swarm.best_params[6],swarm.best_params[7],swarm.best_params[8]);
            //swarm.iterations, swarm.best_fit, swarm.best_params[0],swarm.best_params[1],swarm.best_params[2],swarm.best_params[3],swarm.best_params[4],swarm.best_params[5],swarm.best_params[6],swarm.best_params[7],swarm.best_params[8]);
        debug(msg);

        iter--;
        swarm.iterations++;
    }

    return EXIT_SUCCESS;
}

