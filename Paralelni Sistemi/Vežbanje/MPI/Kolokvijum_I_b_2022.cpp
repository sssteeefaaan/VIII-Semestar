#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv)
{
	int rank, p, diag_rank, s_data, r_data, q, col;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	q = sqrt(p), col = (rank / q == rank % q) ? 0 : 1;

	MPI_Comm diag_comm;
	MPI_Comm_split(MPI_COMM_WORLD, col, rank / q, &diag_comm);

	if (rank % q == rank / q)
	{
		MPI_Comm_rank(diag_comm, &diag_rank);
		if (diag_rank == 0)
		{
			r_data = s_data = 2904;
			for (int i = 1; i < q; i++)
				MPI_Send(&s_data, 1, MPI_INT, i, 0, diag_comm);
		}
		else
		{
			MPI_Status stat;
			MPI_Recv(&r_data, 1, MPI_INT, 0, 0, diag_comm, &stat);
		}

		printf("P[%d] %d\n", rank, r_data);
		fflush(stdout);
	}

	MPI_Finalize();
	return 0;
}