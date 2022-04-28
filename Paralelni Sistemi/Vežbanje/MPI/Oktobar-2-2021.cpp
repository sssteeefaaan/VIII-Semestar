#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
using namespace std;

bool check_result(int* res, int* A, int* B, int n, int m, int k);
void print_matrix(const char* lbl, int* M, int n, int m);
void print_vector(const char* lbl, int* v, int n);

int main(int argc, char** argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	const int n = 1 << 3,
		p = 1 << 4,
		q = sqrt(p),
		s = n / q;

	MPI_Comm row_comm, col_comm;
	MPI_Comm_split(MPI_COMM_WORLD, rank % q, 0, &col_comm);
	MPI_Comm_split(MPI_COMM_WORLD, rank / q, 0, &row_comm);

	int col_rank, row_rank;
	MPI_Comm_rank(col_comm, &col_rank);
	MPI_Comm_rank(row_comm, &row_rank);

	int A[n][n], B[n][n], C[n][n],
		* local_A, * local_B, * local_C, * local_part_C;

	local_A = (int*)malloc(sizeof(int) * s * n);
	local_B = (int*)malloc(sizeof(int) * n * s);
	local_part_C = (int*)malloc(sizeof(int) * s * s);
	local_C = (int*)malloc(sizeof(int) * s * n);

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				A[i][j] = i * n + j;
				B[i][j] = 0;
			}
			B[i][n - 1 - i] = 1;
		}

		print_matrix("A", &A[0][0], n, n);
		print_matrix("B", &B[0][0], n, n);
	}

	MPI_Datatype tmp, nt_col, nt_row;
	MPI_Type_vector(s, n, n * q, MPI_INT, &tmp);
	MPI_Type_create_resized(tmp, 0, n * sizeof(int), &nt_row);
	MPI_Type_commit(&nt_row);

	MPI_Type_vector(s, 1, q, MPI_INT, &tmp);
	MPI_Type_create_resized(tmp, 0, sizeof(int), &tmp);
	MPI_Type_vector(n, 1, n, tmp, &tmp);
	MPI_Type_create_resized(tmp, 0, sizeof(int), &nt_col);
	MPI_Type_commit(&nt_col);

	if (col_rank == 0)
		MPI_Bcast(A, n * n, MPI_INT, 0, row_comm);
	if (row_rank == 0)
		MPI_Bcast(B, n * n, MPI_INT, 0, col_comm);

	MPI_Scatter(A, 1, nt_row, local_A, s * n, MPI_INT, 0, col_comm);
	MPI_Scatter(B, 1, nt_col, local_B, n * s, MPI_INT, 0, row_comm);

	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			local_part_C[i * s + j] = 0;
			for (int k = 0; k < n; k++)
				local_part_C[i * s + j] += local_A[i * n + k] * local_B[k * s + j];
		}
	}

	int* strides = (int*)malloc(sizeof(int) * s * s);
	int* block_lens = (int*)malloc(sizeof(int) * s * s);

	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			block_lens[i * s + j] = 1;
			strides[i * s + j] = i * n + j * q;
		}
	}
	MPI_Type_indexed(s * s, block_lens, strides, MPI_INT, &tmp);
	MPI_Type_create_resized(tmp, 0, sizeof(int), &nt_col);
	MPI_Type_commit(&nt_col);

	MPI_Gather(local_part_C, s * s, MPI_INT, local_C, 1, nt_col, 0, row_comm);

	if (row_rank == 0)
	{
		MPI_Gather(local_C, s * n, MPI_INT, C, 1, nt_row, 0, col_comm);

		if (rank == 0)
		{
			print_matrix("C", &C[0][0], n, n);

			if (check_result(&C[0][0], &A[0][0], &B[0][0], n, n, n))
				cout << "\n\nCorrect!" << endl;
			else
				cout << "\n\nFalse!" << endl;
		}
	}

	free(block_lens);
	free(strides);
	free(local_part_C);
	free(local_C);
	free(local_B);
	free(local_A);

	MPI_Finalize();

	return 0;
}

bool check_result(int* res, int* A, int* B, int n, int m, int k)
{
	bool c = true;
	for (int i = 0; c && i < n; i++)
	{
		for (int j = 0; c && j < k; j++)
		{
			int val = 0;
			for (int p = 0; p < m; p++)
				val += A[i * m + p] * B[p * k + j];
			c = val == res[i * k + j];
		}
	}
	return c;
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