#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include"omp.h"
using namespace std;
void test()
{
    for (int i = 0; i < 80000; i++)
    {
    }
}

int main()
{
    float startTime = omp_get_wtime();
    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < 10000; i++)
    {
        test();
    }
    float endTime = omp_get_wtime();
    printf("指定 2 个线程，执行时间: %f\n", endTime - startTime);
    startTime = endTime;

    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < 100; i++)
    {
        for (int j=0;j<100;j++)
 	{
            test();
	}
    }

    endTime = omp_get_wtime();
    printf("指定 2 个线程，双循环的执行时间: %f\n", endTime - startTime);
    startTime = endTime;
 	
    for (int i = 0; i < 100; i++)
    {
        for (int j=0;j<100;j++)
        {
            test();
	}
    }
    endTime = omp_get_wtime();
    printf("不使用OpenMP多线程，双循环的执行时间: %f\n", endTime - startTime);
    startTime = endTime;
    return 0;
}

[username@node169 openmp]$ ./time2
指定 2 个线程，执行时间: 0.968750
指定 2 个线程，双循环的执行时间: 0.968750
不使用OpenMP多线程，双循环的执行时间: 1.937500
