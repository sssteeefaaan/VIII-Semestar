#include <omp.h>
#include <stdio.h>

int k = 3;
#pragma omp threadprivate(k)
int main(int argc, char** argv)
{
    k = 5;
    omp_set_dynamic(0);
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        printf("T[%d]: %d\n", omp_get_thread_num(), k);
        #pragma omp single copyprivate(k)
            printf("\nT[%d]: I'm here >:D!\n\n", k = omp_get_thread_num());
    }

    printf("\n\n");

    #pragma omp parallel
    {
        printf("T[%d]: %d\n", omp_get_thread_num(), k);
        #pragma omp atomic
            k++;
        printf("Hey %d\n", k);
    }

    omp_nest_lock_t lock;
    omp_init_nest_lock(&lock);
    omp_set_nest_lock(&lock);
    omp_unset_nest_lock(&lock);
    omp_destroy_nest_lock(&lock);

    printf("Done!\n");

    return 0;
}