#include <stdlib.h>

#include "logica.h"


int select_velocity_method = 3;
int convergence_factor = 100;


char checkProgramArguments(int argc) {
    return argc == 2;
}

int main(int argc, char **argv) {

    Config config;
    Swarm swarm;
    char msg[LENGTH];
    int iter = 100;
    int count_convergence = 0;
    double best_fit_local = 0.0;
    int not_converged = 1;
    double best_fit_compare;

    Matrix yhat;
    Matrix data;
    Matrix y;
    Matrix test;
    Matrix y_test;
    Matrix yhat_test;

    srand(time(NULL));

    if (!checkProgramArguments(argc)) {
        print(MSG_WRONG_ARGS);
        exit(EXIT_FAILURE);
    }

    config = readConfigFile(argv[1]);

    data = X_train();
    y = y_train(data);

    test = X_train();
    y_test = y_train(test);

    createInitialPopulation(config, &swarm, data, y);


    while (not_converged) {

        getFitValues(config, &swarm, data, y);


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

        yhat = forward_pass(data, swarm.best_params);
        float b_train_acc = accuracy(y, yhat);

        yhat_test = forward_pass(test, swarm.best_params);

        float val_loss = mse_loss(y_test, yhat_test);
        float b_val_acc = accuracy(y_test, yhat_test);

        swarm.best_train_acc = b_train_acc;
        swarm.best_val_acc = b_val_acc;

        sprintf(msg, "I: %d, Best fit value: %f, train_acc = %f, val_loss = %f, val_acc = %f\n", swarm.iterations, swarm.best_fit, swarm.best_train_acc, val_loss, swarm.best_val_acc);//,swarm.best_params[0],swarm.best_params[1],swarm.best_params[2],swarm.best_params[3],swarm.best_params[4],swarm.best_params[5],swarm.best_params[6],swarm.best_params[7],swarm.best_params[8]);
            //swarm.iterations, swarm.best_fit, swarm.best_params[0],swarm.best_params[1],swarm.best_params[2],swarm.best_params[3],swarm.best_params[4],swarm.best_params[5],swarm.best_params[6],swarm.best_params[7],swarm.best_params[8]);
        debug(msg);

        iter--;
        swarm.iterations++;
    }

    return EXIT_SUCCESS;
}

