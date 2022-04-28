#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	printf("Hey\n");
	fflush(stdout);

	int rank, size;
	const int n = 10;

	int A[n][n], T[n][n],
		len_ut[n], disp_ut[n],
		len_lt[n], disp_lt[n];

	MPI_Datatype mpi_ut, mpi_lt;
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = rank;
		}
		T[i][i] = rank;
	}

	for (int i = 0; i < n; i++)
	{
		len_ut[i] = n - (i + 1);
		disp_ut[i] = i * (n + 1) + 1;

		len_lt[i] = i;
		disp_lt[i] = i * n;
	}
	MPI_Type_indexed(n, &len_ut[0], &disp_ut[0], MPI_INT, &mpi_ut);
	MPI_Type_commit(&mpi_ut);

	MPI_Type_indexed(n, &len_lt[0], &disp_lt[0], MPI_INT, &mpi_lt);
	MPI_Type_commit(&mpi_lt);

	if (rank != 1)
		MPI_Send(&A[0][0], 1, mpi_ut, 1, 0, MPI_COMM_WORLD);
	else if (rank == 1)
	{
		MPI_Recv(&T[0][0], 1, mpi_lt, 0, 0, MPI_COMM_WORLD, &stat);
		MPI_Recv(&T[0][0], 1, mpi_ut, 2, 0, MPI_COMM_WORLD, &stat);
		printf("P[%d]:\n", rank);
		for (int i = 0; i < n; i++)
		{
			printf("|\t");
			for (int j = 0; j < n; j++)
				printf("%d\t", T[i][j]);
			printf("|\n");
		}
	}

	MPI_Finalize();
	return 0;
}