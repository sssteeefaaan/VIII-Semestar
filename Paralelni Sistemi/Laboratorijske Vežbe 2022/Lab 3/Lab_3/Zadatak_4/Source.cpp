#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int n = 8, p = 4;

	int Mat[n][n], New_Mat[n][n];
	MPI_Status stat;
	MPI_Request req;

	if (rank == 0)
	{
		printf("\n\nInitial matrix:\n");
		for (int i = 0; i < n; i++)
		{
			printf("|\t");
			for (int j = 0; j < n; printf("%d\t", Mat[i][j++] = i * n + j));
			printf("|\n");
		}
		fflush(stdout);
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; New_Mat[i][j++] = 0);

	MPI_Datatype send_type;
	MPI_Type_create_resized(MPI_INT, 0, (n - 1) * sizeof(int), &send_type);
	MPI_Type_commit(&send_type);

	if (rank == 0)
	{
		for (int i = 0; i < p; i++)
		{
			MPI_Isend(&Mat[0][n - 1 - i], n - i, send_type, i, 0, MPI_COMM_WORLD, &req);
			MPI_Isend(&Mat[i][n - 1], n - i, send_type, i, 0, MPI_COMM_WORLD, &req);
		}
	}

	MPI_Recv(&New_Mat[0][0], (n - rank), MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
	MPI_Recv(&New_Mat[1][0], (n - rank), MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

	printf("\n\nP[%d]:\n", rank);
	for (int i = 0; i < n; i++)
	{
		printf("|\t");
		for (int j = 0; j < n; printf("%d\t", New_Mat[i][j++]));
		printf("|\n");
	}
	fflush(stdout);

	MPI_Finalize();
	return 0;
}