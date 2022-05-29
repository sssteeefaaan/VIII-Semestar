#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

template <typename T>
void init_vector(T*& a, const unsigned long n);

template <typename T>
void init_matrix(T**& a, const unsigned long n, const unsigned long m);

template <typename T>
void destroy_matrix(T*& a, const unsigned long n, const unsigned long m);

template <typename T>
bool check_result(T** a, T** b, const unsigned long n, const unsigned long m);

int main(int argc, char** argv)
{
	int** a_seq, **a_par, **a_par_mirr;
	unsigned long n = 1 << 10, m = 1 << 5;
	init_matrix(a_seq, m, n);
	init_matrix(a_par, m, n);
	init_matrix(a_par_mirr, m, n);

	double dt;

#pragma region Serial

	dt = omp_get_wtime();
	for (long i = 1; i < m; i++)
		for (unsigned long j = 0; j < n; j++)
			a_seq[i][j] = 2 * a_seq[i - 1][j];
	dt = omp_get_wtime() - dt;
	printf("[Serial]: Time taken = %f\n", dt);

#pragma endregion
	
#pragma region Parallel Mirrored

	dt = omp_get_wtime();
	#pragma omp parallel for
	for (long j = 0; j < n; j++)
		for (unsigned long i = 1; i < m; i++)
			a_par_mirr[i][j] = 2 * a_par_mirr[i - 1][j];
	dt = omp_get_wtime() - dt;
	printf("[Parallel Mirrored]: Time taken = %f\n", dt);

	check_result(a_seq, a_par_mirr, m, n) ?
		printf("Correct!\n")
		:
		printf("False!\n");

#pragma endregion

#pragma region Parallel

	dt = omp_get_wtime();
	for (unsigned long i = 1; i < m; i++)
		#pragma omp parallel for
		for (long j = 0; j < n; j++)
			a_par[i][j] = 2 * a_par[i - 1][j];
	dt = omp_get_wtime() - dt;
	printf("[Parallel]: Time taken = %f\n", dt);

	check_result(a_seq, a_par, m, n) ?
		printf("Correct!\n")
		:
		printf("False!\n");

#pragma endregion

	destroy_matrix(a_par_mirr, m, n);
	destroy_matrix(a_par, m, n);
	destroy_matrix(a_seq, m, n);
	return 0;
}

template <typename T>
void init_vector(T*& a, const unsigned long n)
{
	a = (T*)malloc(sizeof(T) * n);
	for (unsigned long i = 0; i < n; i++)
		a[i] = 1;
}

template <typename T>
void init_matrix(T**& a, const unsigned long n, const unsigned long m)
{
	a = (T**)malloc(sizeof(T*) * n);
	for (unsigned long i = 0; i < n; i++)
		init_vector(a[i], m);
}

template <typename T>
void destroy_matrix(T*& a, const unsigned long n, const unsigned long m)
{
	for (unsigned long i = 0; i < n; i++)
		free(a[i]);
	free(a);
}

template <typename T>
bool check_result(T** a, T** b, const unsigned long n, const unsigned long m)
{
	bool ret = true;
	for (unsigned long i = 0; ret && i < n; i++)
		for (unsigned long j = 0; j < m; j++)
			ret = a[i][j] == b[i][j];
	return ret;
}