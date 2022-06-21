#include "utilities.h"
#include <omp.h>

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();

    LL N = 1 << 4, x_p, x_s, *b_p, *b_s, j, *a;

    if(argc > 1)
    {
        N = atoll(argv[1]);
        if(argc > 2)
            th_numb = atoi(argv[2]);
    }

    initialize_vector_random(b_p, N * 2 + 1, 1LL, 100LL);
    copy_vector(b_p, b_s, N * 2 + 1);
    initialize_vector_random(a, N, 1LL, 100LL);

    j = N + 1;
    x_s = 0;
    for(LL i = 0; i < N; i++)
    {
        x_s = x_s + a[i];
        b_s[i] = b_s[i] + b_s[j++];
    }

    omp_set_num_threads(th_numb);

    j = N + 1;
    x_p = 0;
    #pragma omp parallel for reduction(+ : x_p) firstprivate(j)
    for(LL i = 0; i < N; i++)
    {
        x_p = x_p + a[i];
        b_p[i] = b_p[i] + b_p[i + j];
    }

    std::cout << "x = " << x_p << std::endl;
    print_vector("b", b_p, N * 2 + 1, std::cout);

    std::cout << (x_p == x_s && compare_vectors(b_p, b_s, N * 2 + 1) ? "Correct!" : "False!") << std::endl;

    free(a);
    free(b_s);
    free(b_p);

    return 0;
}
