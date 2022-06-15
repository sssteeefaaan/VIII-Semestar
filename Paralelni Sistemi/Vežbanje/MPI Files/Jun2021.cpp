#include <mpi.h>
#include <stdlib.h>
#include <iostream>

#define NUMB 15
#define TYPE int
#define MPI_TYPE MPI_INT
#define ULL unsigned long long

template <typename T>
bool check_outcome(const T* v1, const T* v2, const ULL n);

template <typename T>
void print_vector(const T* v, const ULL n, int my_rank);


int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Offset disp = sizeof(TYPE) * NUMB;
	TYPE* data = (TYPE*) malloc(disp);
	for (ULL i = 0, start = rank * NUMB; i < NUMB; i++)
		data[i] = start + i;

	MPI_File f1;
	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f1);
	MPI_File_seek(f1, (size - 1 - rank) * disp, MPI_SEEK_SET);
	MPI_File_write_all(f1, data, NUMB, MPI_TYPE, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);

	TYPE* read_data = (TYPE*) malloc(disp);
	MPI_File f2;
	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f2);
	MPI_File_read_at_all(f2, (size - 1 - rank) * disp, read_data, NUMB, MPI_TYPE, MPI_STATUS_IGNORE);
	MPI_File_close(&f2);

	std::cout << "P[" << rank << "]: " << (check_outcome(data, read_data, NUMB) ? "Correct!" : "False!") << std::endl;
	free(data);

	MPI_Barrier(MPI_COMM_WORLD);
	if (!rank)
		std::cout << std::endl << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);
	print_vector(read_data, NUMB, rank);

	int n = (sqrt(1 + 8 * NUMB) - 1) / 2,
		* bl = (int*) malloc(sizeof(int) * n),
		* str = (int*) malloc(sizeof(int) * n);

	bl[0] = 1;
	str[0] = 0;

	for (int i = 1; i < n; i++)
	{
		bl[i] = i + 1;
		str[i] = str[i - 1] + size * i + rank;
	}

	MPI_Datatype filetype;
	MPI_Type_indexed(n, bl, str, MPI_TYPE, &filetype);
	MPI_Type_commit(&filetype);

	free(bl);
	free(str);

	MPI_File f3;
	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f3);
	MPI_File_set_view(f3, rank * sizeof(TYPE), MPI_TYPE, filetype, "native", MPI_INFO_NULL);
	MPI_File_write_all(f3, read_data, NUMB, MPI_TYPE, MPI_STATUS_IGNORE);
	MPI_File_close(&f3);
	
	free(read_data);

	// Provera
	TYPE* check_data = (TYPE*)malloc(disp);

	MPI_File f_check;
	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f_check);
	MPI_File_read_at_all(f_check, rank * disp, check_data, NUMB, MPI_TYPE, MPI_STATUS_IGNORE);
	MPI_File_close(&f_check);

	MPI_Barrier(MPI_COMM_WORLD);
	if (!rank)
		std::cout << std::endl << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);
	print_vector(check_data, NUMB, rank);
	free(check_data);

	MPI_Finalize();
	return 0;
}

template <typename T>
bool check_outcome(const T* v1, const T* v2, const ULL n)
{
	bool r = true;
	for (ULL i = 0; r && i < n; i++)
		r = v1[i] == v2[i];
	return r;
}

template <typename T>
void print_vector(const T* v, const ULL n, int my_rank)
{
	using namespace std;
	cout << "P[" << my_rank << "] = |\t";
	for (ULL i = 0; i < n; i++)
		cout << v[i] << "\t";
	cout << "|" << endl;
}