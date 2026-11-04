#include<mpi.h>
#include<stdio.h>
#include<math.h>

double f(double x){
    return x*x;
}

int main(int argc, char **argv){
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 1000;
    double a = 0, b = 1;
    double h, local_a, local_b;
    double sum = 0, total;
    double x, exact, error;

    h = (b-a)/n;

    int local_n = n/size;
    local_a = a + rank*local_n * h;
    local_b = local_a + rank*local_n * h;

    sum = (f(local_a) + f(local_b)) / 2.0;

    for(int i=1; i<local_n; i++){
        x = local_a + i*h;
        sum += f(x);
    }

    sum = sum * h;

    printf("P%d : local integer = %f\n", rank, sum);

    MPI_Reduce(&sum, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0){
        exact = 1.0/3.0;
        error = fabs(exact - total);

        printf("\nNumerical integral = %f\n",total);
        printf("Exact integral = %f\n",exact);
        printf("Error = %f\n",error);

    }

    MPI_Finalize();
    return 0;
}