#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
using namespace std;

bool check_result(int* res, int* A, int* b, int n);
void print_matrix(const char* lbl, int* M, int n, int m);
void print_vector(const char* lbl, int* v, int n);

int main(int argc, char** argv)
{
	int rank, size;
	const int m = 1 << 3,
		p = 1 << 4,
		q = sqrt(p),
		s = m / q;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size != p)
		exit(-100);

	MPI_Comm row_comm, col_comm;
	int row_rank, col_rank;

	MPI_Comm_split(MPI_COMM_WORLD, rank % q, 0, &col_comm);
	MPI_Comm_split(MPI_COMM_WORLD, rank / q, 0, &row_comm);

	MPI_Comm_rank(col_comm, &col_rank);
	MPI_Comm_rank(row_comm, &row_rank);

	int A[m][m], b[m], c[m], * local_A, * local_b, * local_part_c, * local_c;
	local_A = (int*)malloc(sizeof(int) * s * s);
	local_b = (int*)malloc(sizeof(int) * s);
	local_part_c = (int*)malloc(sizeof(int) * s);
	local_c = (int*)malloc(sizeof(int) * s);

	/* // Ovako nece moci scatter, za njega mora indexed
	MPI_Datatype tmp, nt;
	MPI_Type_vector(s, 1, q, MPI_INT, &tmp);
	MPI_Type_create_resized(tmp, 0, m * sizeof(int), &tmp);
	MPI_Type_vector(s, 1, q, tmp, &tmp);
	MPI_Type_create_resized(tmp, 0, sizeof(int), &nt);
	MPI_Type_commit(&nt);*/

	MPI_Datatype nt;
	int* strides = (int*)malloc(sizeof(int) * s * s);
	int* block_lens = (int*)malloc(sizeof(int) * s * s);
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			block_lens[i * s + j] = 1;
			strides[i * s + j] = i * (q * m) + j * q;
		}
	}

	MPI_Type_indexed(s * s, block_lens, strides, MPI_INT, &nt);
	// Ne mora ako se ne salje vise od jedan, odnosno ne koristi Scatter
	//MPI_Type_create_resized(nt, 0, sizeof(int), &nt);
	MPI_Type_commit(&nt);

	free(block_lens);
	free(strides);


	MPI_Status stat;

	if (rank == 0)
	{
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < m; j++)
				A[i][j] = i * m + j;
			b[i] = i;
		}

		print_matrix("A", &A[0][0], m, m);
		print_vector("b", b, m);

		for (int i = 0; i < s; i++)
			for (int j = 0; j < s; j++)
				local_A[i * s + j] = A[i * q][j * q];
		for (int i = 1; i < size; i++)
			MPI_Send(&A[i / q][i % q], 1, nt, i, 0, MPI_COMM_WORLD);
	}
	else
		MPI_Recv(local_A, s * s, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

	if (col_rank == 0)
	{
		MPI_Type_vector(s, 1, q, MPI_INT, &nt);
		MPI_Type_create_resized(nt, 0, sizeof(int), &nt);
		MPI_Type_commit(&nt);

		MPI_Scatter(b, 1, nt, local_b, s, MPI_INT, 0, row_comm);
	}

	MPI_Bcast(local_b, s, MPI_INT, 0, col_comm);

	struct {
		int max;
		int rank;
	} local_max{ INT16_MIN, rank }, global_max{};

	for (int i = 0; i < s; i++)
	{
		local_part_c[i] = 0;
		for (int j = 0; j < s; j++)
		{
			int ind = i * s + j;
			local_part_c[i] += local_A[ind] * local_b[j];
			if (local_max.max < local_A[ind])
				local_max.max = local_A[ind];
		}
	}

	MPI_Reduce(&local_max, &global_max, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&global_max, 1, MPI_2INT, 0, MPI_COMM_WORLD);

	MPI_Reduce(local_part_c, local_c, s, MPI_INT, MPI_SUM, global_max.rank % q, row_comm);

	if (row_rank == global_max.rank % q)
	{
		MPI_Type_vector(s, 1, q, MPI_INT, &nt);
		MPI_Type_create_resized(nt, 0, sizeof(int), &nt);
		MPI_Type_commit(&nt);
		MPI_Gather(local_c, s, MPI_INT, c, 1, nt, global_max.rank / q, col_comm);

		if (rank == global_max.rank)
		{
			ostringstream ss;
			ss << "P[" << col_rank << ", " << row_rank << "] I have the maximum: " << global_max.max << "\n\nc";
			print_vector(ss.str().c_str(), c, m);

			MPI_Send(c, m, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}

	if (!rank)
	{
		MPI_Recv(c, m, MPI_INT, global_max.rank, 0, MPI_COMM_WORLD, &stat);

		if (check_result(c, &A[0][0], b, m))
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

bool check_result(int* res, int* A, int* b, int n)
{
	bool ret = true;
	for (int i = 0; ret && i < n; i++)
	{
		int val = 0;
		for (int j = 0; j < n; j++)
			val += A[i * n + j] * b[j];
		ret = res[i] == val;
	}
	return ret;
}

void print_matrix(const char* lbl, int* M, int n, int m)
{
	cout << "\n\n" << lbl << ":\n";
	for (int i = 0; i < n; i++)
	{
		cout << "|\t";
		for (int j = 0; j < m; (cout << M[i * m + j++] << "\t"));
		cout << "|\n";
	}
}
void print_vector(const char* lbl, int* v, int n)
{
	cout << "\n\n" << lbl << " = |\t";
	for (int i = 0; i < n; cout << v[i++] << "\t");
	cout << "|\n";
}