#include "utilities.h"
#include <omp.h>

int main(int argc, char** argv)
{
    srand(time(NULL));
    omp_set_num_threads(omp_get_num_procs());

    LL N = 1 << 3, x_s, x_p, *res, *add, *sum_s, *sum_p;

    if(argc > 1)
    {
        N = atoll(argv[1]);
        if(argc > 2)
            omp_set_num_threads(atoi(argv[2]));
    }

    initialize_vector_random(res, N, 1LL, 50LL);
    initialize_vector_random(add, N, 1LL, 50LL);
    initialize_vector_random(sum_s, N, 1LL, 50LL);
    copy_vector(sum_s, sum_p, N);

    for(LL i = N - 1; i > 1; i--)
    {
        x_s = res[i] + add[i];
        sum_s[i] = sum_s[i - 1] + x_s;
    }

    LL* sum_p_copy = (LL*) malloc(sizeof(LL) * (N - 2));
    #pragma omp parallel for
    for(LL i = 0; i < N - 2; i++)
        sum_p_copy[i] = sum_p[i + 1];

    #pragma omp parallel for lastprivate(x_p)
    for(LL i = N - 1; i > 1; i--)
    {
        x_p = res[i] + add[i];
        sum_p[i] = sum_p_copy[i - 2] + x_p;
    }

    std::cout << (x_s == x_p && compare_vectors(sum_s, sum_p, N) ? "Correct!" : "False!") << std::endl;

    free(sum_p_copy);
    free(sum_p);
    free(sum_s);
    free(add);
    free(res);

    return 0;
}