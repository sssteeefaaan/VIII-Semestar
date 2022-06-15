#include "utilities.h"
#include <omp.h>

int main(int argc, char** argv)
{
    srand(time(NULL));
    omp_set_num_threads(omp_get_num_procs());

    LL N = 1 << 3, M = 1 << 4, x_s, x_p, **a_s, **a_p;

    if(argc > 1)
    {
        N = atoll(argv[1]);
        if(argc > 2)
        {
            M = atoll(argv[2]);
            if(argc > 3)
                omp_set_num_threads(atoi(argv[3]));
        }
    }

    initialize_matrix_random(a_s, N, M, 1LL, 100LL);
    copy_matrix(a_s, a_p, N, M);

    x_s = 0;
    for(LL i = 0; i < N; i++)
    {
        for(LL j = 0; j < M; j++)
        {
            x_s = x_s + a_s[i][j];
            a_s[i][j] = 2 * a_s[i][j];
        }
    }

    x_p = 0;
    #pragma omp parallel for reduction(+ : x_p)
    for(LL l = 0; l < N * M; l++)
    {
        LL i = l / M, j = l % M;
        x_p = x_p + a_p[i][j];
        a_p[i][j] = 2 * a_p[i][j];
    }

    std::cout << (x_s == x_p && compare_matrices(a_s, a_p, N, M) ? "Correct!" : "False!") << std::endl;

    destroy_matrix(a_p, N);
    destroy_matrix(a_s, N);

    return 0;
}