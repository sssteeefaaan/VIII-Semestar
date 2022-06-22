#include <omp.h>
#include "utilities.h"

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();
    LL N = 1 << 5, s = 1, k = 2, z_s, z_p, *A_s, *A_p, *B;

    switch(argc)
    {
        case(5): th_numb = atoi(argv[4]);
        case(4): s = atoll(argv[3]);
        case(3): k = atoll(argv[2]);
        case(2): N = atoll(argv[1]);
    }

    initialize_vector_random(A_s, N, 1LL, 100LL);
    copy_vector(A_s, A_p, N);
    initialize_vector_random(B, s + k * N * N, 1LL, 100LL);

    z_s = s;
    for(LL i = 0 ; i < N; i++)
    {
        for(LL j = 0; j < N; j++)
        {
            A_s[j] += B[z_s];
            z_s += k;
        }
    }

    z_p = s;
    #pragma omp parallel for reduction(+ : z_p)
    for(LL i = 0 ; i < N * N; i++)
    {
        #pragma omp critical
        A_p[i % N] += B[s + i * k];
        z_p += k;
    }

    // z_p = s;
    // for(LL j = 0; j < N; j++)
    // {
    //     #pragma omp parallel for reduction(+ : A_p[j], z_p)
    //     for(LL i = 0; i < N; i++)
    //     {
    //         A_p[j] += B[s + k * (i * N + j)];
    //         z_p += k;
    //     } 
    // }

    std::cout << "z = " << z_p << std::endl;
    print_vector("A", A_p, N, std::cout);
    std::cout << (z_p == z_s && compare_vectors(A_s, A_p, N) ? "Correct!" : "False!") << std::endl;

    free(B);
    free(A_p);
    free(A_s);

    return 0;
}