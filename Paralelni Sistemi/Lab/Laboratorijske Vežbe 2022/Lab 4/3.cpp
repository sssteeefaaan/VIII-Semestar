#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpich/mpi.h>

int initialize_matrix(int**A, int n, int m, int def = -1);
int array_sum(int* v, int n, int* sum);
int print_matrix(int* A, int n, int m);

int main(int argc, char** argv)
{
    const int v = 10, m = 5;
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm comm1;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 3, rank, &comm1);
    if(rank % 3 == 0)
    {
        int *A, *local_A, comm1_rank, sum, n;
        MPI_Comm_rank(comm1, &comm1_rank);
        MPI_Comm_size(comm1, &n);

        if(!comm1_rank){
            initialize_matrix(&A, n, m);
            // print_matrix(A, n, m);
            // fflush(stdout);
        }

        initialize_matrix(&local_A, 1, m, 0);
        MPI_Scatter(A, m, MPI_INT, local_A, m, MPI_INT, 0, comm1);

        if(!comm1_rank)
            free(A);

        array_sum(local_A, m, &sum);
        // printf("P[%d](%d): ", rank, comm1_rank);
        // print_matrix(local_A, 1, m);
        // fflush(stdout);
        free(local_A);

        if(sum > v)
        {
            printf("P[%d](%d): %d\n", rank, comm1_rank, sum);
            fflush(stdout);
        }
    }

    MPI_Finalize();
    return 0;
}

int initialize_matrix(int**A, int n, int m, int def)
{
    (*A) = (int*)malloc(sizeof(int) * n * m);
    if(def == -1)
        for(int i = 0; i < n * m; (*A)[i++] = i);
    else
        for(int i = 0; i < n * m; (*A)[i++] = def);
    return 1;
}

int array_sum(int* v, int n, int* sum)
{
    (*sum) = 0;
    for(int i = 0; i < n; (*sum) += v[i++]);
    return 1;
}

int print_matrix(int* A, int n, int m)
{
    for(int i = 0; i < n; i++)
    {
        printf("|\t");
        for(int j = 0; j < m; j++)
            printf("%d\t", A[i * m + j]);
        printf("|\n");
    }
    return 1;
}