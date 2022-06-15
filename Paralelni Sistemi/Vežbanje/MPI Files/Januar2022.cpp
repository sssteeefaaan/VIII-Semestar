#include <mpi.h>
#include <stdlib.h>

#define ULL unsigned long long
#define MPI_ULL MPI_UNSIGNED_LONG_LONG

const ULL FILE_SIZE = 10 * 1024 * 1024;

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Write/Create 10MB file
	{
		ULL my_size = FILE_SIZE / size,
			numb = my_size / sizeof(ULL),
			* data = (ULL*)malloc(my_size);
		for (ULL i = 0, start = rank * numb; i < numb; i++)
			data[i] = start + i;

		MPI_File fp;
		MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
		MPI_File_write_at_all(fp, rank * my_size, data, numb, MPI_ULL, MPI_STATUS_IGNORE);
		MPI_File_close(&fp);
		free(data);
	}

	MPI_File f1, f2;
	MPI_Offset file_size, my_portion;
	ULL* file_data = nullptr, n;

	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f1);
	MPI_File_get_size(f1, &file_size);
	my_portion = file_size / size;
	n = my_portion / sizeof(ULL);
	file_data = (ULL*) malloc(my_portion);
	MPI_File_read_shared(f1, file_data, n, MPI_ULL, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);

	MPI_Datatype my_type;
	MPI_Type_vector(2, n / 2, size * n / 2, MPI_ULL, &my_type);
	MPI_Type_commit(&my_type);

	MPI_Offset segment = my_portion / 2;
	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f2);
	MPI_File_set_view(f2, rank * segment, MPI_ULL, my_type, "native", MPI_INFO_NULL);
	MPI_File_write_all(f2, file_data, n, MPI_LONG_LONG, MPI_STATUS_IGNORE);
	MPI_File_close(&f2);

	free(file_data);

	MPI_Finalize();
	return 0;
}