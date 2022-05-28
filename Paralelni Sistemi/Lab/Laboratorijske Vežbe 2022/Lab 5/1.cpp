#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void init_matrix(int*A, int n, int m)
{
    #pragma omp parallel for shared(A, n, m)
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            A[i * m + j] = rand() % (n * m);
}
bool check_result(int*A, int*B, int*C, int n, int m, int p)
{
    bool ret = true;
    for(int i = 0; ret && i < n; i++)
    {
        for(int j = 0; ret && j < p; j++)
        {
            int tmp = 0;
            for(int k = 0; k < m; k++)
                tmp += A[i * m + k] * B[k * p + j];
            ret = C[i * p + j] == tmp;
        }
    }
    return ret;
}
void multiply(int*A, int*B, int*C, int i, int j, int n, int m, int p)
{
    int tmp = 0;
    for(int k = 0; k < m; k++)
        tmp += A[i * m + k] * B[k * p + j];
    C[i * p + j] = tmp;
}

int main(int argc, char** argv)
{
    const int n = 256;
    int A[n][n], B[n][n], C[n][n];

    init_matrix(&A[0][0], n, n);
    init_matrix(&B[0][0], n, n);

    double t1 = omp_get_wtime();

    for(int i = 0; i < n; i++)
    {
        #pragma omp parallel for shared(A, B, C, n)
        for(int j = 0; j < n; j++)
        {
            #pragma omp task if(false) untied shared(A, B, C, n)
            multiply(&A[0][0], &B[0][0], &C[0][0], i, j, n, n, n);
        }
    }

    double dt = omp_get_wtime() - t1;
    printf("Elapsed time: %f\n", dt);

    if(check_result(&A[0][0], &B[0][0], &C[0][0], n, n, n))
        printf("Correct\n");
    else
        printf("False!");

    return 0;
}