#include <mpi.h>
#include <stdio.h>


int main(int argc, char** argv)
{
	int rank, size, cart_rank, cart_coords[2];
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Comm cart_comm;
	int dims[] = { 5, 3 },
		periods[] = { 1, 0 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);

	MPI_Comm_rank(cart_comm, &cart_rank);
	MPI_Cart_coords(cart_comm, cart_rank, 2, cart_coords);

	printf("P[%d] {%d} (%d, %d)\n", rank, cart_rank, cart_coords[0], cart_coords[1]);

	MPI_Finalize();
	return 0;
}