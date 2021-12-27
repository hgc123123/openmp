programs' name is hello.cpp
=============================
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
