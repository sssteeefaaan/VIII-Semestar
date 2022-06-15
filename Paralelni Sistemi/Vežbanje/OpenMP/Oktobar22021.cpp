#include "utilities.h"
#include <omp.h>

int main(int argc, char** argv)
{
    srand(time(NULL));
    omp_set_num_threads(omp_get_num_procs());

    LL N = 1 << 3, M = 1 << 4, h_p, h_s, **a_p, **a_s;
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

    initialize_matrix_random(a_s, N, M, 1LL, 50LL);
    copy_matrix(a_s, a_p, N, M);

    h_s = 0;
    for(LL i = 2; i < N; i++)
    {
        for(LL j = 0; j < M; j++)
        {
            h_s = h_s+ a_s[i][j];
            a_s[i][j] = a_s[i - 2][j];
        }
    }

    h_p = 0;
    #pragma omp parallel for reduction(+ : h_p)
    for(LL j = 0; j < M; j++)
    {
        for(LL i = 2; i < N; i++)
        {
            h_p = h_p + a_p[i][j];
            a_p[i][j] = a_p[i - 2][j];
        }
    }

    std::cout << (h_p == h_s && compare_matrices(a_p, a_s, N, M) ? "Correct!" : "False!") << std::endl;
    print_matrix(a_p, N, M, std::cout);
    
    destroy_matrix(a_s, N);
    destroy_matrix(a_p, N);

    return 0;
}