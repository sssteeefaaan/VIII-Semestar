#include <omp.h>
#include <stdlib.h>
#include <iostream>
#include "utilities.h"

int main(int argc, char** argv)
{
    omp_set_num_threads(omp_get_num_procs());

    LL N = 1 << 4, offset = 1 << 2, prod_s, prod_p, *a_s, *a_p, *b, *c, *x;

    if(argc > 1)
    {
        N = atoll(argv[1]);
        if(argc > 2)
        {
            offset = atoll(argv[2]);
            if(argc > 3)
                omp_set_num_threads(atoi(argv[3]));
        }
    }

    std::cout << "N = " << N << std::endl << "offset = " << offset << std::endl;

    initialize_vector(a_s, N + 1, 1LL);
    initialize_vector(a_p, N + 1, 1LL);
    initialize_vector(b, N, 1LL);
    initialize_vector(c, N + offset, 1LL);
    initialize_vector(x, N, 1LL);

    prod_s = 1;
    for(LL i = 0; i < N; i++)
    {
        a_s[i] = a_s[i + 1] + b[i] * c[N - 1 - i + offset];
        prod_s = prod_s * x[i];
    }
    
    LL* a_copy = (LL*) malloc(sizeof(LL) * N);
    #pragma omp parallel for
    for(LL i = 0 ; i < N; i++)
        a_copy[i] = a_p[i + 1];
    
    prod_p = 1;
    #pragma omp parallel for reduction(*:prod_p)
    for(LL i = 0; i < N; i++)
    {
        a_p[i] = a_copy[i] + b[i] * c[offset + N - 1 - i];
        prod_p = prod_p * x[i];
    }

    std::cout << "prod = " << prod_p << std::endl;
    print_vector("a", a_p, N + 1, std::cout);

    std::cout << (prod_s == prod_p && compare_vectors(a_s, a_p, N + 1) ? "Correct!" : "False!") << std::endl;

    free(a_copy);
    free(x);
    free(c);
    free(b);
    free(a_p);
    free(a_s);

    return 0;
}