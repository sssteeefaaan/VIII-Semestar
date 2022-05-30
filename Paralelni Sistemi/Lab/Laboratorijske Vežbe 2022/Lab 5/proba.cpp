#include <omp.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int t = 2;
    omp_set_num_threads(4);
    #pragma omp parallel reduction(*:t)
    {
        printf("Thread[%d]: My value of t is %d\n", omp_get_thread_num(), t);
        t += 2;
    }
    printf("The value of t is %d\n", t);

    return 0;
}