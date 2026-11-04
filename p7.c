#include<mpi.h>
#include<stdio.h>

int main(int argc, char **argv)
{
    int rank,size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int a[8],b[8];
    for(int i=0;i<8;i++)
    {
        if(rank==0)
        {
            a[i]=(i+1)*10;
        }
        else
        {
            a[i]=(i+1);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double start=MPI_Wtime();

    if(rank==0)
    {
        MPI_Send(a,8,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Recv(b,8,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    }
    else
    {
        MPI_Recv(b,8,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(a,8,MPI_INT,0,0,MPI_COMM_WORLD);
    }

    double blocking_time=MPI_Wtime()-start;

    MPI_Barrier(MPI_COMM_WORLD);

    start=MPI_Wtime();

    MPI_Request rq1,rq2;

    MPI_Irecv(b,8,MPI_INT,1-rank,1,MPI_COMM_WORLD,&rq1);
    MPI_Isend(a,8,MPI_INT,1-rank,1,MPI_COMM_WORLD,&rq2);

    MPI_Wait(&rq1,MPI_STATUS_IGNORE);
    MPI_Wait(&rq2,MPI_STATUS_IGNORE);

    double non_blocking_time=MPI_Wtime()-start;

    printf("P%d received: ",rank);
    for(int i=0;i<8;i++)
    {
        printf("%d ",b[i]);
    }
    printf("\n");

    if(rank==0)
    {
        printf("Blocking time : %f\n",blocking_time);
        printf("Non-Blocking time : %f\n",non_blocking_time);
    }


    MPI_Finalize();
    return 0;
}
