#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define DARTS 50000
#define ROUNDS 100

void srandom(unsigned seed);
double dboard(int darts);

int main(int argc, char *argv[]) {
    double homepi;
    MPI_Comm parentcomm;
    int taskid, rc, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_get_parent(&parentcomm);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    if (parentcomm == MPI_COMM_NULL) {
        printf("Error: No parent found!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    printf("MPI worker task %d has started...\n", taskid);

    srandom(taskid);

    for (i = 0; i < ROUNDS; i++) {
        homepi = dboard(DARTS);
        printf("Worker %d contributing result for round %d: %f\n", taskid, i, homepi);

        rc = MPI_Reduce(&homepi, NULL, 1, MPI_DOUBLE, MPI_SUM, 0, parentcomm);
    }

    MPI_Finalize();
    return 0;
}

double dboard(int darts) {
    #define sqr(x) ((x) * (x))
    long random(void);
    double x_coord, y_coord, pi, r;
    int score = 0, n;
    unsigned int cconst;

    if (sizeof(cconst) != 4) {
        printf("Wrong data size for cconst variable in dboard routine!\n");
        printf("See comments in source file. Quitting.\n");
        exit(1);
    }

    cconst = 2 << (31 - 1);
    
    for (n = 1; n <= darts; n++) {
        r = (double)random() / cconst;
        x_coord = (2.0 * r) - 1.0;
        r = (double)random() / cconst;
        y_coord = (2.0 * r) - 1.0;

        if ((sqr(x_coord) + sqr(y_coord)) <= 1.0) {
            score++;
        }
    }

    pi = 4.0 * (double)score / (double)darts;
    return pi;
}