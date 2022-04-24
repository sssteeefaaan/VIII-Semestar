#include <mpi.h>
#include <stdio.h>
#include <random>
#include <time.h>

int main(int argc, char** argv)
{
	srand(time(NULL));

	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int n = 5;
	int A[n][n], B[n][n], local_A[n], local_C[n], C[n][n], max[n];

	if (rank == 0)
	{
		printf("\nA:\n");
		for (int i = 0; i < n; i++)
		{
			printf("|\t");
			for (int j = 0; j < n; printf("%d\t", (A[i][j++] = rand() %(n * n))));
			printf("|\n");
		}

		printf("\nB:\n");
		for (int i = 0; i < n; i++)
		{
			printf("|\t");
			for (int j = 0; j < n; printf("%d\t", (B[i][j++] = rand() % (n * n))));
			printf("|\n");
		}
	}

	MPI_Scatter(&A[0][0], n, MPI_INT, &local_A[0], n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&B[0][0], n * n, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++)
	{
		local_C[i] = 0;
		for (int j = 0; j < n; j++)
			local_C[i] += local_A[j] * B[j][i];
	}

	MPI_Gather(&local_C[0], n, MPI_INT, &C[0][0], n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(&local_C[0], &max[0], n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("\nC:\n");
		for (int i = 0; i < n; i++)
		{
			printf("|\t");
			for (int j = 0; j < n; printf("%d\t", C[i][j++]));
			printf("|\n");
		}

		printf("\nMax in each column:\n|\t");
		for (int i = 0; i < n; i++)
			printf("%d\t", max[i]);
		printf("|\n");
	}

	MPI_Finalize();
	return 0;
}