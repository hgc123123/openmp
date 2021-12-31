#include <iostream>
#include <omp.h>
#include <time.h>
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
    clock_t startTime=clock();
    #pragma omp parallel for reduction(+:sum)
    for(int i=1;i<=n;i++)
    {
        sum+=f((i-0.5)/n);
    }
    sum/=n;
    clock_t endTime=clock();
    cout<<"The value of pi is: "<<sum<<", cost time is: "<<(endTime-startTime)<<endl;
    //float endTime=omp_get_wtime();
    sum=0;
    cout<<endl;
    startTime=clock();
    cout<<startTime<<endl;
    for(int i=1;i<=n;i++)
    {
        sum+=f((i-0.5)/n);
    }
    sum/=n;
    endTime=clock();
    cout<<endTime<<endl;
    cout<<"The value of pi is: "<<sum<<", cost time is: "<<(endTime-startTime)<<endl;

    return 0;
}
