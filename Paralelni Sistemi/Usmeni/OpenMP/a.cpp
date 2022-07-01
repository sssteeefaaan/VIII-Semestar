#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    omp_set_num_threads(4);
    int x_s, a_s, b_s, c_s, x_p, a_p, b_p, c_p;
    double dt;

    x_s = x_p = 3;
    c_s = c_p = 1;

    dt = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        {
            x_p++;
            #pragma omp task
            a_p = x_p + 2;
            #pragma omp task
            b_p = x_p + 5;
            #pragma omp task
            c_p++;
        }
    }
    dt = omp_get_wtime() - dt;
    printf("Time taken = %lf\n", dt);

    x_s++;
    c_s++;
    a_s = x_s + 2;
    b_s = a_s + 3;

    printf("x = (%d, %d)\n", x_s, x_p);
    printf("a = (%d, %d)\n", a_s, a_p);
    printf("b = (%d, %d)\n", b_s, b_p);
    printf("c = (%d, %d)\n", c_s, c_p);

    printf("\nBolji nacin\n\n");

    x_s = x_p = 3;
    c_s = c_p = 1;

    dt = omp_get_wtime();
    #pragma omp parallel sections firstprivate(x_p) lastprivate(x_p)
    {
        c_p++;
        #pragma omp section
        a_p = x_p + 3;
        #pragma omp section
        b_p = x_p + 6;
        #pragma omp section
        x_p++;
    }
    dt = omp_get_wtime() - dt;
    printf("Time taken = %lf\n", dt);

    x_s++;
    c_s++;
    a_s = x_s + 2;
    b_s = a_s + 3;

    printf("x = (%d, %d)\n", x_s, x_p);
    printf("a = (%d, %d)\n", a_s, a_p);
    printf("b = (%d, %d)\n", b_s, b_p);
    printf("c = (%d, %d)\n", c_s, c_p);

    return 0;
}