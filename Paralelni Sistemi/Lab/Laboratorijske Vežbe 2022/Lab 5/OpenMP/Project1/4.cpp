#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

template <typename T>
void init_vector(T*& a, const unsigned long n);

int main(int arhc, char** argv)
{
	omp_set_num_threads(omp_get_num_procs());

	int* a;
	long n = 1 << 22, sum_par, sum_seq;
	double dt, avg_par, avg_seq;

	init_vector(a, n);

#pragma region Serial

	dt = omp_get_wtime();

	sum_seq = 0;
	for (long i = 0; i < n; i++)
		sum_seq += a[i];

	avg_seq = (double)sum_seq / n;

	dt = omp_get_wtime() - dt;
	printf("[Serial]: Time taken = %f\n", dt);

#pragma endregion

#pragma region Parallel reduction

	dt = omp_get_wtime();

	sum_par = 0;
	#pragma omp parallel for reduction(+ : sum_par)
	for (long i = 0; i < n; i++)
		sum_par += a[i];

	avg_par = (double)sum_par / n;

	dt = omp_get_wtime() - dt;
	printf("[OpenMP Reduction]: Time taken = %f\n", dt);

	avg_par == avg_seq ?
		printf("Correct!\n")
		:
		printf("False!\n");

#pragma endregion

#pragma region Parallel

	sum_par = 0;
	dt = omp_get_wtime();

#pragma omp parallel for
	for (long i = 0; i < n; i++)
	#pragma omp critical
		sum_par += a[i];

	avg_par = (double)sum_par / n;

	dt = omp_get_wtime() - dt;
	printf("[OpenMP]: Time taken = %f\n", dt);


	avg_seq == avg_par ?
		printf("Correct!\n")
		:
		printf("False!\n");

#pragma endregion

	free(a);

	return 0;
}

template <typename T>
void init_vector(T*& a, const unsigned long n)
{
	a = (T*)malloc(sizeof(T) * n);
	for (unsigned long i = 0; i < n; i++)
		a[i] = 1;
}