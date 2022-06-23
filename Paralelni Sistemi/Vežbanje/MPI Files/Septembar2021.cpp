#include <stdlib.h>
#include <mpi.h>
#include <iostream>

#define NUMB 9

template <typename T>
bool check_data(T* v1, T* v2, const unsigned long long n);
template <typename T>
void print_vector(T* v, const unsigned long long n, int my_rank);

int main(int argc, char** argv)
{
	int N, M, rank, size, b_size = sqrt(NUMB);
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (argc != 3 || (N = atoi(argv[1])) < 1 || (M = atoi(argv[2])) < 1 || (N % b_size) || (M % b_size) || (N * M != size * NUMB))
	{
		std::cout << "Wrong N/M parameters!" << std::endl;
		exit(-100);
	}

	MPI_File f1;
	MPI_Offset disp = sizeof(int) * NUMB;
	int* random_data = (int*)malloc(disp);
	for (int i = 0, start = rank * NUMB; i < NUMB; i++)
		random_data[i] = start + i;

	print_vector(random_data, NUMB, rank);
	MPI_Barrier(MPI_COMM_WORLD);
	if(!rank)
		std::cout << std::endl << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f1);
	MPI_File_seek(f1, (size - 1 - rank) * disp, MPI_SEEK_SET);
	MPI_File_write_all(f1, random_data, NUMB, MPI_INT, MPI_STATUS_IGNORE);
	MPI_File_close(&f1);

	MPI_File f2;
	int* read_data = (int*)malloc(disp);
	MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f2);
	MPI_File_read_at_all(f2, (size - 1 - rank) * disp, read_data, NUMB, MPI_INT, MPI_STATUS_IGNORE);
	MPI_File_close(&f2);

	std::cout << "P[" << rank << "]: " << (check_data(read_data, random_data, NUMB) ? "Correct!" : "False!") << std::endl;
	free(random_data);

	MPI_Barrier(MPI_COMM_WORLD);
	if (!rank)
		std::cout << std::endl << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	
	int gsizes[] = { N, M },
		distribs[] = { MPI_DISTRIBUTE_BLOCK, MPI_DISTRIBUTE_BLOCK },
		dargs[] = { MPI_DISTRIBUTE_DFLT_DARG, MPI_DISTRIBUTE_DFLT_DARG },
		psizes[] = { N / b_size, M / b_size };
	MPI_Datatype filetype;
	MPI_Type_create_darray(size, rank, 2, gsizes, distribs, dargs, psizes, MPI_ORDER_C , MPI_INT, &filetype);
	MPI_Type_commit(&filetype);

	// If you want to torture yourself
	/*int n = N / b_size,
		m = M / b_size,
		* bl = (int*) malloc(sizeof(int) * n * m * b_size),
		* str = (int*) malloc(sizeof(int) * n * m * b_size);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			for (int k = 0; k < b_size; k++)
			{
				bl[i * m * b_size + j * b_size + k] = 1;
				str[i * m * b_size + j * b_size + k] = (i * b_size + k) * m + j ;
			}
		}
	}

	MPI_Datatype etype, filetype;
	MPI_Type_contiguous(b_size, MPI_INT, &etype);
	MPI_Type_commit(&etype);

	MPI_Type_indexed(n * m * b_size, bl, str, etype, &filetype);
	MPI_Type_commit(&filetype);

	free(bl);
	free(str);*/

	MPI_File f3;
	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f3);
	//MPI_File_set_view(f3, ((rank / m) * M + rank % m) * b_size * sizeof(int), etype, filetype, "native", MPI_INFO_NULL);
	MPI_File_set_view(f3, 0, MPI_INT, filetype, "native", MPI_INFO_NULL);
	MPI_File_write_all(f3, read_data, NUMB, MPI_INT, MPI_STATUS_IGNORE);
	MPI_File_close(&f3);
	free(read_data);

	// Provera
	int* check_data = (int*)malloc(disp);
	MPI_File f4;
	MPI_File_open(MPI_COMM_WORLD, "file2.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &f4);
	MPI_File_read_at_all(f4, rank * disp, check_data, NUMB, MPI_INT, MPI_STATUS_IGNORE);
	MPI_File_close(&f4);

	print_vector(check_data, NUMB, rank);
	free(check_data);

	MPI_Finalize();
	return 0;
}

template <typename T>
void print_vector(T* v, const unsigned long long n, int my_rank)
{
	using namespace std;
	cout << "P[" << my_rank << "] = |\t";
	for (unsigned long long i = 0; i < n; i++)
		cout << v[i] << "\t";
	cout << "|" << endl;
}

template <typename T>
bool check_data(T* v1, T* v2, const unsigned long long n)
{
	bool ret = true;
	for (unsigned long long i = 0; ret && i < n; i++)
		ret = v1[i] == v2[i];
	return ret;
}