#include <stdio.h>
#include <mpich/mpi.h>
#include <math.h>
struct message {
    int size;
    char* payload;
};

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size, q, col_rank, row_rank, diag_rank;
    MPI_Comm row_comm, col_comm, diag_comm;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    q = sqrt(size);

    MPI_Comm_split(MPI_COMM_WORLD, rank % q, rank / q, &col_comm);
    MPI_Comm_rank(col_comm, &col_rank);

    MPI_Comm_split(MPI_COMM_WORLD, rank / q, rank % q, &row_comm);
    MPI_Comm_rank(row_comm, &row_rank);

    MPI_Comm_split(MPI_COMM_WORLD, row_rank == col_rank, row_rank, &diag_comm);
    MPI_Comm_rank(diag_comm, &diag_rank);

    if(row_rank == col_rank){
        int value;
        if(!diag_rank)
        {
            value = 5;
            for(int i = 1; i < q; i++)
                MPI_Send(&value, 1, MPI_INT, i, 0, diag_comm);
        }
        else
            MPI_Recv(&value, 1, MPI_INT, 0, 0, diag_comm, MPI_STATUS_IGNORE);

        printf("P[%d], [%d, %d], [%d]: %d\n", rank, col_rank, row_rank, diag_rank, value);
    }

    MPI_Finalize();
    return 0;
}