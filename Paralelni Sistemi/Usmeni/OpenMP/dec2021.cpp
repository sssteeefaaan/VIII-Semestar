#include <omp.h>
#include "utilities.h"

void sub_a(float* a, float* b, float* c, float* d, int n, int m)
{
    #pragma omp for collapse(2)
    for(int i = 1; i < n; i++)
        for(int j = 0; j <= i; j++)
            b[j * n + i] = (a[j * n + i] - a[j * n + i - 1]) / 2.;
 
    #pragma omp for collapse(2)
    for(int i = 1; i < m; i++)
        for(int j = 0; j <= i; j++)
            d[j * m + i] = (c[j * m + i] - c[j * m + i - 1]) / 2.;
}
int main(int argc, char** argv)
{
    int n = 1 << 5,
    m = (1 << 6) + 3,
    th_numb = omp_get_num_procs();

    switch(argc)
    {
        case(4): th_numb = atoi(argv[3]);
        case(3): m = atoi(argv[2]);
        case(2): n = atoi(argv[1]);
    }

    float *a_s, *a_p, *c_s, *c_p, *b_s, *b_p, *d_s, *d_p;

    initialize_vector(b_s, n * n, 1.f);
    initialize_vector(b_p, n * n, 1.f);
    initialize_vector(d_s, m * m, 1.f);
    initialize_vector(d_p, m * m, 1.f);
    initialize_vector_random(a_s, n * n, 1.f, 100.f);
    initialize_vector_random(c_s, m * m, 1.f, 100.f);
    copy_vector(a_s, a_p, n * n);
    copy_vector(c_s, c_p, m * m);

    double dt;
    dt = omp_get_wtime();
    sub_a(a_s, b_s, c_s, d_s, n, m);
    dt = omp_get_wtime() - dt;
    printf("Time taken: %lf\n", dt);

    omp_set_dynamic(1);
    omp_set_num_threads(th_numb);
    dt = omp_get_wtime();
    #pragma omp parallel
    sub_a(a_p, b_p, c_p, d_p, n, m);
    dt = omp_get_wtime() - dt;
    printf("Time taken: %lf\n", dt);

    compare_vectors(b_s, b_p, n * n)
    && 
    compare_vectors(d_s, d_p, m * m)
    ?
    printf("Correct!\n")
    :
    printf("False!\n");

    free(c_p);
    free(a_p);
    free(c_s);
    free(a_s);
    free(d_p);
    free(d_s);
    free(b_p);
    free(b_s);

    return 0;
}