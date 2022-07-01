#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int i = 20, j = 22;
#pragma omp threadprivate(i, j)

int main(int argc, char** argv)
{
    omp_set_dynamic(1);
    omp_set_num_threads(9);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("T[%d]: (%d, %d)\n", tid, i, j);
        i = tid;
    }

    printf("\n\n");
    i = 55, j = 66;

    #pragma omp parallel copyin(j)
    {
        printf("T[%d]: (%d, %d)\n", omp_get_thread_num(), i, j);
    }

    printf("\n\n");

    #pragma omp parallel
    {
        printf("T[%d]: (%d, %d)\n", omp_get_thread_num(), i, j);
    }

    return printf("\n\nDone!\n\n"), 0;
}