#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

float f(int i = 333) {
    if(!i)
        return 3.;
    return f(i - 1) + 2.;
}

float g(int i = 222) {
    if(!i)
        return 2.;
    return g(i - 1) + 3.;
}

int main()
{
    float y1 = 0, y2;
    omp_set_dynamic(0);
    omp_set_num_threads(omp_get_num_procs());

    #pragma omp parallel
    {
        #pragma omp single
        {
            y1 += f();
            #pragma omp task
            { y1 += g(); }
        }
    }

    #pragma omp parallel
    {
        float temp = 0;
        
        #pragma omp sections
        {
            temp = f();
            #pragma omp section
            temp = g();
        }

        #pragma omp critical (sekcija)
        y2 += temp;
    }

    printf("%lf\n", y1);
    printf("%lf\n", y2);

    return 0;
}