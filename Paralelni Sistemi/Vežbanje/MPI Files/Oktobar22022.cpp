#include <mpi.h>
#include <stdlib.h>

#define ULL long long
#define MPI_ULL MPI_LONG_LONG

const ULL N = 16;

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Offset disp = sizeof(ULL) * N;
	ULL* data = (ULL*) malloc(disp);
	for (ULL i = 0, start = rank * N; i < N; i++)
		data[i] = start + i;

	MPI_File f1;
	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f1);
	MPI_File_set_view(f1, (size - 1 - rank) * disp, MPI_ULL, MPI_ULL, "native", MPI_INFO_NULL);
	MPI_File_write_all(f1, data, N, MPI_ULL, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);
	free(data);

	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f1);
	MPI_Offset file_size, my_size;
	MPI_File_get_size(f1, &file_size);
	my_size = file_size / size;
	ULL n = my_size / sizeof(ULL);
	data = (ULL*)malloc(my_size);
	MPI_File_set_view(f1, rank * my_size, MPI_ULL, MPI_ULL, "native", MPI_INFO_NULL);
	MPI_File_read_all_begin(f1, data, n, MPI_ULL);

	MPI_Datatype my_type;
	MPI_Type_vector(n, 1, size, MPI_ULL, &my_type);
	MPI_Type_commit(&my_type);

	MPI_File f2;
	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f2);
	MPI_File_set_view(f2, rank * sizeof(ULL), MPI_ULL, my_type, "native", MPI_INFO_NULL);
	
	MPI_File_read_all_end(f1, data, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);

	MPI_File_write_all(f2, data, n, MPI_ULL, MPI_STATUS_IGNORE);
	MPI_File_close(&f2);
	free(data);

	MPI_Finalize();
	return  0;
}