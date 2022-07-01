#include <omp.h>
#include <stdio.h>

int main()
{
    int a = 10;
    #pragma omp parallel num_threads(3) shared(a)
    {
        #pragma omp for firstprivate(a)
        for(int i = 0; i < a; i++)
        printf("T[%d]: %d, %d\n", omp_get_thread_num(), i, a);
    }
    return 0;
}