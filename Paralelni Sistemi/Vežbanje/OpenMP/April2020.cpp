#include <omp.h>
#include "utilities.h"

template <typename K>
K f(K);

int main(int argc, char** argv)
{
    int th_numb = omp_get_num_procs();

    LL N = 1 << 5, start = 10, step = 2, sum_s, sum_p, v_s, v_p;

    switch(argc)
    {
        case(5):
            th_numb = atoi(argv[4]);
        case(4):
            step = atoll(argv[3]);
        case(3):
            start = atoll(argv[2]);
        case(2):
            N = atoll(argv[1]);
    }

    v_s = start;
    sum_s = 0;
    for(LL i = 0; i < N; i++)
    {
        sum_s = sum_s + f(v_s);
        v_s = v_s + step;
    }

    omp_set_num_threads(th_numb);
    v_p = start;
    sum_p = 0;
    #pragma omp parallel for reduction(+ : sum_p, v_p)
    for(LL i = 0; i < N; i++)
    {
        sum_p = sum_p + f(start + i * step);
        v_p = v_p + step;
    }

    std::cout << (v_s == v_p && sum_s == sum_p ? "Correct!" : "False!") << std::endl;

    return 0;
}

template <typename K>
K f(K k)
{
    return k * k;
}