#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

auto create_task = [] (int i) {
    return [i]() {
        printf("Zdravo%d %d\n", i, omp_get_thread_num());
    };
};

int main(int argc, char** argv)
{
    int proc_numb = omp_get_num_procs();

    using func_type = decltype(create_task(-1));
    std::vector<func_type> funcs;

    for(int i = 0; i < 2 * proc_numb; i++) funcs.push_back(create_task(i));

    printf("Initialized!\n");

    #pragma omp parallel for num_threads(proc_numb)
    for(int i = 0; i < funcs.size(); i++)
    {
        #pragma omp task if(-1)
        funcs[i]();
    }

    return printf("\n\nDone!\n\n"), 0;
}