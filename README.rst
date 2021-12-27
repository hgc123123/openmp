# OPENMP

OpenMP编程模型以线程为基础，通过编译制导指令制导并行化，有三种编程要素可以实现并行化控制，他们分别是编译制导、API函数集和环境变量。

编译程序使用的语句为

.. code:: bash

   gcc hello.cpp -o hello -fopenmp -lstdc++

编译制导
---------

编译制导指令以#pragma omp 开始，后边跟具体的功能指令，格式如：#pragma omp 指令[子句[,子句] …]。常用的功能指令如下：

   parallel：用在一个结构块之前，表示这段代码将被多个线程并行执行;
   
   for：用于for循环语句之前，表示将循环计算任务分配到多个线程中并行执行，以实现任务分担，必须由编程人员自己保证每次循环之间无数据相关性;

   parallel for：parallel和for指令的结合，也是用在for循环语句之前，表示for循环体的代码将被多个线程并行执行，它同时具有并行域的产生和任务分担两个功能;

   sections：用在可被并行执行的代码段之前，用于实现多个结构块语句的任务分担，可并行执行的代码段各自用section指令标出（注意区分sections和section）;

   parallel sections：parallel和sections两个语句的结合，类似于parallel for;

   single：用在并行域内，表示一段只被单个线程执行的代码；

   critical：用在一段代码临界区之前，保证每次只有一个OpenMP线程进入；

   flush：保证各个OpenMP线程的数据影像的一致性；

   barrier：用于并行域内代码的线程同步，线程执行到barrier时要停下等待，直到所有线程都执行到barrier时才继续往下执行；

   atomic：用于指定一个数据操作需要原子性地完成；

   master：用于指定一段代码由主线程执行；

   threadprivate：用于指定一个或多个变量是线程专用，后面会解释线程专有和私有的区别。
