#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

template <typename T>
void init_vector(T*& a, int n);

template <typename T>
bool compare_vectors(T* a, T* b, const long n);

int main(int argc, char** argv)
{
	const long n = 1 << 10;

	int* a, * b,
		* c_par = (int*)malloc(sizeof(int) * n * n),
		*c_seq = (int*)malloc(sizeof(int) * n * n);

	init_vector(a, n * n);
	init_vector(b, n * n);

	double dt;
	
#pragma region Serial

	dt = omp_get_wtime();
	for (long i = 0; i < n; i++)
	{
		for (long j = 0; j < n; j++)
		{
			c_seq[i * n + j] = 0;
			for (long k = 0; k < n; k++)
				c_seq[i * n + j] += a[i * n + k] * b[k * n + j];
		}
	}
	dt = omp_get_wtime() - dt;
	printf("[Serial]: Time taken = %f\n", dt);

#pragma endregion

	omp_set_num_threads(omp_get_num_procs());

#pragma region Parallel Collapsed Foor-Loops

	dt = omp_get_wtime();

#pragma omp parallel for
	for (long i = 0; i < n * n; i++)
	{
		long tmp = 0;
		for (long k = 0; k < n; k++)
			tmp += a[(i / n) + k] * b[k * n + (i % n)];
		c_par[i] = tmp;
	}

	dt = omp_get_wtime() - dt;
	printf("[OpenMP Collapsed For-Loops]: Time taken = %f\n", dt);

	compare_vectors(c_seq, c_par, n * n) ?
		printf("Correct!\n")
		:
		printf("False!\n");

#pragma endregion

	
#pragma region Parallel

	dt = omp_get_wtime();

	#pragma omp parallel for
	for (long i = 0; i < n; i++)
	{
		for (long j = 0; j < n; j++)
		{
			long tmp = 0;
			for (long k = 0; k < n; k++)
				tmp += a[i * n + k] * b[k * n + j];
			c_par[i * n + j] = tmp;
		}
	}

	dt = omp_get_wtime() - dt;
	printf("[OpenMP]: Time taken = %f\n", dt);

	compare_vectors(c_seq, c_par, n * n) ?
		printf("Correct!\n")
		:
		printf("False!\n");

#pragma endregion

	free(b);
	free(a);
	free(c_seq);
	free(c_par);

	return 0;
}

template <typename T>
bool compare_vectors(T* a, T* b, const long n)
{
	bool ret = true;
	for (long i = 0; ret && i < n; i++)
		ret = a[i] == b[i];
	return ret;
}

template <typename T>
void init_vector(T*& a, int n)
{
	a = (T*)malloc(sizeof(T) * n);
	for (int i = 0; i < n; i++)
		a[i] = 1;
}