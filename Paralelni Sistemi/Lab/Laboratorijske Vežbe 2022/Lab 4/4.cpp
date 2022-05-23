#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm cart_comm;
    int cart_rank, upper, lower, part_sum = 0, sum;
    const int dimensions = 2, 
                n = 3,
                m = 2,
                dims[dimensions] = { n, m },
                periods[dimensions] = { 1, 0 };

    MPI_Cart_create(MPI_COMM_WORLD, dimensions, dims, periods, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &cart_rank);
    MPI_Cart_shift(cart_comm, 0, 1, &upper, &lower);

    part_sum = upper + lower;
    MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(!rank)
        printf("Sum is %d\n", sum);
    
    MPI_Finalize();
    return 0;
}