#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    int c1 = 0, c2 = 0, i;
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        #pragma omp atomic
        c1++;

        printf("par: %d\n", omp_get_thread_num());

        #pragma omp master
        {
            #pragma omp critical
            {
                c2++;
                printf("master: %d\n", omp_get_thread_num());
            }
        }

        #pragma omp single
        {
            #pragma omp critical
            {
                printf("single: %d\n", omp_get_thread_num());
                c2++;
            }
        }

        printf("c1: %d, c2: %d\n", c1, c2);
    }

    return 0;
}