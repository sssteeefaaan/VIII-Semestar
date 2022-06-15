#include <mpi.h>
#include <stdlib.h>
#include <iostream>

#define ULL unsigned long long

const ULL N = 16;

template <typename T>
void print_vector(T* vector, const ULL N, const int my_rank)
{
	using namespace std;
	cout << "P[" << my_rank << "] = |\t";
	for (ULL i = 0; i < N; i++)
		cout << vector[i] << "\t";
	cout << "|" << std::endl;
	// cout << "|\n";
	// fflush(stdout);
}

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Offset disp = sizeof(char) * N; // sizeof(char) == 1, al kao aj da budemo korektni
	char* data = (char*) malloc(disp);
	for (ULL i = 0, start = rank * N; i < N; i++)
		data[i] = (start + i) % 93 + 33;

	// 1
	MPI_File f1;
	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f1);
	MPI_File_seek(f1, (size - 1 - rank) * disp, MPI_SEEK_SET);
	MPI_File_write_all(f1, data, N, MPI_CHAR, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);

	print_vector(data, N, rank);
	free(data);

	MPI_Barrier(MPI_COMM_WORLD);
	if (!rank)
		std::cout << std::endl << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	// 2
	char* read_data = (char*) malloc(disp);

	MPI_File f2;
	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f2);
	MPI_File_read_shared(f2, read_data, N, MPI_CHAR, MPI_STATUS_IGNORE);
	MPI_File_close(&f2);

	print_vector(read_data, N, rank);
	//free(read_data);

	// 3
	MPI_Datatype my_type;
	MPI_Type_vector(N, 1, size, MPI_CHAR, &my_type);
	MPI_Type_commit(&my_type);

	MPI_File f3;
	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f3);
	MPI_File_set_view(f3, rank * sizeof(char), MPI_CHAR, my_type, "native", MPI_INFO_NULL);
	MPI_File_write_all(f3, read_data, N, MPI_CHAR, MPI_STATUS_IGNORE);
	MPI_File_close(&f3);

	free(read_data);

	MPI_Finalize();
	return  0;
}