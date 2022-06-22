#include <omp.h>
#include "utilities.h"

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();
    LL N = 1 << 5, d_s, d_p, *b_s, *b_p; 

    switch(argc)
    {
        case(3): th_numb = atoi(argv[2]);
        case(2): N = atoll(argv[1]);
    }

    initialize_vector_random(b_s, N, 1LL, 100LL);
    copy_vector(b_s, b_p, N);

    d_s = 0;
    for(LL m = 0; m < N - 1; m ++)
    {
        b_s[m] = b_s[m + 1] * d_s;
        d_s = d_s + m;
    }

    omp_set_num_threads(th_numb);
    LL* b_p_copy = (LL*) malloc(sizeof(LL) * (N - 1));
    d_p =  0;
    #pragma omp parallel
    {
        #pragma omp for
        for(LL i = 0; i < N - 1; i++)
            b_p_copy[i] = b_p[i + 1];

        #pragma omp for lastprivate(d_p)
        for(LL m = 0 ; m < N - 1; m++)
        {
            d_p = m * (m - 1) / 2;
            b_p[m] = b_p_copy[m] * d_p;
            d_p = d_p + m;
        }

    }
    free(b_p_copy);

    std::cout << "d = " << d_p << std::endl;
    print_vector("b", b_p, N, std::cout);
    std::cout << (d_s == d_p && compare_vectors(b_s, b_p, N) ? "Correct!" : "False!") << std::endl;

    free(b_p);
    free(b_s);
    return 0;
}