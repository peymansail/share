#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define DARTS 50000
#define ROUNDS 100
#define MASTER 0

void srandom (unsigned seed);
double dboard (int darts);

int main(int argc, char *argv[]) {
    double avepi, pi, pisum, homepi = 0.0;
    int i, numtasks;
    MPI_Status status;
    MPI_Comm intercomm;
    double start, end;

    if (argc != 2) {
        printf("Usage: %s num_workers\n", argv[0]);
        return 1;
    }
    numtasks = atoi(argv[1]);

    MPI_Init(&argc, &argv);

    start = MPI_Wtime();

    printf("MASTER NOW STARTING %d WORKERS...\n", numtasks);
    
    MPI_Comm_spawn("mpi_worker", MPI_ARGV_NULL, numtasks, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm, MPI_ERRCODES_IGNORE);
    
    printf("Workers spawned.\n");

    avepi = 0;
    for (i = 0; i < ROUNDS; i++) {
        printf("Master gathering results for round %d...\n", i);
        MPI_Reduce(&homepi, &pisum, 1, MPI_DOUBLE, MPI_SUM, MPI_ROOT, intercomm);
        
        pi = pisum / (double)numtasks;
        avepi = ((avepi * i) + pi) / (i + 1);
        printf("   After %8d throws, average value of pi = %10.8f\n", (DARTS * (i + 1)), avepi);
    }
    
    printf("\nReal value of PI: 3.1415926535897 \n");

    end = MPI_Wtime();
    printf("Runtime = %f\n", end - start);

    MPI_Finalize();
    return 0;
}