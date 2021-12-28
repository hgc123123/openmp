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
    cout<<"The total sum of n! is: "<<sum<<"cost time is: "<<(endTime-startTime)<<endl;
    //float endTime=omp_get_wtime();
    sum=0;
    startTime=omp_get_wtime();
    for(int i=0;i<=n;i++)
    {
        sum+=i;
    }
     endTime=omp_get_wtime();
    cout<<"The total sum of n! is: "<<sum<<"cost time is: "<<(endTime-startTime)<<endl;

    return 0;
}
```
