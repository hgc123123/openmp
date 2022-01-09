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

```
:set paste

:set nopaste
```
## 编译制导

- 并行域控制类
- 任务分担类
- 同步控制类(互斥锁和事件同步类)
- 数据环境类(与指令parallel、for和section相结合，控制变量的作用范围)

编译制导指令以#pragma omp 开始，后边跟具体的功能指令，格式如：#pragma omp 指令[子句[,子句] …]。常用的功能指令如下：

parallel：用在一个结构块之前，表示这段代码将被多个线程并行执行；__并行域控制类__

```
#pragma omp parallel [for | sections] [子句[子句]...]

example:
#pragma omp parallel num_threads(8)
{
    printf("Hello world,thread id is: ",omp_get_thread_num());
}

```

for：用于for循环语句之前，表示将循环计算任务分配到多个线程中并行执行，以实行任务分担，保证每次循环之间无数据相关性,单独使用不会实行并行执行，也不会加快运行速度，需配合parallel,表示for循环的代码将会被多个线程执行。若一个parallel并行域中有多个for循环，则会被依次执行。 __任务分担类__
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

sections：用在可被并行执行的代码段之前，用于实现多个结构块语句的任务分担，可并行执行的代码段各自用section指令标出（注意区分sections和section）,用于非迭代计算的任务分类。 __任务分担类__

parallel sections：parallel和sections两个语句的结合，类似于parallel for;

```
#pragma omp parallel sections
{
    #pragma omp setion
    {    printf("Section 1 thread = %d\n",omp_get_thread_num());}
    #pragma omp section
    {    printf("Section 2 thread = %d\n",omp_get_thread_num());}
}

[username@node341 openmp]$ ./sections
Section 1 thread = 0
Section 2 thread = 1
```

single：用在并行域内，表示一段只被单个线程执行的代码,没有nowait则其它线程需在该指令结束处隐式同步点同步，否则其它线程继续向下执行,__nowait：不需要等待，下面的线程可以先于它执行执行__
```
#pragma omp parallel
{	
    #pragma omp single
        printf("Beginning work 1.\n");
    printf("Work on 1 parallely.%d\n",omp_get_thread_num());
    #pragma omp single
        printf("Finishing work 1.\n");
    #pragma omp single nowait
        printf("Beginning work 2.\n");
    printf("Work on 2 parallely.%d\n",omp_get_thread_num());
}


[username@node668 openmp]$ ./single 
Beginning work 1.
Work on 1 parallely.0
Finishing work 1.
Work on 1 parallely.3
Work on 1 parallely.2
Work on 1 parallely.1
Work on 2 parallely.3
Beginning work 2.
Work on 2 parallely.2
Work on 2 parallely.1
Work on 2 parallely.0
```

互斥锁：有critical、atomic和API中的互斥函数
critical：用在一段代码临界区之前，保证每次只有一个OpenMP线程进入；__同步控制类(互斥锁)__
```
#include<stdio.h>
#include<omp.h>
int main(){
    int arx[]={1,2,3,4,5};
    int ary[]={4,6,7,8,9};
    int max_num_x=0;
    int max_num_y=0;
    #pragma omp parallel for
    for(int i=0;i<5;i++)
    {
        #pragma omp critical (max_arx)
	if(arx[i]>max_num_x)
    	    max_num_x=arx[i];
	#pragma omp critical (max_ary)
	if(ary[i]>max_num_y)
	    max_num_y=ary[i];	
    }
    printf("max_x=%d,max_y=%d\n",max_num_x,max_num_y);
    return 0;
}

[username@node668 openmp]$ ./critical 
max_x=5,max_y=9


#include<stdio.h>
#include<omp.h>
int main()
{
    int i;
    const int N=1000;
    int sum;
    #pragma omp parallel for
    for(i=0;i<N;i++)
    {
	#pragma omp critical(sum)
        sum+=i;
    }
    printf("reduction sum = %d (expected %d)\n",sum,((N-1)*N)/2);
    return 0;
}

[user@node171 openmp]$ ./reduction
reduction sum = 499500 (expected 499500)
```

   flush：保证各个OpenMP线程的数据影像的一致性；

barrier：用于并行域内代码的线程同步，线程执行到barrier时要停下等待，直到所有线程都执行到barrier时才继续往下执行；__事件同步__

atomic：用于指定一个数据操作需要原子性地完成。__同步控制类—互斥锁__
```
#include<stdio.h>
#include<omp.h>
int main(){
    int counter=0;
    #pragma omp parallel
    {
        for(int i=0;i<5000;i++)
        {
            #pragma omp atomic
            counter++;
        }
    }
    printf("counter=%d\n",counter);
    return 0;
}

[username@node668 openmp]$ ./atomic 
counter=10000


#include<stdio.h>
#include<omp.h>
int main()
{
    int i;
    const int N=1000;
    int sum;
//    #pragma omp parallel for private(i) reduction(+: sum)
    #pragma omp parallel for
    for(i=0;i<N;i++)
    {
	#pragma omp atomic
        sum+=i;
    }
    printf("reduction sum = %d (expected %d)\n",sum,((N-1)*N)/2);
    return 0;
}

[user@node171 openmp]$ ./reduction
reduction sum = 499500 (expected 499500)
```

互斥锁函数：需释放相应锁空间，否则可能造成多线程程序的死锁
```
#include<stdio.h>
#include<omp.h>
static omp_lock_t lock;
int main()
{
    int i;
    omp_init_lock(&lock);
    #pragma omp parallel for
    for(int i=0;i<5;i++)
    {
        omp_set_lock(&lock);
	printf("%d + \n",omp_get_thread_num());
	printf("%d     - \n",omp_get_thread_num());
	omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
    return 0;
}

[username@node169 openmp]$ ./lock
3 +
3     -
4 +
4     -
2 +
2     -
1 +
1     -
0 +
0     -
```

reduction:规约操作
```
#pragma omp reduction(operator: variable)
One of: +, *, -, / (and &, ^, |, &&, ||)

#include<stdio.h>
#include<omp.h>
int main()
{
    int i;
    const int N=1000;
    int sum;
    #pragma omp parallel for private(i) reduction(+: sum)
    for(i=0;i<N;i++)
    {
        sum+=i;
    }
    printf("reduction sum = %d (expected %d)\n",sum,((N-1)*N)/2);
    return 0;
}

[user@node171 openmp]$ ./reduction 
reduction sum = 499500 (expected 499500)
```


master：用于指定一段代码由主线程执行；__事件同步__
```
#include<stdio.h>
#include<omp.h>
int main()
{
     int i,a[5];
     #pragma omp parallel
     {
         #pragma omp for 
	 for(i=0;i<5;i++)
         {
	     a[i]=i*i;
	 }
	 #pragma omp master
	 for(i=0;i<5;i++)
 	 {
	     printf("a[%d] = %d,threadId = %d\n",i,a[i],omp_get_thread_num());
	 }
      }
      return 0;
}

[username@node169 openmp]$ ./mater
a[0] = 0,threadId = 0
a[1] = 1,threadId = 0
a[2] = 4,threadId = 0
a[3] = 9,threadId = 0
a[4] = 16,threadId = 0
```

threadprivate：用于指定一个或多个变量是线程专用，后面会解释线程专有和私有的区别。
```
#include<stdio.h>
#include<omp.h>
int A=100;
#pragma omp threadprivate(A)

int main()
{
    #pragma omp parallel for
    for(int i=0;i<10;i++)
    {
        A++;
	printf("Thread id is: %d,%d,%d\n",omp_get_thread_num(),i,A);
    }
    printf("Global A: %d\n",A);
    #pragma omp parallel for
    for(int i=0;i<10;i++)
    {
        A++;
	printf("Thread id is: %d,%d,%d\n",omp_get_thread_num(),i,A);
    }
    printf("Global A: %d\n",A);
    return 0;
}

[user@node169 openmp]$ ./threadprivate
Thread id is: 0,0,101
Thread id is: 2,2,101
Thread id is: 1,1,101
Thread id is: 7,7,101
Thread id is: 6,6,101
Thread id is: 3,3,101
Thread id is: 4,4,101
Thread id is: 9,9,101
Thread id is: 5,5,101
Thread id is: 8,8,101
Global A: 101
Thread id is: 5,5,102
Thread id is: 0,0,102
Thread id is: 3,3,102
Thread id is: 8,8,102
Thread id is: 6,6,102
Thread id is: 4,4,102
Thread id is: 1,1,102
Thread id is: 9,9,102
Thread id is: 7,7,102
Thread id is: 2,2,102
Global A: 102
```

## 相应的OpenMP子句为:

private：指定一个或多个变量在每个线程中都有它自己的私有副本, __环境变量__
```
#include<stdio.h>
#include<omp.h>
int main()
{
    int k=100;
    #pragma omp parallel for private(k)
    for(int i=0;i<5;i++)
    {
        k+=i;
        printf("k=%d\n",k);
    }
    printf("last k is %d\n",k);
    return 0;
}
[user@node169 openmp]$ ./private
k=0
k=4
k=3
k=1
k=2
last k is 100

#include<stdio.h>
#include<omp.h>
int main()
{
    int A=100;
    #pragma omp parallel for private(A)
    for(int i=0;i<10;i++)
    {
        printf("Thread ID: %d, %d, %d\n",omp_get_thread_num(),i,A);
	A=i;
    }
    printf("%d\n",A);
    return 0;
}
[username@node169 openmp]$ ./private
Thread ID: 4, 4, 0
Thread ID: 9, 9, 0
Thread ID: 8, 8, 0
Thread ID: 3, 3, 0
Thread ID: 2, 2, 0
Thread ID: 7, 7, 0
Thread ID: 1, 1, 0
Thread ID: 5, 5, 0
Thread ID: 6, 6, 0
Thread ID: 0, 0, 0
100
```

firstprivate：指定一个或多个变量在每个线程都有它自己的私有副本，并且私有变量要在进入并行域或任务分担域时，继承主线程中的同名变量的值作为初值；
```
#include<stdio.h>
#include<omp.h>
int main()
{
    int k=100;
    #pragma omp parallel for firstprivate(k)
    for(int i=0;i<5;i++)
    {
	k+=i;
        printf("k=%d\n",k);
    }
    printf("last k is %d\n",k);
    return 0;
}
[user@node169 openmp]$ ./firstprivate 
k=100
k=101
k=102
k=103
k=104
last k is 100

#include<stdio.h>
#include<omp.h>
int main()
{
    int A=100;
    #pragma omp parallel for firstprivate(A)
    for(int i=0;i<10;i++)
    {
	printf("Thread ID: %d, %d, %d\n",omp_get_thread_num(),i,A);
	A=i;
    }
    printf("%d\n",A);
    return 0;
}
[user@node169 openmp]$ ./firstprivate 
Thread ID: 7, 7, 100
Thread ID: 5, 5, 100
Thread ID: 8, 8, 100
Thread ID: 1, 1, 100
Thread ID: 3, 3, 100
Thread ID: 0, 0, 100
Thread ID: 2, 2, 100
Thread ID: 4, 4, 100
Thread ID: 9, 9, 100
Thread ID: 6, 6, 100
100

```
lastprivate：是用来指定将线程中的一个或多个私有变量的值在并行处理结束后复制到主线程中的同名变量中，负责拷贝的线程是for或sections任务分担中的最后一个线程, __环境变量__
```
#include<stdio.h>
#include<omp.h>
int main()
{
    int A=100;
    #pragma omp parallel for firstprivate(A),lastprivate(A)
    for(int i=0;i<10;i++)
    {
	A+=i;    
	printf("Thread ID: %d, %d, %d\n",omp_get_thread_num(),i,A);
    }
    printf("%d\n",A);
    return 0;
}
[usrname@node169 openmp]$ ./lastprivate 
Thread ID: 0, 0, 100
Thread ID: 6, 6, 106
Thread ID: 3, 3, 103
Thread ID: 2, 2, 102
Thread ID: 9, 9, 109
Thread ID: 1, 1, 101
Thread ID: 5, 5, 105
Thread ID: 7, 7, 107
Thread ID: 8, 8, 108
Thread ID: 4, 4, 104
109
```

   reduction：用来指定一个或多个变量是私有的，并且在并行处理结束后这些变量要执行指定的归约运算，并将结果返回给主线程同名变量;

nowait：指出并发线程可以忽略其他制导指令暗含的路障同步,__事件同步__
```
#include<stdio.h>
#include<omp.h>
int main()
{
     int i,j;
     #pragma omp parallel
     {
         #pragma omp for nowait
	 for(i=0;i<6;i++)
         {
	     printf("+\n");
	 }
	 #pragma omp for
	 for(j=0;j<6;j++)
 	 {
	     printf("-    \n");
	 }
      }
      return 0;
}

[username@node169 openmp]$ ./nowait
+
-    
+
-    
+
-    
+
-    
+
-    
+
-

```

   num_threads：指定并行域内的线程的数目；

schedule：子句schedule (type,[,size]),type为static、dynamic、runtime、size可用可不用，调度类型为runtime时size参数未非法  __任务分担类__
```
#pragma omp parallel for schedule(static)    __每个线程分配任务均衡__
for(int i=0;i<10;i++)
    printf("j=%d,ThreadId=%d\n",i,omp_get_thread_num());

[username@node341 openmp]$ ./schedule 
j=1,ThreadId=1
j=0,ThreadId=0
j=6,ThreadId=6
j=3,ThreadId=3
j=4,ThreadId=4
j=7,ThreadId=7
j=9,ThreadId=9
j=5,ThreadId=5
j=8,ThreadId=8
j=2,ThreadId=2


#pragma omp parallel for schedule(static,2) __size为分配给给个线程的size个连续的迭代计算
for(int i=0;i<10;i++)
    printf("j=%d,ThreadId=%d\n",i,omp_get_thread_num());

[username@node341 openmp]$ ./schedule 
j=0,ThreadId=0
j=1,ThreadId=0
j=8,ThreadId=4
j=9,ThreadId=4
j=4,ThreadId=2
j=5,ThreadId=2
j=6,ThreadId=3
j=7,ThreadId=3
j=2,ThreadId=1
j=3,ThreadId=1

#pragma omp parallel for schedule(dynamic) __较快的线程会分到更多的线程__
for(int i=0;i<20;i++)
    printf("j=%d,ThreadId=%d\n",i,omp_get_thread_num());
[user@node341 openmp]$ ./schedule
j=0,ThreadId=0
j=10,ThreadId=0
j=11,ThreadId=0
j=12,ThreadId=0
j=13,ThreadId=0
j=14,ThreadId=0
j=15,ThreadId=0
j=16,ThreadId=0
j=17,ThreadId=0
j=18,ThreadId=0
j=19,ThreadId=0
j=4,ThreadId=7
j=9,ThreadId=8
j=1,ThreadId=9
j=5,ThreadId=6
j=8,ThreadId=5
j=2,ThreadId=1
j=6,ThreadId=3
j=7,ThreadId=4
j=3,ThreadId=2

#pragma omp parallel for schedule(dynamic,2) __分配给每个线程size个连续迭代的次数__
for(int i=0;i<20;i++)
    printf("j=%d,ThreadId=%d\n",i,omp_get_thread_num());
[username@node341 openmp]$ ./schedule
j=0,ThreadId=0
j=1,ThreadId=0
j=16,ThreadId=1
j=17,ThreadId=1
j=2,ThreadId=4
j=3,ThreadId=4
j=18,ThreadId=5
j=19,ThreadId=5
j=10,ThreadId=2
j=11,ThreadId=2
j=6,ThreadId=3
j=7,ThreadId=3
j=8,ThreadId=9
j=9,ThreadId=9
j=4,ThreadId=8
j=5,ThreadId=8
j=14,ThreadId=7
j=15,ThreadId=7
j=12,ThreadId=6
j=13,ThreadId=6

#pragma omp parallel for schedule(guided) __开始时每个线程分配到较大的迭代块，之后会逐渐递减__
for(int i=0;i<20;i++)
    printf("j=%d,ThreadId=%d\n",i,omp_get_thread_num());

[username@node341 openmp]$ ./schedule 
j=0,ThreadId=0
j=1,ThreadId=0
j=2,ThreadId=0
j=3,ThreadId=0
j=4,ThreadId=0
j=12,ThreadId=2
j=13,ThreadId=2
j=16,ThreadId=2
j=5,ThreadId=1
j=6,ThreadId=1
j=7,ThreadId=1
j=8,ThreadId=1
j=14,ThreadId=0
j=15,ThreadId=0
j=19,ThreadId=0
j=17,ThreadId=2
j=9,ThreadId=3
j=10,ThreadId=3
j=11,ThreadId=3
j=18,ThreadId=1

#pragma omp parallel for schedule(guided，2) __最后迭代块大小将为2__
for(int i=0;i<20;i++)
    printf("j=%d,ThreadId=%d\n",i,omp_get_thread_num());

[user@node341 openmp]$ ./schedule  //export OMP_NUM_THREADS=4
j=0,ThreadId=0
j=1,ThreadId=0
j=2,ThreadId=0
j=3,ThreadId=0
j=4,ThreadId=0
j=14,ThreadId=0
j=15,ThreadId=0
j=9,ThreadId=3
j=10,ThreadId=3
j=11,ThreadId=3
j=5,ThreadId=2
j=6,ThreadId=2
j=7,ThreadId=2
j=8,ThreadId=2
j=16,ThreadId=0
j=17,ThreadId=0
j=12,ThreadId=1
j=13,ThreadId=1
j=18,ThreadId=3
j=19,ThreadId=3
```
shared：指定一个或多个变量为多个线程间的共享变量,不加这句话会有数据竞争
```
#include<stdio.h>
#include<omp.h>
int main()
{
    int i=0;
    int sum=0;
    #pragma omp parallel for private(i) shared(sum) 
    for(i=0;i<5;i++)
    {
        sum=sum+i;
    }
    printf("%d\n",i);
    printf("%d\n",sum);
    return 0;
}

[user@node171 openmp]$ ./shared 
0
10
[user@node171 openmp]$ ./shared 
0
3
[user@node171 openmp]$ ./shared 
0
1
```

   ordered：用来指定for任务分担域内指定代码段需要按照串行循环次序执行；

   copyprivate：配合single指令，将指定线程的专有变量广播到并行域内其他线程的同名变量中；
   copyin：用来指定一个threadprivate类型的变量需要用主线程同名变量进行初始化；

default：用来指定并行域内的变量的使用方式，缺省是shared。使用shared传入并行区域同名变量被当做共享变量处理，不会产生线程私有副本，除非使用private指定。使用none除明确定义的外，线程中用到的变量都必须显式指定为共享还是私有。
```
#include<stdio.h>
#include<omp.h>
int main()
{
    int a=50;
    int i;
    int b=0;
    const int N=5;
    float startTime=omp_get_wtime();
    #pragma omp parallel for default(shared) 
    for(i=0;i<N;i++)
    {
        b=a+i;
        printf("Thread id is: %d, i=%d, b=%d\n",omp_get_thread_num(),i,b);
    }
    float endTime=omp_get_wtime();
    printf("a=%d b=%d (expected a=50 b=1049)\n",a,b);
}

[user@node171 openmp]$ ./default
Thread id is: 0, i=0, b=50
Thread id is: 4, i=4, b=54
Thread id is: 2, i=2, b=52
Thread id is: 3, i=3, b=53
Thread id is: 1, i=1, b=51
a=50 b=53 (expected a=50 b=1049)
[user@node171 openmp]$ ./default
Thread id is: 0, i=0, b=50
Thread id is: 4, i=4, b=54
Thread id is: 2, i=2, b=52
Thread id is: 1, i=1, b=51
Thread id is: 3, i=3, b=53
a=50 b=52 (expected a=50 b=1049)



#include<stdio.h>
#include<omp.h>
int main()
{
    int a=50;
    int i;
    int b=0;
    const int N=5;
    float startTime=omp_get_wtime();
    #pragma omp parallel for default(none) private(i) firstprivate(a) lastprivate(b) 
    for(i=0;i<N;i++)
    {
        b=a+i;
	printf("Thread id is: %d, i=%d, b=%d\n",omp_get_thread_num(),i,b);
    }
    float endTime=omp_get_wtime();
    printf("a=%d b=%d (expected a=50 b=1049)\n",a,b);
}

[user@node171 openmp]$ ./default 
Thread id is: 0, i=0, b=50
Thread id is: 4, i=4, b=54
Thread id is: 1, i=1, b=51
Thread id is: 2, i=2, b=52
Thread id is: 3, i=3, b=53
a=50 b=54 (expected a=50 b=1049)
[user@node171 openmp]$ ./default 
Thread id is: 0, i=0, b=50
Thread id is: 1, i=1, b=51
Thread id is: 2, i=2, b=52
Thread id is: 3, i=3, b=53
Thread id is: 4, i=4, b=54
a=50 b=54 (expected a=50 b=1049)
```

## API函数

除上述编译制导指令之外，OpenMP还提供了一组API函数用于控制并发线程的某些行为，下面是一些常用的OpenMP API函数以及说明：

```
   omp_get_thread_num  返回线程的编号
   omp_get_num_threads 返回并行域中线程的总数
   omp_get_max_threads 返回最大线程数
   omp_in_parallel     返回当前是否在并行域中
   omp_get_nested      判断系统是否支持并行嵌套
   omp_set_nested      启用或关闭并行嵌套
   omp_set_dynamic     启用或关闭线程数目的动态改变
   omp_get_wtime       返回某个点上的时间
```

## 环境变量

```
OMP_SCHEDULE、OMP_NUM_THREADS、OMP_DYNAMIC、OMP_NESTED等。

```
