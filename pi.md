# pragma omp parallel for reduction(+:sum)

---
pragma omp parallel for 后面加上了 reduction(+:sum)，它的意思是告诉编译器：下面的for循环你要分成多个线程跑，但每个线程都要保存变量sum的拷贝，循环结束后，所有线程把自己的sum累加起来作为最后的输出。reduction虽然很方便，但它只支持一些基本操作，比如+,-,||,&,&&等。有些情况下，我们既要避免race condition，但涉及到的操作又超出了reduction的能力范围，应该怎么办呢？这就要用到openMP的另一个工具，critical。

```
#include <iostream>
#include <omp.h>

using namespace std;


int main()
{
    int n,sum;
    cin>>n;
    sum=0;
    float startTime=omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<=n;i++)
    {
        sum+=i;
    }
    float endTime=omp_get_wtime();
    cout<<"The total sum of n! is: "<<sum<<", cost time is: "<<(endTime-startTime)<<endl;
    //float endTime=omp_get_wtime();
    sum=0;
    startTime=omp_get_wtime();
    for(int i=0;i<=n;i++)
    {
        sum+=i;
    }
     endTime=omp_get_wtime();
    cout<<"The total sum of n! is: "<<sum<<", cost time is: "<<(endTime-startTime)<<endl;

    return 0;
}

[hpchgc@node838 ~]$ ./pi 
100000000
The total sum of n! is: 987459712, cost time is: 0.0625

The total sum of n! is: 987459712, cost time is: 0.1875
```

ijij
