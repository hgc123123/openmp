#include<omp.h>
#include<stdio.h>
int main(){
    omp_set_dynamic(0);
    #pragma omp parallel num_threads(4)
    {
        printf("num threads: %d\n",omp_get_num_threads());
    }
    return 0;
}
