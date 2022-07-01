#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int N = 1 << 3,
    th_num = omp_get_num_procs();

    switch(argc)
    {
        case(3):th_num = atoi(argv[2]);
        case(2):  N = atoi(argv[1]);
    }

    #pragma omp parallel for schedule(static) num_threads(th_num)
    for(int i = 0; i < N; i++)
        printf("T[%d]: %d\n", omp_get_thread_num(), i);

    return 0;
}