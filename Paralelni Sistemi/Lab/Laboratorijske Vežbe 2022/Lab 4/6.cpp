#include <stdlib.h>
#include <stdio.h>
#include <mpich/mpi.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size, cart_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 9,
            m = 10,
            dimensions = 2,
            dims[dimensions] = { m, n },
            periods[dimensions] = { 0, 0 };
    int cart_coords[dimensions], col_rank, row_rank, row_size, res[dimensions] = { cart_coords[0], cart_coords[1] };
    
    MPI_Comm cart_comm, col_comm, row_comm;
    MPI_Cart_create(MPI_COMM_WORLD, dimensions, dims, periods, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &cart_rank);
    MPI_Cart_coords(cart_comm, cart_rank, dimensions, cart_coords);

    MPI_Comm_split(cart_comm, cart_coords[0], cart_coords[1], &col_comm);
    MPI_Comm_rank(col_comm, &col_rank);
    MPI_Comm_split(cart_comm, cart_coords[1], cart_coords[0], &row_comm);
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);
    
    MPI_Reduce(&cart_coords[1], &res[1], 1, MPI_INT, MPI_MAX, 0, col_comm);

    if(col_rank == 0){
        if(row_rank == 0){
            printf("(%d, %d)\n", res[0], res[1]);
            for(int i = 1; i < row_size; i++)
            {
                MPI_Recv(res, 2, MPI_INT, i, 0, row_comm, MPI_STATUS_IGNORE);
                printf("(%d, %d)\n", res[0], res[1]);
            }
        }
        else
            MPI_Send(res, 2, MPI_INT, 0, 0, row_comm);
    }

    MPI_Finalize();
    return 0;
}