#include <omp.h>
#include "utilities.h"

int main(int argc, char** argv)
{
    double dt;
    int th_numb = omp_get_num_procs();
    LL N = 1 << 5, s_s, s_p, p_s, p_p, step = 2, start = 20;

    switch(argc)
    {
        case(5): step = atoll(argv[4]);
        case(4): start = atoll(argv[3]);
        case(3): th_numb = atoi(argv[2]);
        case(2): N = atoll(argv[1]);
    }

    dt = omp_get_wtime();
    s_s = start;
    p_s = start;
    for(LL i = 1; i < N; i++)
    {
        s_s += i;
        p_s *= i;
    }
    std::cout <<"[Serial] Time taken: " << omp_get_wtime() - dt << std::endl;


    omp_set_num_threads(th_numb);

    dt = omp_get_wtime();
    s_p = start;
    p_p = start;
    #pragma omp parallel for reduction(+ : s_p) reduction(* : p_p)
    for(LL i = 1; i < N; i++)
    {
        s_p += i;
        p_p *= i;
    }
    std::cout <<"[Parallel] Time taken: " << omp_get_wtime() - dt << std::endl;

    std::cout << (s_p == s_s && p_s == p_p ? "Correct!" : "False!") << std::endl
                << s_p << std::endl << p_p << std::endl;

    LL *a_s, *a_p;
    initialize_vector_random(a_s, N, 1LL, 100LL);
    copy_vector(a_s, a_p, N);
    std::cout << "Initialization successful!" << std::endl;

    dt = omp_get_wtime();
    for(LL i = 1; i < N; i++)
        a_s[i] = 2 * a_s[i - 1];
    std::cout <<"[Serial] Time taken: " << omp_get_wtime() - dt << std::endl;

    dt = omp_get_wtime();
    #pragma omp parallel for ordered
    for(LL i = 1; i < N; i++)
        #pragma omp ordered threads
        a_p[i] = 2 * a_p[i - 1];
    std::cout <<"[Parallel] Time taken: " << omp_get_wtime() - dt << std::endl;

    std::cout << (compare_vectors(a_s, a_p, N) ? "Correct!" : "False!") << std::endl;

    free(a_p);
    free(a_s);

    #pragma omp parallel for ordered
    for(const char* i : {"Mogu", "li", "nesto", "da", "te", "pitam", "?"})
        #pragma omp ordered
        std::cout << i << " ";
    std::cout << std::endl;

    return 0;
}