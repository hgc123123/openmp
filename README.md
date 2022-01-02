# OPENMP

OpenMP编程模型以线程为基础，通过编译制导指令制导并行化，有三种编程要素可以实现并行化控制，他们分别是编译制导、API函数集和环境变量。

编译程序使用的语句为

```
   gcc hello.cpp -o hello -fopenmp -lstdc++
```

使用intel编译程序使用的语句为

```

   module load intel-oneapi-compilers/2021.4.0
   module load intel-oneapi-mpi/2021.4.0
   module load intel-oneapi-mkl/2021.4.0

   icc -qopenmp -c pi.cpp -o pi.o
   icc -qopenmp pi.o  -o pi
   ./pi

```
## 编译制导

- 并行域控制类
- 任务分担类
- 同步控制类
- 数据环境类

编译制导指令以#pragma omp 开始，后边跟具体的功能指令，格式如：#pragma omp 指令[子句[,子句] …]。常用的功能指令如下：

parallel：用在一个结构块之前，表示这段代码将被多个线程并行执行；<u>[并行域控制类]</u>

```
#pragma omp parallel [for | sections] [子句[子句]...]

example:
#pragma omp parallel num_threads(8)
{
    printf("Hello world,thread id is: ",omp_get_thread_num());
}

```

for：用于for循环语句之前，表示将循环计算任务分配到多个线程中并行执行，以实行任务分担，保证每次循环之间无数据相关性,单独使用不会实行并行执行，也不会加快运行速度，需配合parallel,表示for循环的代码将会被多个线程执行。若一个parallel并行域中有多个for循环，则会被依次执行。 <u>[任务分担类]</u>
```
#pragma omp [parallel] for [子句]

example:
#pragma omp for
for(j=0;j<4;j++)	
    printf("j = %d,ThreadId = %d\n",j,omp_get_thread_num());

[username@node341 openmp]$ ./for
j=0,ThreadId=0
j=1,ThreadId=0
j=2,ThreadId=0
j=3,ThreadId=0
```

parallel for：parallel和for指令的结合，也是用在for循环语句之前，表示for循环体的代码将被多个线程并行执行，它同时具有并行域的产生和任务分担两个功能;
```
#pragma omp parallel
{
    #pragma omp for
    for(int j=0;j<4;j++)
        printf("j=%d,ThreadId=%d\n",j,omp_get_thread_num());	    
}
```

   sections：用在可被并行执行的代码段之前，用于实现多个结构块语句的任务分担，可并行执行的代码段各自用section指令标出（注意区分sections和section）;

   parallel sections：parallel和sections两个语句的结合，类似于parallel for;

   single：用在并行域内，表示一段只被单个线程执行的代码；

   critical：用在一段代码临界区之前，保证每次只有一个OpenMP线程进入；

   flush：保证各个OpenMP线程的数据影像的一致性；

   barrier：用于并行域内代码的线程同步，线程执行到barrier时要停下等待，直到所有线程都执行到barrier时才继续往下执行；

   atomic：用于指定一个数据操作需要原子性地完成；

   master：用于指定一段代码由主线程执行；

   threadprivate：用于指定一个或多个变量是线程专用，后面会解释线程专有和私有的区别。

## 相应的OpenMP子句为:

   private：指定一个或多个变量在每个线程中都有它自己的私有副本;

   firstprivate：指定一个或多个变量在每个线程都有它自己的私有副本，并且私有变量要在进入并行域或任务分担域时，继承主线程中的同名变量的值作为初值；

   lastprivate：是用来指定将线程中的一个或多个私有变量的值在并行处理结束后复制到主线程中的同名变量中，负责拷贝的线程是for或sections任务分担中的最后一个线程；

   reduction：用来指定一个或多个变量是私有的，并且在并行处理结束后这些变量要执行指定的归约运算，并将结果返回给主线程同名变量;

   nowait：指出并发线程可以忽略其他制导指令暗含的路障同步；

   num_threads：指定并行域内的线程的数目；

   schedule：指定for任务分担中的任务分配调度类型；

   shared：指定一个或多个变量为多个线程间的共享变量；

   ordered：用来指定for任务分担域内指定代码段需要按照串行循环次序执行；

   copyprivate：配合single指令，将指定线程的专有变量广播到并行域内其他线程的同名变量中；

   copyin：用来指定一个threadprivate类型的变量需要用主线程同名变量进行初始化；

   default：用来指定并行域内的变量的使用方式，缺省是shared。

## API函数

除上述编译制导指令之外，OpenMP还提供了一组API函数用于控制并发线程的某些行为，下面是一些常用的OpenMP API函数以及说明：

```
   omp_get_thread_num  返回线程的编号
   omp_get_num_threads 返回并行域中线程的总数
   omp_in_parallel     返回当前是否在并行域中
   omp_get_nested      判断系统是否支持并行嵌套
   omp_set_nested      启用或关闭并行嵌套
   omp_set_dynamic     启用或关闭线程数目的动态改变
```

## 环境变量

```
OMP_SCHEDULE、OMP_NUM_THREADS、OMP_DYNAMIC、OMP_NESTED等。

```
