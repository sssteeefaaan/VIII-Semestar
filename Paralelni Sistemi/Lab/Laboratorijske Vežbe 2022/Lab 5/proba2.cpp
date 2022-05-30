#include <omp.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int i = 1;
    #pragma omp parallel for shared(i)
    for(int m = 2; m < 5; m++)
    {
        i *= m;
        printf("HMMMMMM\n");
    }
    
    printf("The address of i is %d\n", i);

    return 0;
}