#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

template <typename T>
void init_vector(T*&v, const unsigned long long n)
{
    v = (T*) malloc(sizeof(T) * n);
    for(unsigned long long i = 0 ; i < n; i++)
        v[i] = 1;
}

template <typename T>
void init_matrix(T**&M, const unsigned long long n, const unsigned long long m)
{
    M = (T**) malloc(sizeof(T*) * n);
    for(unsigned long long i = 0 ; i < n; i++)
        init_vector(M[i], m);
}

template <typename T>
void destroy_matrix(T**&M, const unsigned long long n, const unsigned long long m)
{
    for(unsigned long long i = 0 ; i < n; i++)
        free(M[i]);
    free(M);
}

template <typename T>
bool check_result(T** A, T** B, const unsigned long long n, const unsigned long long m)
{
    bool ret = true;
    for(unsigned long long i = 0; ret && i < n; i++)
        for(unsigned long long j = 0; ret && j < m; j++)
            ret = A[i][j] == B[i][j];
    return ret;
}

int main(int argc, char** argv)
{
    long long n, m;
    int**a_par, **a_seq;
    long long x_par, x_seq;
    double dt;

    if(argc == 3)
        n = atoll(argv[1]), m = atoll(argv[2]);
    else
        n = 1 << 10, m = 1 << 12;

    init_matrix(a_par, n, m);
    init_matrix(a_seq, n, m);

    omp_set_num_threads(omp_get_num_procs());

    dt = omp_get_wtime();
    x_par = 0;
    #pragma omp parallel for reduction(+ : x_par)
    for(long long i = 0; i < n * m; i++)
    {
        long long ii = i / m, ij = i % m;
        x_par += a_par[ii][ij];
        a_par[ii][ij] <<= 1;
    }

    dt = omp_get_wtime() - dt;
    printf("[Parallel Reduction]: Time taken = %f\n", dt);

    dt = omp_get_wtime();
    x_seq = 0;
    for(long long i = 0; i < n; i++)
    {
        for(long long j = 0; j < m; j++)
        {
            x_seq = x_seq + a_seq[i][j];
            a_seq[i][j] = 2 * a_seq[i][j];
        }
    }

    dt = omp_get_wtime() - dt;
    printf("[Serial]: Time taken = %f\n", dt);

    x_par == x_seq && 
    check_result(a_seq, a_par, n, m) ?
    printf("Correct!\n")
    :
    printf("False!\n");

    destroy_matrix(a_seq, n, m);
    destroy_matrix(a_par, n, m);

    return 0;
}