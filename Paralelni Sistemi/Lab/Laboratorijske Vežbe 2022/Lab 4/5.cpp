#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>

int main(int argc, char** argv)
{
    int rank, size, cart_rank, ul_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    MPI_Comm cart_comm;
    const int n = 3,
            dimensions = 2,
            dims[dimensions] = { n, n },
            periods[dimensions] = { 0, 0 };
    int cart_coords[dimensions], sum;

    if(size != n * n)
        exit(-100);

    MPI_Cart_create(MPI_COMM_WORLD, dimensions, dims, periods, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &cart_rank);
    MPI_Cart_coords(cart_comm, cart_rank, dimensions, cart_coords);
    
    MPI_Comm upper_lower_matrix;
    MPI_Comm_split(cart_comm, cart_coords[0] + cart_coords[1] < n, cart_rank, &upper_lower_matrix);
    MPI_Comm_rank(upper_lower_matrix, &ul_rank);
    MPI_Reduce(&cart_rank, &sum, 1, MPI_INT, MPI_SUM, 0, upper_lower_matrix);

    struct {
        int rank;
        int value;
    } send = { cart_rank, rank }, recv{};

    MPI_Reduce(&send, &recv, 1, MPI_2INT, MPI_MINLOC, 0, cart_comm);
    MPI_Bcast(&recv, 1, MPI_2INT, 0, cart_comm);
    
    MPI_Request req;
    if(!ul_rank)
        MPI_Isend(&sum, 1, MPI_INT, 0, 0, cart_comm, &req);
    if(!rank){
        int res_sum;
        MPI_Recv(&res_sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, cart_comm, MPI_STATUS_IGNORE);
        printf("P[%d]: Lower sum is %d.\n", rank, res_sum);
        MPI_Recv(&res_sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, cart_comm, MPI_STATUS_IGNORE);
        printf("P[%d]: Upper sum is %d.\n", rank, res_sum);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}