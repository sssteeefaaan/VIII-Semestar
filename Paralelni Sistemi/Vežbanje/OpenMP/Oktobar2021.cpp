#include "utilities.h"
#include <omp.h>

int main(int argc, char** argv)
{
    srand(time(NULL));
    omp_set_num_threads(omp_get_num_procs());

    LL N = 1 << 3, M = 1 << 4, x_s, x_p, **b_s, **b_p, **a;

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

    initialize_matrix_random(a, N, M, 1LL, 100LL);
    initialize_matrix(b_s, N, M, 0LL);
    initialize_matrix(b_p, N, M, 0LL);

    x_s = 0;
    for(LL i = 2; i < N; i++)
    {
        for(LL j = 0; j < M; j++)
        {
            x_s = x_s + a[i][j];
            b_s[i][j] = a[i - 2][j];
        }
    }

    x_p = 0;
    #pragma omp parallel for reduction(+ : x_p)
    for(LL l = 2 * M; l < N * M; l++)
    {
        LL i = l / M, j = l % M;
        x_p = x_p + a[i][j];
        b_p[i][j] = a[i - 2][j];
    }

    std::cout << (x_s == x_p && compare_matrices(b_s, b_p, N, M) ? "Correct!" : "False!") << std::endl;

    destroy_matrix(b_p, N);
    destroy_matrix(b_s, N);
    destroy_matrix(a, N);

    return 0;
}