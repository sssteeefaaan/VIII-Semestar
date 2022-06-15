#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

template <typename T>
void init_vector(T*& a, const unsigned long n);

int main(int arhc, char** argv)
{
	omp_set_num_threads(omp_get_num_procs());

	const long n = 1 << 31;
	int* a, * b,
		res_par = 0,
		res_seq = 0;
	double dt;

	init_vector(a, n);
	init_vector(b, n);

#pragma region Serial

	dt = omp_get_wtime();

	for (long i = 0; i < n; i++)
		res_seq += a[i] * b[i];

	dt = omp_get_wtime() - dt;
	printf("[Serial]: Time taken = %f\n", dt);

#pragma endregion

#pragma region Parallel reduction

	dt = omp_get_wtime();

#pragma omp parallel for reduction(+ : res_par)
	for (long i = 0; i < n; i++)
		res_par += a[i] * b[i];

	dt = omp_get_wtime() - dt;
	printf("[OpenMP reduction]: Time taken = %f\n", dt);

	res_seq == res_par ?
		printf("Correct!\n")
		:
		printf("False!\n");

#pragma endregion

#pragma region Parallel

	res_par = 0;
	dt = omp_get_wtime();

#pragma omp parallel for
	for (long i = 0; i < n; i++)
#pragma omp critical
		res_par += a[i] * b[i];

	dt = omp_get_wtime() - dt;
	printf("[OpenMP]: Time taken = %f\n", dt);


	res_seq == res_par ?
		printf("Correct!\n")
		:
		printf("False!\n");

#pragma endregion

	free(a);
	free(b);

	return 0;
}

template <typename T>
void init_vector(T*& a, const unsigned long n)
{
	a = (int*)malloc(sizeof(int) * n);
	for (unsigned long i = 0; i < n; i++)
		a[i] = 1;
}