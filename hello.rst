programs' name is hello.cpp

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
