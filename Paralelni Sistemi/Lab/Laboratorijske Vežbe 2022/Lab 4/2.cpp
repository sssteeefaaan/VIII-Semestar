#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>
using namespace std;

int main(int argc, char** argv)
{
    const int n = 10, m = 5, dimensions = 2;

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size != n * m)
        exit(-99);

    int my_rank,
        my_coords[dimensions],
        shift = 2,
        dims[dimensions] = { n, m },
        periods[dimensions] = { 0, 1 },
        ln_rank,
        rn_rank,
        ln_coords[dimensions],
        rn_coords[dimensions];

    MPI_Comm cart_comm;

    MPI_Cart_create(MPI_COMM_WORLD, dimensions, dims, periods, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &my_rank);
    MPI_Cart_coords(cart_comm, rank, dimensions, my_coords);

    MPI_Cart_shift(cart_comm, 1, shift, &ln_rank, &rn_rank);
    MPI_Cart_coords(cart_comm, ln_rank, dimensions, ln_coords);
    MPI_Cart_coords(cart_comm, rn_rank, dimensions, rn_coords);

    printf("P{%d}[%d][%d]: ({%d}[%d][%d], {%d}[%d][%d])\n", my_rank, my_coords[0], my_coords[1],
                                                        ln_rank, ln_coords[0], ln_coords[1],
                                                        rn_rank, rn_coords[0], rn_coords[1]);
    fflush(stdout);
    MPI_Finalize();
    return 0;
}