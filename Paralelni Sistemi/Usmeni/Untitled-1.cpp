int k, j, i, ***A, ***B, ***C, X1, X2, X3, L, M, N, u, v, w;

int main()
{
    for(k = 0; k < L; k++)
        for(j = 0; j < M; j++)
            for(i = 0; i < N; i++)
            {
                A[i][j][k + 1] = A[i][j][k] + X1;
                B[i + 1][j][k] = B[i][j][k] + X2;
                C[i + 1][j + 1][k + 1] = C[i][j][k] + X3;
            }

    for(u = 0; u < L; u++)
        for(w = 0; w < M; w++)
            for(v = 0; v < N; v++)
            {
                A[v][w][u + 1] = A[v][w][u] + X1;
                B[v + 1][w][u] = B[v][w][u] + X2;
                C[v + 1][w + 1][u + 1] = C[v][w][u] + X3;
            }
}