#include <stdlib.h>

#include "logica.h"

void matrix_multiplication(float first[2][2], float second[2][2], float multiply[2][2]){
    //float multiply[2][2];
    int sum = 0;
    for (int c = 0; c < 2; c++) {
        for (int d = 0; d < 2; d++) {
            for (int k = 0; k < 2; k++) {
                sum = sum + first[c][k]*second[k][d];
            }

            multiply[c][d] = sum;
            sum = 0;
        }
    }
}

int main(){
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

    float multiply[2][3];
    //matrix_multiplication(first,second, multiply);
    //sprintf(msg, "%f\n", multiply[1][0]);
    size_t shape = sizeof(multiply)/ sizeof(multiply[0]);
    size_t shape2 = sizeof(multiply[0])/ sizeof(multiply[0][0]);

    sprintf(msg, "%d\n", shape);
    debug(msg);
    sprintf(msg, "%d\n", shape2);
    debug(msg);
}

