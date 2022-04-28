#include <mpi.h>
#include <iostream>
using namespace std;

bool check_result(int* A, int* b, int* c, int n);

int main(int argc, char** argv)
{
	int rank, size, col_rank, row_rank;

	const int n = 1 << 3, p = 1 << 4, q = sqrt(p), s = n / q;
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size != p)
		exit(-100);

	MPI_Comm col_comm, row_comm;
	MPI_Comm_split(MPI_COMM_WORLD, rank % q, rank / q, &col_comm);
	MPI_Comm_split(MPI_COMM_WORLD, rank / q, rank % q, &row_comm);

	MPI_Comm_rank(col_comm, &col_rank);
	MPI_Comm_rank(row_comm, &row_rank);

	int A[n][n], b[n], c[n],
		* local_A, * local_b, * local_part_c, * local_c;

	local_A = (int*)malloc(sizeof(int) * s * s);
	local_b = (int*)malloc(sizeof(int) * s);
	local_part_c = (int*)malloc(sizeof(int) * s);
	local_c = (int*)malloc(sizeof(int) * s);

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				A[i][j] = i * n + j;
			b[i] = i;
		}
	}

	MPI_Datatype nt;
	MPI_Type_vector(s, 1, q, MPI_INT, &nt);
	MPI_Type_create_resized(nt, 0, sizeof(int), &nt);
	MPI_Type_commit(&nt);

	if (col_rank == 0)
		MPI_Scatter(&b[0], 1, nt, local_b, s, MPI_INT, 0, row_comm);

	MPI_Bcast(local_b, s, MPI_INT, 0, col_comm);

#pragma region a)

	/*MPI_Datatype mnt;
	MPI_Type_vector(s, 1, n, nt, &mnt);
	MPI_Type_create_resized(mnt, 0, sizeof(int), &mnt);
	MPI_Type_commit(&mnt);
	
	if (rank == 0)
	{
		for (int i = 0; i < s; i++)
		{
			for (int j = 0; j < s; j++)
				local_A[i * s + j] = A[i][j * q];
		}

		for (int i = 1; i < p; i++)
			MPI_Send(&A[i / q * s][i % q], 1, mnt, i, 0, MPI_COMM_WORLD);
	}
	else
		MPI_Recv(local_A, s * s, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);*/

#pragma endregion

#pragma region b)

	MPI_Datatype mnt;
	MPI_Type_vector(s, n, n, MPI_INT, &mnt);
	MPI_Type_create_resized(mnt, 0, sizeof(int) * n * s, &mnt);
	MPI_Type_commit(&mnt);

	int* local_part_A = (int*)malloc(sizeof(int) * n * s);
	if (row_rank == 0)
		MPI_Scatter(&A[0][0], 1, mnt, local_part_A, s * n, MPI_INT, 0, col_comm);

	MPI_Type_vector(s, 1, n, nt, &mnt);
	MPI_Type_create_resized(mnt, 0, sizeof(int), &mnt);
	MPI_Type_commit(&mnt);

	MPI_Scatter(local_part_A, 1, mnt, local_A, s * s, MPI_INT, 0, row_comm);
	free(local_part_A);

#pragma endregion

	struct {
		int min;
		int rank;
	}local_min = { INT16_MAX, rank }, global_min{};
	for (int i = 0; i < s; i++)
	{
		local_part_c[i] = 0;
		for (int k = 0; k < s; k++)
		{
			local_part_c[i] += local_A[i * s + k] * local_b[k];
			if (local_min.min > local_A[i * s + k])
				local_min.min = local_A[i * s + k];
		}
	}

	MPI_Reduce(&local_min, &global_min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&global_min, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(local_part_c, local_c, s, MPI_INT, MPI_SUM, global_min.rank % q, row_comm);

	if (global_min.rank % q == row_rank)
	{
		MPI_Gather(local_c, s, MPI_INT, c, s, MPI_INT, global_min.rank / q, col_comm);
		if (rank == global_min.rank)
		{
			MPI_Request req;
			cout << "\n\nP[" << col_rank << ", " << row_rank << "] I have the minimum: " << global_min.min << endl;
			MPI_Isend(c, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
		}
	}

	if (rank == 0)
	{
		MPI_Recv(c, n, MPI_INT, global_min.rank, 0, MPI_COMM_WORLD, &stat);
		if (check_result(&A[0][0], b, c, n))
			cout << "\n\nCorrect!" << endl;
		else
			cout << "\n\nFalse!" << endl;
	}

	free(local_part_c);
	free(local_c);
	free(local_b);
	free(local_A);
	
	MPI_Finalize();

	return 0;
}

bool check_result(int* A, int* b, int* c, int n)
{
	bool ret = true;
	for (int i = 0; ret&& i < n; i++)
	{
		int temp = 0;
		for (int j = 0; ret && j < n; j++)
			temp += A[i * n + j] * b[j];
		ret = c[i] == temp;
	}
	return ret;
}