#include <mpi.h>
#include <stdlib.h>
#include <iostream>

#define ULL unsigned long long
#define TYPE char
#define MPI_TYPE MPI_CHAR


template <typename T, typename K>
std::ostream& print_vector(int rank, const char* lbl, T* v, const K N, std::ostream& o)
{
	o << "P[" << rank << "]: " << lbl << " = |\t";
	for (K i = 0; i < N; i++)
		o << v[i] << "\t";
	o << "|" << std::endl;
	return o;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	ULL N = 1 << 5;
	if (argc > 1)
		N = atoll(argv[1]);

	if (N != size)
		exit(-100);

	// 1
	MPI_Offset disp = sizeof(TYPE) * N;
	TYPE* data = (TYPE*) malloc(disp);
	for (ULL i = 0; i < N; i++)
		data[i] = ((i + rank) % 94) + 33;

	MPI_File f1;
	MPI_File_open(MPI_COMM_WORLD, "prva.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f1);
	MPI_File_write_at_all(f1, rank * disp, data, N, MPI_TYPE, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);
	free(data);
	//

	// 2
	TYPE* read_data;
	ULL read_disp, read_numb;
	MPI_Offset file_size;

	MPI_File f2;
	MPI_File_open(MPI_COMM_WORLD, "prva.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f2);
	MPI_File_get_size(f2, &file_size);
	read_disp = file_size / size;
	read_numb = read_disp / sizeof(TYPE);
	read_data = (TYPE*) malloc(read_disp);
	MPI_File_read_shared(f2, read_data, read_numb, MPI_TYPE, MPI_STATUS_IGNORE);
	MPI_File_close(&f2);
	//

	print_vector(rank, "read_data", read_data, read_numb, std::cout);
	MPI_Barrier(MPI_COMM_WORLD);
	std::cout << std::endl << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	// 3
	TYPE* write_data = (TYPE*)malloc(2 * read_disp);
	for (ULL i = 0; i < N; i++)
		write_data[i] = write_data[i + N] = read_data[i];
	free(read_data);

	MPI_Datatype file_type;
	MPI_Type_vector(N, 1, 2 * N, MPI_TYPE, &file_type);
	MPI_Type_create_resized(file_type, 0, sizeof(TYPE), &file_type);
	MPI_Type_contiguous(2, file_type, &file_type);
	MPI_Type_commit(&file_type);

	MPI_File f3;
	MPI_File_open(MPI_COMM_WORLD, "druga.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f3);
	MPI_File_set_view(f3, sizeof(TYPE) * 2 * rank, MPI_TYPE, file_type, "native", MPI_INFO_NULL);
	MPI_File_write_all(f3, write_data, 2 * N, MPI_TYPE, MPI_STATUS_IGNORE);
	MPI_File_close(&f3);
	free(write_data);
	//

	// Provera
	read_data = (TYPE*) malloc(2 * read_disp);
	MPI_File f4;
	MPI_File_open(MPI_COMM_WORLD, "druga.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f4);
	MPI_File_read_at_all(f4, sizeof(TYPE) * 2 * N * rank, read_data, 2 * N, MPI_TYPE, MPI_STATUS_IGNORE);
	MPI_File_close(&f4);
	print_vector(rank, "read_data_2", read_data, 2 * N, std::cout);
	free(read_data);
	//

	MPI_Finalize();
	return 0;
}