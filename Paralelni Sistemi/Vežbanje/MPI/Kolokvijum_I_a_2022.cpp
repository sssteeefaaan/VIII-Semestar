#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

void init_matrix(int* A, int n, int m);
bool check_result(int* A, int* B, int* C, int n, int m, int k);
void print_matrix(const char* lbl, int* A, int n, int m);

int main(int argc, char** argv)
{
	int rank, p;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	const int k = 1 << 6, n = 1 << 5, m = 1 << 4, l = k / p;
	int A[k][n], B[n][m], C[k][m],
		* local_A, * local_C;

	local_A = (int*)malloc(sizeof(int) * l * n);
	local_C = (int*)malloc(sizeof(int) * l * m);

	if (!rank)
	{
		init_matrix(&A[0][0], k, n);
		init_matrix(&B[0][0], n, m);
	}
	
	MPI_Datatype nt;
	MPI_Type_vector(l, n, n * p, MPI_INT, &nt);
	MPI_Type_create_resized(nt, 0, sizeof(int) * n, &nt);
	MPI_Type_commit(&nt);

	MPI_Scatter(&A[0][0], 1, nt, local_A, l * n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&B[0][0], n * m, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (int i = 0; i < l; i++)
	{
		for (int j = 0; j < m; j++)
		{
			local_C[i * m + j] = 0;
			for (int q = 0; q < n; q++)
				local_C[i * m + j] += local_A[i * n + q] * B[q][j];
		}
	}

	MPI_Type_vector(l, m, m * p, MPI_INT, &nt);
	MPI_Type_create_resized(nt, 0, sizeof(int) * m, &nt);
	MPI_Type_commit(&nt);

	MPI_Gather(local_C, l * m, MPI_INT, &C[0][0], 1, nt, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		// print_matrix("C", &C[0][0], k, m);
		if (check_result(&A[0][0], &B[0][0], &C[0][0], k, n, m))
			printf("\nCorrect\n");
		else
			printf("\nFalse\n");
	}

	MPI_Finalize();
	return 0;
}

void init_matrix(int* A, int n, int m)
{
	for (int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			A[i * m + j] = i * m + j;
}

bool check_result(int* A, int* B, int* C, int n, int m, int k)
{
	bool ret = true;

	for (int i = 0; ret && i < n; i++)
	{
		for (int j = 0; ret && j < k; j++)
		{
			int temp = 0;
			for (int p = 0; p < m; p++)
				temp += A[i * m + p] * B[p * k + j];
			ret = temp == C[i * k + j];
		}
	}

	return ret;
}

void print_matrix(const char* lbl, int* A, int n, int m)
{
	printf("\n\n%s", lbl);
	for(int i = 0; i < n; i++)
	{
		printf("|\t");
		for(int j = 0; j < m; j++)
			printf("%d\t", A[i * m + j]);
		printf("|\n");
	}
}
