#include <omp.h>
#include "utilities.h"

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();
    LL N = 1 << 5, x_s, x_p, *z_s, *z_p;

    switch(argc)
    {
        case(3): th_numb = atoi(argv[2]);
        case(2): N = atoll(argv[1]);
    }

    initialize_vector_random(z_s, N, 1LL, 100LL);
    copy_vector(z_s, z_p, N);

    x_s = 1;
    for(LL k = 0; k < N; k++)
    {
        z_s[k] = k + x_s;
        x_s = k;
    }

    omp_set_num_threads(th_numb);
    #pragma omp parallel for
    for(LL k = 0; k < N; k++)
        z_p[k] = k + abs(k - 1);
    x_p = N - 1;

    std::cout << "x = " << x_p << std::endl;
    print_vector("z", z_p, N, std::cout);
    std::cout << (x_s == x_p && compare_vectors(z_s, z_p, N) ? "Correct!" : "False!") << std::endl;

    free(z_p);
    free(z_s);

    return 0;
}