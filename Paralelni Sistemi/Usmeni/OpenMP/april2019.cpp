#include <omp.h>
#include "utilities.h"

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();
    LL N = 1 << 10;

    switch(argc)
    {
        case(3): th_numb = atoi(argv[2]);
        case(2): N = atoll(argv[1]);
    }

    LL  *c_s = (LL*) malloc(sizeof(LL) * N),
        *c_p = (LL*) malloc(sizeof(LL) * N),
        *a,
        *b;
    initialize_vector_random(a, N * N, 1LL, 10LL);
    initialize_vector_random(b, N * N, 1LL, 10LL);

    c_s[0] = 1;
    for(LL i = 1; i < N; i++)
    {
        c_s[i] = c_s[i - 1];
        for(LL j = 0 ; j < N; j++)
            c_s[i] *= a[i * N + j] + b[i * N + j];
    }

    c_p[0] = 1;
    //[[ omp :: sequence(directive(parallel), directive(for ordered)) ]]
    #pragma omp parallel for ordered schedule(dynamic)
    for(LL i = 1; i < N; i++)
    {
        LL temp = 1;
        for(LL j = 0; j < N; j++)
            temp *= a[i * N + j] + b[i * N + j];
        //printf("T[%d]: Iteration %d done!\n", omp_get_thread_num(), i);
        #pragma omp ordered
        c_p[i] = c_p[i - 1] * temp;
    }

    std::cout << (compare_vectors(c_s, c_p, N) ? "Correct!" : "False!" ) << std::endl;

    free(b);
    free(a);
    free(c_p);
    free(c_s);

    return 0;
}