//#include <mpi.h>
//#include <stdio.h>
//
//int main(int argc, char** argv)
//{
//	int rank, size;
//	const int n = 10;
//	int A[n][n], T[n][n], b_len[n], disp[n];
//
//	MPI_Datatype mpi_nt;
//	MPI_Status stat;
//
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//	if (!rank)
//		for (int i = 0; i < n; i++)
//			for (int j = 0; j < n; j++)
//				A[i][j] = n * i + j + 1;
//	else
//		for (int i = 0; i < n; i++)
//			for (int j = 0; j < n; j++)
//				T[i][j] = rank;
//
//	for (int i = 0; i < n; i++)
//	{
//		b_len[i] = n - i;
//		disp[i] = i * (n + 1);
//	}
//	MPI_Type_indexed(n, &b_len[0], &disp[0], MPI_INT, &mpi_nt);
//	MPI_Type_commit(&mpi_nt);
//
//	if(!rank)
//		MPI_Send(&A[0][0], 1, mpi_nt, 1, 0, MPI_COMM_WORLD);
//	else if (rank == 1)
//	{
//		MPI_Recv(&T[0][0], 1, mpi_nt, 0, 0, MPI_COMM_WORLD, &stat);
//		printf("P[%d]:\n", rank);
//		for (int i = 0; i < n; i++)
//		{
//			printf("|\t");
//			for (int j = 0; j < n; j++)
//				printf("%d\t", T[i][j]);
//			printf("|\n");
//		}
//	}
//
//	MPI_Finalize();
//	return 0;
//}