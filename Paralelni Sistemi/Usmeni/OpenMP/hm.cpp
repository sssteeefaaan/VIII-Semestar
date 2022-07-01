#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    const int N = 10;
    int *A = (int*)malloc(sizeof(int) * N * N);

    omp_set_nested(1);
    omp_set_dynamic(0);
    omp_set_num_threads(N);
    #pragma omp parallel
    {
        int i = omp_get_thread_num();
        #pragma omp parallel
        { 
            int ind = i * N + omp_get_thread_num();
            A[ind] = 1;
        }
    }

    printf("Done!\n\n");

    printf("A\n");
    for(int i = 0; i < N; i++)
    {
        printf("|\t");
        for(int j = 0; j < N; j++)
            printf("%d\t", A[i * N + j]);
        printf("|\n");
    }

    free(A);

    return 0;
}