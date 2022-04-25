//#include <mpi.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <iostream>
//#include <sstream>
//using namespace std;
//
//bool check_result(int* res, int* A, int* b, int n);
//void print_matrix(const char* lbl, int* M, int n, int m);
//void print_vector(const char* lbl, int* v, int n);
//
//int main(int argc, char** argv)
//{
//	int rank, size;
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//	const int k = 1 << 3,
//			p = 1 << 4,
//			q = sqrt(p),
//			s = k / q;
//
//	if (size != p)
//		exit(-100);
//
//	int A[k][k], b[k], *local_A, *local_b, *local_part_res, *local_res, res[k];
//	local_A = (int*)malloc(sizeof(int) * s * s);
//	local_b = (int*)malloc(sizeof(int) * s);
//	local_part_res = (int*)malloc(sizeof(int) * s);
//	local_res = (int*)malloc(sizeof(int) * s);
//
//	if (rank == 0)
//	{
//		for (int i = 0; i < k; i++)
//		{
//			for (int j = 0; j < k; j++)
//				A[i][j] = (k * k) - (i * k + j + 1);
//			b[i] = i;
//		}
//
//		print_matrix("A", &A[0][0], k, k);
//		print_vector("b", &b[0], k);
//	}
//
//	int row_rank, col_rank;
//	MPI_Comm row_comm, col_comm;
//	MPI_Comm_split(MPI_COMM_WORLD, rank % q, 0, &col_comm);
//	MPI_Comm_split(MPI_COMM_WORLD, rank / q, 0, &row_comm);
//
//	MPI_Comm_rank(col_comm, &col_rank);
//	MPI_Comm_rank(row_comm, &row_rank);
//
//#pragma region a)
//
//	/*MPI_Status stat;
//	MPI_Request req;
//
//	if (rank == 0)
//		for (int i = 0; i < size; i++)
//			for (int blck = 0; blck < s; blck++)
//				MPI_Isend(&A[(i / q) + blck * q][(i % q) * s], s, MPI_INT, i, 0, MPI_COMM_WORLD, &req);
//
//	for (int i = 0; i < s; i++)
//		MPI_Recv(&local_A[i * s], s, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);*/
//
//#pragma endregion
//
//#pragma region b)
//
//	MPI_Datatype tmp, n_type;
//	MPI_Type_vector(s, s, k * q, MPI_INT, &tmp);
//	MPI_Type_create_resized(tmp, 0, sizeof(int) * s, &n_type);
//	MPI_Type_commit(&n_type);
//
//	MPI_Scatter(&A[0][0], 1, n_type, local_A, s * s, MPI_INT, 0, MPI_COMM_WORLD);
//
//#pragma endregion
//
//	if (col_rank == 0)
//		MPI_Scatter(&b[0], s, MPI_INT, local_b, s, MPI_INT, 0, row_comm);
//
//	MPI_Bcast(local_b, s, MPI_INT, 0, col_comm);
//
//	struct {
//		int min;
//		int rank;
//	} local_min { INT16_MAX, rank }, global_min{};
//
//	for (int i = 0; i < s; i++)
//	{
//		local_part_res[i] = 0;
//		for (int j = 0; j < s; j++)
//		{
//			int ind = i * s + j;
//			local_part_res[i] += local_A[ind] * local_b[j];
//			if (local_min.min > local_A[ind])
//				local_min.min = local_A[ind];
//		}
//	}
//
//	MPI_Reduce(&local_min, &global_min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
//	MPI_Bcast(&global_min, 1, MPI_2INT, 0, MPI_COMM_WORLD);
//
//	MPI_Reduce(local_part_res, local_res, s, MPI_INT, MPI_SUM, global_min.rank % q, row_comm);
//
//	if (row_rank == (global_min.rank % q))
//	{
//		MPI_Datatype tmp_2, n_type_2;
//		MPI_Type_vector(s, 1, q, MPI_INT, &tmp_2);
//		MPI_Type_create_resized(tmp_2, 0, sizeof(int), &n_type_2);
//		MPI_Type_commit(&n_type_2);
//		MPI_Gather(local_res, s, MPI_INT, res, 1, n_type_2, (global_min.rank / q), col_comm);
//
//		if (rank == global_min.rank)
//		{
//			ostringstream ss;
//			ss << "P[" << col_rank << ", " << row_rank << "] I have the minimum element: " << global_min.min << "!\n\nres";
//			print_vector(ss.str().c_str(), res, k);
//
//			MPI_Request req;
//			MPI_Isend(res, k, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
//		}
//	}
//
//	if (!rank)
//	{
//		MPI_Status stat;
//
//		MPI_Recv(res, k, MPI_INT, global_min.rank, 0, MPI_COMM_WORLD, &stat);
//		if (check_result(res, &A[0][0], b, k))
//			cout << "\n\nCorrect!" << endl;
//		else
//			cout << "\n\nFalse!" << endl;
//	}
//
//	free(local_res);
//	free(local_part_res);
//	free(local_b);
//	free(local_A);
//
//	MPI_Finalize();
//	
//	return 0;
//}
//
//bool check_result(int* res, int* A, int* b, int n)
//{
//	bool ret = true;
//	for (int i = 0; ret && i < n; i++)
//	{
//		int val = 0;
//		for (int j = 0; j < n; j++)
//			val += A[i * n + j] * b[j];
//		ret = res[i] == val;
//	}
//	return ret;
//}
//
//void print_matrix(const char* lbl, int* M, int n, int m)
//{
//	cout << "\n\n" << lbl << ":\n";
//	for (int i = 0; i < n; i++)
//	{
//		cout << "|\t";
//		for (int j = 0; j < m; (cout << M[i * m + j++] << "\t"));
//		cout << "|\n";
//	}
//}
//void print_vector(const char* lbl, int* v, int n)
//{
//	cout << "\n\n" << lbl << " = |\t";
//	for (int i = 0; i < n; cout << v[i++] << "\t");
//	cout << "|\n";
//}