#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
    int rank, size;
    int msg_size = 1024;
    int iter = 5;
    char *msg;
    double start, end, total_time;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //  if(size != 5){
    //     if(rank == 0) fprintf(stderr, "Run with -np 5/\n");
    //     MPI_Finalize();
    //     return 1;
    // }

    msg = malloc(msg_size);

    for(int i=0; i<msg_size; i++){
        msg[i] = 'A';
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    for(int i=0; i<iter; i++){
        if(rank == 0){
            MPI_Send(msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        }

        else{  
            MPI_Recv(msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    end = MPI_Wtime();

    if(rank == 0){
        total_time = end - start;

        printf("Message size    : %dbytes\n", msg_size);
        printf("Iterations      : %d\n", iter);
        printf("Total round trip: %d\n", iter);
        printf("Average round-trip : %f seconds\n", total_time/iter);
        printf("Average one-way    : %f seconds\n", total_time/iter/2);
    }

    free(msg);
    MPI_Finalize();

    return 0;
}