#include <stdlib.h>
#include <iostream>

#define LL long long
#define ULL unsigned long long

template <typename T, typename K>
void initialize_vector(T*& v, const LL n, const K& value)
{
    v = (T*) malloc(sizeof(T) * n);
    for(LL i = 0; i < n; i++)
        v[i] = value;
}

template <typename T>
std::ostream& print_vector(const char* label, const T* v, const LL n, std::ostream& o)
{
    o << label << " = |\t";
    for(LL i = 0; i < n; i++)
        o << v[i] << "\t";
    o << "|" << std::endl;
    return o;
}

template <typename T>
bool compare_vectors(const T* v1, const T* v2, const LL n)
{
    bool r = true;
    for(LL i = 0; r && i < n; i++)
        r = v1[i] == v2[i];
    return r;
}