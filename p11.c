#include<mpi.h>
#include<stdio.h>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int iter = 1000, threshold = 50, block_size = 256, config[3];
    if(rank == 0){
        config[0] = iter;
        config[1] = threshold;
        config[2] = block_size;
    }

    MPI_Bcast(config, 3, MPI_INT, 0, MPI_COMM_WORLD);
    printf("P%d received: Iteration = %d, Threasholh = %d, Block size = %d\n", 
        rank, config[0], config[1], config[2]);

    MPI_Finalize();
    return 0;

}