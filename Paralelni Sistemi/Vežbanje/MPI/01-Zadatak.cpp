//#include <mpi.h>
//#include <stdio.h>
//
//#define N 8
//#define M 16
//
//int main(int argc, char** argv)
//{
//	int rank, size,
//		Mat[N][M], linear[N];
//
//	MPI_Datatype mpi_column, mpi_column2;
//
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//	if (rank == 0)
//		for (int i = 0; i < N; i++)
//			for (int j = 0; j < M; j++)
//				Mat[i][j] = i * M + j + 1;
//
//	MPI_Type_vector(N, 1, M, MPI_INT, &mpi_column);
//	MPI_Type_create_resized(mpi_column, 0, sizeof(int), &mpi_column2);
//	MPI_Type_commit(&mpi_column2);
//
//	MPI_Scatter(&Mat[0][0], 1, mpi_column2, &linear[0], N, MPI_INT, 0, MPI_COMM_WORLD);
//
//
//	printf("P[%d]:\t", rank);
//	for (int i = 0; i < N; i++)
//		printf("%d\t", linear[i]);
//	printf("\n");
//	fflush(stdout);
//
//	MPI_Finalize();
//	return 0;
//}