#include <stdlib.h>
#include <iostream>
#include <time.h>

#define LL long long
#define ULL unsigned long long

template <typename T, typename K>
void initialize_vector(T*& v, const K N, const T& initial_value)
{
    v = (T*) malloc(sizeof(T) * N);
    for(K i = 0; i < N; i++)
        v[i] = initial_value;
}

template <typename T, typename K>
void initialize_vector_random(T*& v, const K N, const T& min, const T& max)
{
    v = (T*) malloc(sizeof(T) * N);
    for(K i = 0; i < N; i++)
        v[i] = rand() % (int)(max - min) + min;
}

template <typename T, typename K>
void copy_vector(const T* original, T*& copy, const K N)
{
    copy = (T*) malloc(sizeof(T) * N);
    for(K i = 0; i < N; i++)
        copy[i] = original[i];
}

template <typename T, typename K>
std::ostream& print_vector(const T* v, const K N, std::ostream& o)
{
    o << "|\t";
    for(K i = 0; i < N; i++)
        o << v[i] << "\t";
    o << "|" << std::endl;
    return o;
}

template <typename T, typename K>
std::ostream& print_vector(const char* label, const T* v, const K N, std::ostream& o)
{
    o << label << " = ";
    return print_vector(v, N, o);
}

template <typename T, typename K>
bool compare_vectors(const T* v1, const T* v2, const K N)
{
    bool r = true;
    for(K i = 0; r && i < N; i++)
        r = v1[i] == v2[i];
    return r;
}

template <typename T, typename K>
void initialize_matrix(T**& Matrix, const K N, const K M, const T& initial_value)
{
    Matrix = (T**) malloc(sizeof(T*) * N);
    for(LL i = 0; i < N; i++)
        initialize_vector(Matrix[i], M, initial_value);
}

template <typename T, typename K>
void initialize_matrix_random(T**& Matrix, const K N, const K M, const T& min, const T& max)
{
    Matrix = (T**) malloc(sizeof(T*) * N);
    for(K i = 0; i < N; i++)
        initialize_vector_random(Matrix[i], M, min, max);
}

template <typename T, typename K>
void copy_matrix(T** original, T**& copy, const K N, const K M)
{
    copy = (T**) malloc(sizeof(T*) * N);
    for(K i = 0; i < N; i++)
        copy_vector(original[i], copy[i], M);
}

template <typename T, typename K>
bool compare_matrices(T** A, T** B, const K N, const K M)
{
    bool  r = true;
    for(K i = 0; r && i < N; i++)
        for(K j = 0; r && j < M; j++)
            r = A[i][j] == B[i][j];
    return r;
}

template <typename T, typename K>
void destroy_matrix(T**& A, const K N)
{
    for(K i = 0; i < N; i++)
    {
        free(A[i]);
        A[i] = nullptr;
    }

    free(A);
    A = nullptr;
}

template <typename T, typename K>
std::ostream& print_matrix(T** A, const K N, const K M, std::ostream& o)
{
    for(K i = 0; i < N; i++)
        print_vector(A[i], M, o);
    return o;
}