#include <omp.h>
#include <iostream>
#include <stdlib.h>

#define LL long long
#define ULL unsigned LL

template <typename T>
void init_vector(T*& v, const LL N)
{
    v = (T*) malloc(sizeof(T) * N);
    for(LL i = 0; i < N; i++)
        v[i] = rand() % 5 + 1;
}

template <typename T>
void print_vector(const char* label, const T* v, const ULL n)
{
    using namespace std;
    cout << label << " = |\t";
    for(ULL i = 0; i < n; i++)
        cout << v[i] << "\t";
    cout << "|" << endl;
}

int main(int argc, char** argv)
{
    LL N, offset, prod, 
    *a, *b, *c, *x;

    if(argc > 1)
    {
        N = atoll(argv[1]);
        if(argc > 2)
            offset = atoll(argv[2]);
    }

    init_vector(a, N + 1);
    init_vector(b, N);
    init_vector(c, N + offset);
    init_vector(x, N);

    omp_set_num_threads(omp_get_num_procs());

    LL *a_copy = (LL*)malloc(sizeof(LL) * N);
    #pragma omp parallel for
    for(LL i = 0; i < N; i++)
        a_copy[i] = a[i + 1];
    
    prod = 1;
    #pragma omp parallel for reduction(*:prod)
    for(LL i = 0; i < N; i++)
    {
        a[i] = a_copy[i] + b[i] * c[offset + N - i - 1];
        prod *= x[i];
    }

    std::cout << "prod = " << prod << std::endl;
    print_vector("a", a, N + 1);

    free(a_copy);
    free(x);
    free(c);
    free(b);
    free(a);

    return 0;
}