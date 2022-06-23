#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    double sum = 1;
    #pragma omp parallel for reduction(* : sum) num_threads(atoi(argv[2])) schedule(dynamic, 2)
    for(int i = 1; i <= atoi(argv[1]); i++)
    {
        sum += i;
        printf("Thread[%d]: iteration{%d} sum = %f\n", omp_get_thread_num(), i, sum);
    }
    printf("The sum is %lf\n", sum);
}