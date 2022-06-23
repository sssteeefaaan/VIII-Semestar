#include <omp.h>
#include <stdio.h>

int main()
{
    int i;
    double sum = 0.0;
    #pragma omp parallel for shared (sum)
    {
        for(i = 1; i <= 4; i++)
        sum = sum + i;
    }
    printf("The sum is %lf\n", sum);
}