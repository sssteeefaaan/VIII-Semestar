#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

const long long FILE_SIZE = 10 * 1024 * 1024;

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Write/Create 10MB file
	
	/*MPI_File fp;
	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
	long long my_size = FILE_SIZE / size,
		numb = my_size / sizeof(long long),
		* data = (long long*)malloc(my_size);
	for (long long i = 0; i < numb; i++)
		data[i] = i + rank * numb;
	MPI_File_set_view(fp, rank * my_size, MPI_LONG_LONG, MPI_LONG_LONG, "native", MPI_INFO_NULL);
	MPI_File_write_all(fp, data, numb, MPI_LONG_LONG, MPI_STATUS_IGNORE);
	free(data);
	MPI_File_close(&fp);*/

	MPI_File f1, f2;
	MPI_Offset file_size, my_portion, segment;
	long long* file_data = nullptr, n;

	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f1);
	MPI_File_get_size(f1, &file_size);
	my_portion = file_size / size;
	segment = my_portion / 2;
	n = my_portion / sizeof(long long);
	MPI_File_set_view(f1, rank * my_portion, MPI_LONG_LONG, MPI_LONG_LONG, "native", MPI_INFO_NULL);
	file_data = (long long*) malloc(my_portion);
	MPI_File_read_all_begin(f1, file_data, n, MPI_LONG_LONG);

	MPI_Datatype my_type;
	MPI_Type_vector(2, n / 2, size * n / 2, MPI_LONG_LONG, &my_type);
	MPI_Type_commit(&my_type);

	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f2);
	MPI_File_set_view(f2, rank * segment, MPI_LONG_LONG, my_type, "native", MPI_INFO_NULL);
	MPI_File_read_all_end(f1, file_data, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);

	MPI_File_write_all(f2, file_data, n, MPI_LONG_LONG, MPI_STATUS_IGNORE);
	free(file_data);
	MPI_File_close(&f2);

	MPI_Finalize();
	return 0;
}