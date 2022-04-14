#include <mpi.h>
#include <stdio.h>
#include <random>
#include <time.h>

int main(int argc, char** argv)
{
	srand(time(NULL));

	int rank, size;
	const int n = 16, p = 4;
	int A[n][n], T[n / p][n / 2];

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

	MPI_Datatype temp_type, send_type;
	MPI_Type_vector(n / 2, 1, 2, MPI_INT, &temp_type);
	MPI_Type_create_resized(temp_type, 0, n * sizeof(int), &send_type);
	MPI_Type_commit(&send_type);

	MPI_Scatter(&A[0][0], n / p, send_type, &T[0][0], (n / p) * (n / 2), MPI_INT, 0, MPI_COMM_WORLD);
	
	printf("P[%d]\n", rank);
	for (int i = 0; i < n / p; i++)
	{
		printf("|\t");
		for (int j = 0; j < n / 2; j++)
			printf("%d\t", T[i][j]);
		printf("|\n");
	}
	printf("\n\n");
	fflush(stdout);

	struct {
		int min;
		int rank;
	} local_min{}, min{};

	local_min = { T[0][0], rank };
	for (int i = 0; i < n / p; i++)
		for (int j = 0; j < n / 2; j++)
			local_min.min = local_min.min > T[i][j] ? T[i][j] : local_min.min;

	MPI_Reduce(&local_min, &min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("P[%d]: Globalni minimum je %d i nalazi se u procesu %d.\n", rank, min.min, min.rank);

	MPI_Finalize();
	return 0;
}