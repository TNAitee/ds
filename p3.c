#include<mpi.h>
#include<stdio.h>

int main(int argc, char **argv){
    int rank, size;
    int value = 100, received;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    if(rank == 0){
        start = MPI_Wtime();
    }

    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Sendrecv(&value, 1, MPI_INT, next, 0,
                &received, 1, MPI_INT, prev, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("P%d receives %d from P%d\n", rank, received, prev);

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){
        end = MPI_Wtime();
        printf("Ring communication completed\n");
        printf("COmmunication time =%f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;


}