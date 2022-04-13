//#include <mpi.h>
//#include <stdio.h>
//#include <string>
//
//#define N 32
//
//struct str {
//	int count;
//	char chars[N];
//};
//
//int main(int argc, char** argv)
//{
//	int rank, size, b_len[2], types[2];
//	MPI_Aint disp[2];
//	MPI_Datatype new_type, mpi_string;
//	str temp{};
//
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//	MPI_Type_contiguous(N, MPI_CHAR, &mpi_string);
//	MPI_Type_commit(&mpi_string);
//
//	types[0] = MPI_INT;
//	types[1] = mpi_string;
//
//	MPI_Get_address(&temp.count, &disp[0]);
//	MPI_Get_address(&temp.chars, &disp[1]);
//	disp[1] -= disp[0];
//	disp[0] = 0;
//
//	b_len[0] = b_len[1] = 1;
//
//	MPI_Type_create_struct(2, b_len, disp, types, &new_type);
//	MPI_Type_commit(&new_type);
//
//	if (rank == 0)
//	{
//		gets_s<N>(temp.chars);
//		temp.count = strlen(temp.chars);
//	}
//
//	MPI_Bcast(&temp, 1, new_type, 0, MPI_COMM_WORLD);
//
//	printf("P[%d]: Hello world! strlen(%s)=%d\n", rank, temp.chars, temp.count);
//	fflush(stdout);
//
//	MPI_Finalize();
//	return 0;
//}