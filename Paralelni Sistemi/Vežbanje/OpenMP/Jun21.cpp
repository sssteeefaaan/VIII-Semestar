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
bool check_result(T* a, T* b, const unsigned long long n)
{
    bool ret = true;
    for(unsigned long long i = 0; ret && i < n; i++)
            ret = a[i] == b[i];
    return ret;
}

int main(int argc, char** argv)
{
    long long n;
    int *add, *res, *sum_par, *sum_seq;
    long long x_par, x_seq;
    double dt;

    n = argc == 2 ? atoll(argv[1]) : 1 << 20;

    init_vector(add, n);
    init_vector(res, n);
    init_vector(sum_par, n);
    init_vector(sum_seq, n);

    omp_set_num_threads(omp_get_num_procs());

    dt = omp_get_wtime();
    x_par = 0;
    #pragma omp parallel for lastprivate(x_par)
    for(long long i = n - 1; i > 0; i--)
    {
        x_par = add[i] + res[i];
        sum_par[i] = sum_seq[i - 1] + x_par;
    }

    dt = omp_get_wtime() - dt;
    printf("[Parallel]: Time taken = %f\n", dt);

    dt = omp_get_wtime();
    x_seq = 0;
    for(long long i = n - 1; i > 0; i--)
    {
        x_seq = add[i] + res[i];
        sum_seq[i] = sum_seq[i - 1] + x_seq;
    }

    dt = omp_get_wtime() - dt;
    printf("[Serial]: Time taken = %f\n", dt);

    x_par == x_seq && 
    check_result(sum_seq, sum_par, n) ?
    printf("Correct!\n")
    :
    printf("False!\n");

    free(sum_seq);
    free(sum_par);
    free(res);
    free(add);

    return 0;
}