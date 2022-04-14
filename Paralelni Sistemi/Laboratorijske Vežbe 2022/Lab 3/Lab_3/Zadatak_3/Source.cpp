#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int n = 16, p = 4;
	int Mat[n][n], New_Mat[n][n];

	if (rank == 0)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; (Mat[i][j++] = i * n + j));

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; New_Mat[i][j++] = 0);

	MPI_Datatype temp_type, row_type, column_type;
	MPI_Type_vector(n, 1, n + 1, MPI_INT, &temp_type);
	MPI_Type_create_resized(temp_type, 0, 1, &row_type);
	MPI_Type_commit(&row_type);

	// Ne znam sta koji kurac hoce od mene
	MPI_Type_vector(n, 1, n, MPI_INT, &column_type);
	//MPI_Type_create_resized(MPI_INT, 0, n, &column_type); 
	MPI_Type_commit(&column_type);

	MPI_Scatter(&Mat[0][0], 1, row_type, &New_Mat[0][0], 1, column_type, 0, MPI_COMM_WORLD);

	printf("\n\nP[%d]\n", rank);
	for (int i = 0; i < n; i++)
	{
		printf("|\t");
		for (int j = 0; j < n; j++)
			printf("%d\t", New_Mat[i][j]);
		printf("|\n");
	}
	fflush(stdout);

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();
	return 0;
}