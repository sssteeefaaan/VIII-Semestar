#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

template <typename T>
void init_vector(T*& a, const unsigned long n);

int main(int arhc, char** argv)
{
	omp_set_num_threads(omp_get_num_procs());

	int* a, max_seq, max_par;
	long n = 1 << 31;
	double dt;

	init_vector(a, n);

#pragma region Serial

	dt = omp_get_wtime();

	max_seq = INT_MIN;
	for (long i = 0; i < n; i++)
		max_seq = max_seq > a[i] ? max_seq : a[i];

	dt = omp_get_wtime() - dt;
	printf("[Serial]: Time taken = %f\n", dt);

#pragma endregion

#pragma region Parallel

	max_par = INT_MIN;
	dt = omp_get_wtime();

	#pragma omp parallel for
	for (long i = 0; i < n; i++)
		#pragma omp critical
		max_par = max_par > a[i] ? max_par : a[i];

	dt = omp_get_wtime() - dt;
	printf("[OpenMP]: Time taken = %f\n", dt);

	max_seq == max_par ? 
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