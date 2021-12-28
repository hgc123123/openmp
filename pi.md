# The value of pi

```
#include <iostream>
#include <omp.h>

using namespace std;
double f(double x)
{
    return 4.0/(1+x*x);
}

int main()
{
    int n;
    double sum;
    cout<<"输入N值，N值越大，代表精度越高"<<endl;
    cin>>n;
    sum=0;
    float startTime=omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for(int i=1;i<=n;i++)
    {
        sum+=f((i-0.5)/n);
    }
    sum/=n;
    float endTime=omp_get_wtime();
    cout<<"The value of pi is: "<<sum<<", cost time is: "<<(endTime-startTime)<<endl;
    //float endTime=omp_get_wtime();
    sum=0;
    cout<<endl;
    startTime=omp_get_wtime();
    for(int i=1;i<=n;i++)
    {
        sum+=f((i-0.5)/n);
    }
    sum/=n;
    endTime=omp_get_wtime();
    cout<<"The value of pi is: "<<sum<<", cost time is: "<<(endTime-startTime)<<endl;

    return 0;
}

[hpchgc@node838 ~]$ ./pi
输入N值，N值越大，代表精度越高
1000000000
The value of pi is: 3.14159, cost time is: 0.4375

The value of pi is: 3.14159, cost time is: 16.3125

```
