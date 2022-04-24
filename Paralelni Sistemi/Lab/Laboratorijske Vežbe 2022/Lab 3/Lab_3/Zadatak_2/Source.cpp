#include <mpi.h>
#include <stdio.h>
#include <random>
#include <time.h>

int main(int argc, char** argv)
{
	srand(time(NULL));

	int rank, size;
	const int n = 16, p = 4;
	int A[n][n], T[n / 2][n / p];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		printf("\n\nP[%d] A matrix:\n", rank);
		for (int i = 0; i < n; i++)
		{
			printf("|\t");
			for (int j = 0; j < n; j++)
				printf("%d\t", A[i][j] = i * n + j/*rand() % (n * n)*/);
			printf("|\n");
		}
		printf("\n\n");
		fflush(stdout);
	}

	MPI_Datatype temp_send_type, new_send_type;
	MPI_Type_vector(n / 2, 1, 2 * n, MPI_INT, &temp_send_type);
	MPI_Type_create_resized(temp_send_type, 0, sizeof(int), &new_send_type);
	MPI_Type_commit(&new_send_type);

	MPI_Datatype temp_recv_type, new_recv_type;
	MPI_Type_vector(n / 2, 1, n / p, MPI_INT, &temp_recv_type);
	MPI_Type_create_resized(temp_recv_type, 0, sizeof(int), &new_recv_type);
	MPI_Type_commit(&new_recv_type);

	MPI_Scatter(&A[0][0], n / p, new_send_type, &T[0][0], n / p, new_recv_type, 0, MPI_COMM_WORLD);
	printf("P[%d]\n", rank);
	for (int i = 0; i < n / 2; i++)
	{
		printf("|\t");
		for (int j = 0; j < n / p; j++)
			printf("%d\t", T[i][j]);
		printf("|\n");
	}
	printf("\n\n");
	fflush(stdout);

	struct {
		int max;
		int rank;
	} local_max{}, max{};

	local_max = { T[0][0], rank };
	for (int i = 0; i < n / 2; i++)
		for (int j = 0; j < n / p; j++)
			local_max.max = local_max.max < T[i][j] ? T[i][j] : local_max.max;

	MPI_Reduce(&local_max, &max, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("P[%d]: Globalni maximum je %d i nalazi se u procesu %d.\n", rank, max.max, max.rank);

	MPI_Finalize();
	return 0;
}