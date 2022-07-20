#include <omp.h>
#include "utilities.h"
#include <sstream>

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();
    LL N = 1 << 5;

    switch (argc)
    {
        case 3: th_numb = atoi(argv[2]);
        case 2: N = atoll(argv[1]);
    }

    LL* a_s, *a_p;
    initialize_vector_random(a_s, N + 10, 1LL, 100LL);
    copy_vector(a_s, a_p, N + 10);

    for(int i = 0; i < N; i++)
        a_s[i + 10] = a_s[i] + 1;

    omp_set_num_threads(th_numb);
    
    #pragma omp parallel for
    for(LL i = 0; i < N; i++)
        a_p[i + 10] = a_p[i % 10] + i / 10 + 1;

    std::cout << (compare_vectors(a_s, a_p, N + 10) ? "Correct!" : "False!") << std::endl;

    free(a_p);
    free(a_s);

    return 0;
}