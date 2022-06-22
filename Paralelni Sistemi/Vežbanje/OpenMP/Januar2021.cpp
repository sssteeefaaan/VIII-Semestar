#include <omp.h>
#include "utilities.h"

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();
    LL N = 1 << 5, t_s, t_p, *a_s, *a_p,  *b, *c;

    switch(argc)
    {
        case(3): th_numb = atoi(argv[2]);
        case(2): N = atoll(argv[1]);
    }

    initialize_vector_random(a_s, N, 1LL, 100LL);
    copy_vector(a_s, a_p, N);
    initialize_vector_random(b, N, 1LL, 100LL);
    initialize_vector_random(c, N, 1LL, 100LL);

    t_s = 1;
    for(LL i = 0; i < N - 1; i++)
    {
        a_s[i] = a_s[i + 1] + b[i] * c[i];
        t_s = t_s * a_s[i];
    }

    LL* a_p_copy = (LL*) malloc(sizeof(LL) * (N - 1));
    t_p = 1;
    omp_set_num_threads(th_numb);
    #pragma omp parallel
    {
        #pragma omp for
        for(LL i = 0; i < N - 1; i++)
            a_p_copy[i] = a_p[i + 1];

        #pragma omp for reduction(* : t_p)
        for(LL i = 0; i < N - 1; i++)
        {
            a_p[i] = a_p_copy[i] + b[i] * c[i];
            t_p = t_p * a_p[i];
        }
    }
    free(a_p_copy);

    std::cout << "t = " << t_p << std::endl;
    print_vector("a", a_p, N, std::cout);
    std::cout << (t_s == t_p && compare_vectors(a_s, a_p, N) ? "Correct!" : "False!") << std::endl;

    free(c);
    free(b);
    free(a_p);
    free(a_s);

    return 0;
}