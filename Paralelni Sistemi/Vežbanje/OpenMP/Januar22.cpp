#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

template <typename T>
void init_vector(T*& v, const long N)
{
    v = (T*)malloc(sizeof(T) * N);
    for(long i = 0; i < N; i++)
        v[i] = rand() % 5 + 1;
}

int main()
{
    long N = 10,
        offset = 5,
        prod;
    
    int *a, *b, *c, *x;

    init_vector(a, N + 1);
    init_vector(b, N);
    init_vector(c, N + offset);
    init_vector(x, N);

    omp_set_num_threads(omp_get_num_procs());

    int *a_offset = (int*)malloc(sizeof(int) * N);
    #pragma omp parallel for
    for(long i = 0; i < N; i++)
        a_offset[i] = a[i + 1];
    
    prod = 1;
    #pragma omp parallel for reduction(*:prod)
    for(long i = 0; i < N; i++)
    {
        a[i] = a_offset[i] + b[i] * c[offset + N - i - 1];
        prod *= x[i];
    }

    printf("prod = %ld\n", prod);
    printf("a = |\t");
    for(long i = 0; i < N + 1; i++)
        printf("%d\t", a[i]);
    printf("|\n");

    free(a_offset);
    free(x);
    free(c);
    free(b);
    free(a);

    return 0;
}