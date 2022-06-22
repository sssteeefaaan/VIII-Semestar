#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int j;
#pragma omp threadprivate(j)
int main()
{
    j = 1;
    #pragma omp parallel copyin(j)
    {
        #pragma omp master
        j = 2;
    }
    printf("j = %d\n", j);
}