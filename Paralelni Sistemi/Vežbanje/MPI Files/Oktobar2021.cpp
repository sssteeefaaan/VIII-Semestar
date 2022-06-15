#include <mpi.h>
#include <stdlib.h>

#define ULL unsigned long long
#define MPI_ULL MPI_UNSIGNED_LONG_LONG

const ULL N = 5 * 256 * 8;

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Offset disp = sizeof(ULL) * N;
	ULL* data = (ULL*)malloc(disp);
	for (ULL i = 0, start = rank * N; i < N; i++)
		data[i] = start + i;

	MPI_File f_make;
	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f_make);
	MPI_File_write_at_all(f_make, rank * disp, data, N, MPI_ULL, MPI_STATUS_IGNORE);
	MPI_File_close(&f_make);
	free(data);

	MPI_File f1, f2, f3;
	MPI_Offset file_size, my_portion;
	ULL* my_data, my_amount;

	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f1);
	MPI_File_get_size(f1, &file_size);
	
	my_portion = file_size / size;
	my_amount = my_portion / sizeof(ULL);
	my_data = (ULL*)malloc(my_portion);

	MPI_File_read_at_all(f1, rank * my_portion, my_data, my_amount, MPI_ULL, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);

	// 1
	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f2);
	MPI_File_write_shared(f2, my_data, my_amount, MPI_ULL, MPI_STATUS_IGNORE);
	MPI_File_close(&f2);

	// 2
	MPI_Offset partial = my_portion / 5;
	MPI_Datatype my_type;
	MPI_Type_vector(5, partial / sizeof(ULL), size * partial / sizeof(ULL), MPI_ULL, &my_type);
	MPI_Type_commit(&my_type);

	MPI_File_open(MPI_COMM_WORLD, "file3.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f3);
	MPI_File_set_view(f3, rank * partial, MPI_ULL, my_type, "native", MPI_INFO_NULL);
	MPI_File_write_all_begin(f3, my_data, my_amount, MPI_ULL);

	ULL* new_data = (ULL*)malloc(my_portion);
	for (ULL i = 0; i < my_amount; i++)
		new_data[i] = my_data[i] * 2;
	free(new_data);

	MPI_File_write_all_end(f3, my_data, MPI_STATUS_IGNORE);
	MPI_File_close(&f3);
	free(my_data);

	MPI_Finalize();
	return 0;
}