#programs' name is hello.cpp
=============================

##pragma omp parallel 
-------------------

parallel制导指令用来创建并行域，后边跟一个大括号，将并行执行的代码放在一起

.. code:: bash

   #include<iostream>
   #include<omp.h>
   using namespace std;
   int main()
   {
       #pragma omp parallel
       {
           cout<<"Test "<<endl;
       };
       return 0;
   }

   gcc hello.cpp -o hello -fopenmp -lstdc++

   ./hello
   [hpchgc@cas517 OpenMP]$ ./test
   Test Test Test Test 
   Test Test Test 


   Test 

   Test 


   Test

.. code:: bash

   #include<iostream>
   #include<omp.h>
   using namespace std;
   int main()
   {
       #pragma omp parallel num_threads(6)
       {
           cout<<"Test "<<endl;
       };
       return 0;
   }

   gcc hello.cpp -o hello -fopenmp -lstdc++

   ./hello
   [hpchgc@cas517 OpenMP]$ ./test
   Test Test Test
   Test

   Test

   Test

上述程序输出之所以会出现空行是因为在其中一个线程输出字符“Test”之后还没有来得及换行时，另一个线程直接输出了字符“Test”。

pragma omp parallel for
-------------------------

parallel for用来生成一个并行域，并将多个任务在并行域之间分配，可以使用系统默认线程数，也可以使用num_threads指定

.. code:: bash

   #include<iostream>
   #include<omp.h>
   #include<stdio.h>
   using namespace std;
   int main()
   {
       #pragma omp parallel for num_threads(2)
       for (int i=0;i<12;i++)
       {
           printf("线程的编号为：%d\n",omp_get_thread_num());
       }
       return 0;
   }
   [hpchgc@cas517 OpenMP]$ ./test
   线程的编号为：0
   线程的编号为：0
   线程的编号为：0
   线程的编号为：0
   线程的编号为：0
   线程的编号为：0
   线程的编号为：1
   线程的编号为：1
   线程的编号为：1
   线程的编号为：1
   线程的编号为：1
   线程的编号为：1

OpenMP效率提升以及不同线程数效率对比
-------------------------------------

.. code:: bash

   #include<iostream>
   #include<stdio.h>
   #include<omp.h>
   using namespace std;

   void test()
   {
       for(int i=0;i<80000;i++)
       {

       }
   }

   int main()
   {
       float startTime=omp_get_wtime();
       #pragma omp parallel for num_threads(2)
       for (int i=0;i<80000;i++)
       {
           test();
       }
       float endTime=omp_get_wtime();
       printf("指定2个线程的执行时间是：%f\n",endTime-startTime);

       startTime=omp_get_wtime();
       #pragma omp parallel for num_threads(4)
       for (int i=0;i<80000;i++)
       {
           test();

       }
       endTime=omp_get_wtime();
       printf("指定4个线程的执行时间是：%f\n",endTime-startTime);
       startTime=omp_get_wtime();
       #pragma omp parallel for num_threads(8)
       for (int i=0;i<80000;i++)
       {
           test();
       }
       endTime=omp_get_wtime();
       printf("指定8个线程的执行时间是：%f\n",endTime-startTime);
    
       return 0;

   }

   [hpchgc@cas517 OpenMP]$ ./test
   指定2个线程的执行时间是：4.750000
   指定4个线程的执行时间是：2.500000
   指定8个线程的执行时间是：1.250000
