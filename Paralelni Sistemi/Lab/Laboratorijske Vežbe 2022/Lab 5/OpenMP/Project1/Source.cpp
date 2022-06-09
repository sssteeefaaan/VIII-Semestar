#include <omp.h>
#include <stdio.h>

int main()
{
    int i = 1, m;
#pragma omp parallel for private(m)
    for (m = 2; m < 5; m++)
    {
#pragma omp barrier
        i *= m;
    }

    printf("%d", &i);
    return 0;
}