#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

bool isPrime(long a);

int main(int argc, char** argv)
{
	long n = 1 << 32,
		primes_seq, primes_par;
	double dt;

#pragma region Serial

	dt = omp_get_wtime();
	primes_seq = 0;
	for (long i = 0; i < n; i++)
		if (isPrime(i))
			primes_seq++;
	dt = omp_get_wtime() - dt;
	printf("[Serial]: Time taken = %f\n", dt);

#pragma endregion

	omp_set_num_threads(omp_get_num_procs());

#pragma region OpenMP Reduction

	dt = omp_get_wtime();
	primes_par = 0;
	
	#pragma omp parallel for reduction(+:primes_par) schedule(dynamic, 5)
	for (long i = 0; i < n; i++)
		if (isPrime(i))
			primes_par++;
	dt = omp_get_wtime() - dt;
	printf("[OpenMP Reduction]: Time taken = %f\n", dt);

	printf(primes_par == primes_seq ? "Correct!\n" : "False!\n");

#pragma endregion

#pragma region OpenMP

	dt = omp_get_wtime();
	primes_par = 0;

	#pragma omp parallel for schedule(dynamic, 5)
	for (long i = 0; i < n; i++)
		if (isPrime(i))
		#pragma omp critical
			primes_par++;

	dt = omp_get_wtime() - dt;
	printf("[OpenMP]: Time taken = %f\n", dt);

	printf(primes_par == primes_seq ? "Correct!\n" : "False!\n");

#pragma endregion

	return 0;
	
}

bool isPrime(long a)
{
	if (a <= 1 || a % 2 == 0 || a % 3 == 0)
		return false;

	if (a <= 3)
		return true;

	for (long i = 5; i * i < a; i += 6)
		if (a % i == 0 || a % (i + 2) == 0)
			return false;

	return true;
}