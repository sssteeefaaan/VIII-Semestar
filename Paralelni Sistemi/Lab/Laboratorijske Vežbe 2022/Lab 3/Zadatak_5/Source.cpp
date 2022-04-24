#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int n = 8, m = 4, k = 12, p = 4;
	int A[m][n],
		B[n][k], C[m][k],
		local_B[n][k / p], local_C[m][k / p];

	if (rank == 0)
	{
		printf("\nA:\n");
		for (int i = 0; i < m; i++)
		{
			printf("|\t");
			for (int j = 0; j < n; printf("%d\t", (A[i][j++] = i * n + j)));
			printf("|\n");
		}

		printf("\nB:\n");
		for (int i = 0; i < n; i++)
		{
			printf("|\t");
			for (int j = 0; j < k; printf("%d\t", (B[i][j++] = 1/*i * k + j*/)));
			printf("|\n");
		}
	}

	MPI_Bcast(&A[0][0], m * n, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Datatype temp, column_send, column_recv;
	MPI_Type_vector(n, k / p, k, MPI_INT, &temp);
	MPI_Type_create_resized(temp, 0, (k / p) * sizeof(int), &column_send);
	MPI_Type_commit(&column_send);

	MPI_Scatter(&B[0][0], 1, column_send, &local_B[0][0], n * (k / p), MPI_INT, 0, MPI_COMM_WORLD);

	for (int u = 0; u < m; u++)
	{
		for (int v = 0; v < k / p; v++)
		{
			local_C[u][v] = 0;
			for (int w = 0; w < n; w++)
				local_C[u][v] += A[u][w] * local_B[w][v];
		}
	}

	MPI_Type_vector(m, k / p, k, MPI_INT, &temp);
	MPI_Type_create_resized(temp, 0, (k / p) * sizeof(int), &column_recv);
	MPI_Type_commit(&column_recv);

	MPI_Gather(&local_C[0][0], m * (k / p), MPI_INT, &C[0][0], 1, column_recv, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("\nC:\n");
		for (int i = 0; i < m; i++)
		{
			printf("|\t");
			for (int j = 0; j < k; j++)
				printf("%d\t", C[i][j]);
			printf("|\n");
		}
	}

	MPI_Finalize();
	return 0;
}