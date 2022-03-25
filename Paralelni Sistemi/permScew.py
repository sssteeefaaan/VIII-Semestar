def func(matrix, i, j):
    matrix[i][j] = matrix[i - 1][j] + matrix[i][j - 1]
    print(f"A({i}, {j}) <- A({i - 1}, {j}) + A({i}, {j - 1})")

def showMatrix(mat, n, m):
    for i in range(n):
        print("|", end="\t")
        for j in range(m):
            print(mat[i][j], end="\t")
        print("|")

def equalMatrices(A, B, N, M):
    ret = True
    i = 0
    while i < N and ret:
        j = 1
        while j < M and ret:
            ret = A[i][j] == B[i][j]
            j += 1
        i += 1
    return ret
        
def main():         
    N, M = 10, 4
    A = [[y * M + x for x in range(M)] for y in range(N)]
    B = list(A)
    
    for i in range(1, N):
        for j in range(1, M):
            func(A, i, j)

    print("\n\n")
    for j in range(1, M):
        for i in range(1 + j, N + j):
            func(B, i - j, j)
        print("-"*20)

    print(equalMatrices(A, B, N, M))

if __name__ == '__main__':
    main()