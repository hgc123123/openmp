# OPENMP
OpenMP编程模型以线程为基础，通过编译制导指令制导并行化，有三种编程要素可以实现并行化控制，他们分别是编译制导、API函数集和环境变量。

编译制导
---------

编译程序使用的语句为

.. code:: bash

   gcc hello.cpp -o hello -fopenmp -lstdc++
