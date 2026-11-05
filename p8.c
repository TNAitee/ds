//8
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_value, received;
    int choice;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Please run with exactly 2 processes.\n");

        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
        send_value = 100;
    else
        send_value = 200;

    if (rank == 0) {
        printf("Choose version:\n");
        printf("1. Unsafe version\n");
        printf("2. Corrected version\n");
        printf("Enter choice: ");
        fflush(stdout);
        scanf("%d", &choice);
    }

    // Share choice with both processes
    MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (choice == 1) {

        // Unsafe version
        printf("P%d: Waiting to send %d\n",
               rank, send_value);
        fflush(stdout);

        // Both processes try to send first
        MPI_Send(&send_value, 1, MPI_INT,
                 1 - rank, 0, MPI_COMM_WORLD);

        MPI_Recv(&received, 1, MPI_INT,
                 1 - rank, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

    }
    else if (choice == 2) {

        // Corrected version using MPI_Sendrecv
        MPI_Sendrecv(&send_value, 1, MPI_INT,
                     1 - rank, 0,
                     &received, 1, MPI_INT,
                     1 - rank, 0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

        if (rank == 0) {
            printf("P0 sent 100\n");
            printf("P0 received 200\n");
        }
        else {
            printf("P1 received 100\n");
            printf("P1 sent 200\n");
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0)
            printf("Communication completed without deadlock.\n");
    }

    MPI_Finalize();

    return 0;
}
