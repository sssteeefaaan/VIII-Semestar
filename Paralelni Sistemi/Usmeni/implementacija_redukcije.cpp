int main()
{
    double S = 0;
    #pragma omp parallel sections reduction(+:S)
    {
        // #pragma omp section
        S += f1();
        #pragma omp section
        S += f2();
        #pragma omp section
        S += f3();
    }

    double S = 0;
    #pragma omp parallel
    {
        double temp = 0;
        #pragma omp sections
        {
            // #pragma omp section
            temp += f1();
            #pragma omp section
            temp += f2();
            #pragma omp section
            temp += f3();
        }

        #pragma omp critical (section1)
        S += temp;
    }

    double* x;
    int i, N;


    double P = 1;
    #pragma omp parallel for reduction(*:P)
    for(i = 0; i < N; i++)
        P *= x[i];

    double P = 1;
    #pragma omp parallel
    {
        double temp = 1;
        #pragma omp for
        for(int i = 0; i < N; i++)
            temp *= x[i];

        #pragma omp critical (sekcija1)
        P *= temp;
    }
}