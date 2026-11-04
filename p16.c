#include<mpi.h>
#include<stdio.h>


int main(int argc, char **argv){
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int prefix[16], sum, value;

    value = (rank+1) * 10;

    MPI_Scan(&value, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("P%d: Local value = %d, Prefix sum = %d\n", rank, value, sum);

    MPI_Gather(&sum, 1, MPI_INT, prefix, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        printf("Prefix result: ");

        for(int i=0; i<size; i++){
            printf("%d ", prefix[i]);
        }
        printf("\n");
    }


    MPI_Finalize();
    return 0;
}