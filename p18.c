#include<mpi.h >
#include<stdio.h>

int main(int argc, char **argv){

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local[10] = {0}, global[10];

    int data[4][4] = 
    {
        {1, 2, 2, 3},
        {2, 3, 4, 4},
        {1, 4, 5, 5},
        {0, 2, 5, 9}

    };

    for(int i=0; i<4; i++){
        local[data[rank][i]]++;
    };

    MPI_Allreduce(local, global, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if(rank == 0){
        for(int i=0; i<10; i++){
            printf("%d      %d\n", i, global[i]);

        }
        printf("total number of elemenys = 16\n");

    }

    MPI_Finalize();
    return 0;
    
}