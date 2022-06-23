#include <omp.h>
#include "utilities.h"

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();
    LL N = 1 << 5, g_s, g_p, *x_s, *x_p, *y_s, *y_p, *z;

    switch(argc)
    {
        case(3): th_numb = atoi(argv[2]);
        case(2): N = atoll(argv[1]);
    }

    initialize_vector_random(x_s, N, 1LL, 100LL);
    initialize_vector_random(y_s, N, 1LL, 100LL);
    initialize_vector_random(z, N, 1LL, 100LL);

    copy_vector(x_s, x_p, N);
    copy_vector(y_s, y_p, N);

    g_s = 0;
    for(LL i = 1; i < N; i++)
    {
        y_s[i] = y_s[i] + x_s[i - 1]; 
        x_s[i] = x_s[i] + z[i];
        g_s += z[i - 1]; 
    }

    g_p = 0;
    // #pragma omp parallel num_threads(th_numb)
    // {
    //     #pragma omp for
    //     for(LL i = 1; i < N; i++)
    //         x_p[i] = x_p[i] + z[i];

    //     #pragma omp for reduction(+:g_p)
    //     for(LL i = 1; i < N; i++)
    //     {
    //         y_p[i] = y_p[i] + x_p[i - 1];
    //         g_p += z[i - 1];
    //     }
    // }

    x_p[0] = x_p[0] - z[0];
    #pragma omp parallel for reduction(+ : g_p)
    for(LL i = 1; i < N; i++)
    {
        x_p[i - 1] = x_p[i - 1] + z[i - 1];
        y_p[i] = y_p[i] + x_p[i - 1];
        g_p += z[i - 1];
    }
    x_p[N - 1] = x_p[N - 1] + z[N - 1];

    std::cout << "g_s = " << g_s << std::endl;
    std::cout << "g_p = " << g_p << std::endl;
    print_vector("x_s", x_s, N, std::cout);
    print_vector("x_p", x_p, N, std::cout);
    print_vector("y_s", y_s, N, std::cout);
    print_vector("y_p", y_p, N, std::cout);
    std::cout << (g_p == g_s &&
    compare_vectors(x_s, x_p, N)
    && compare_vectors(y_s, y_p, N) ? 
    "Correct!" 
    : 
    "False!") << std::endl;

    free(y_p);
    free(x_p);
    free(z);
    free(y_s);
    free(y_s);

    return 0;
}