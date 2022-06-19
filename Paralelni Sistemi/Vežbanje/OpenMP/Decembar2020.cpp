#include <omp.h>
#include "utilities.h"

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();
    LL m = 3, N = 1 << 3, z_s, z_p, *X_s, *X_p, *Y;

    if(argc > 1)
    {
        N = atoll(argv[1]);
        if(argc > 2)
            th_numb = atoi(argv[2]);
    }

    initialize_vector_random(X_s, N, 1LL, 100LL);
    initialize_vector_random(Y, m + 2 * N * N, 1LL, 100LL);
    copy_vector(X_s, X_p, N);

    std::cout << "Initialization complete!" << std::endl;

    z_s = m;
    for(LL i = 0; i < N; i++)
    {
        for(LL j = 0; j < N; j++)
        {
            X_s[j] += Y[z_s];
            z_s += 2;
        }
    }

    std::cout << "Serial method complete!" << std::endl;

    omp_set_num_threads(th_numb);

    #pragma omp parallel for lastprivate(z_p)
    for(LL i = 0; i < N * N; i++)
    {
        z_p = m + 2 * i;
        #pragma omp critical
        X_p[i % N] += Y[z_p];
        z_p += 2;
    }

    // for(LL i = 0; i < N; i++)
    // {
    //     #pragma omp parallel for lastprivate(z_p)
    //     for(LL j = 0; j < N; j++)
    //     {
    //         z_p = m + 2 * (i * N + j);
    //         X_p[j] += Y[z_p];
    //         z_p += 2;
    //     }
    // }

    std::cout << "Parallel method complete!" << std::endl;
    std::cout << (z_p == z_s && compare_vectors(X_s, X_p, N) ? "Correct!" : "False!") << std::endl;

    free(X_p);
    free(Y);
    free(X_s);

    return 0;
}