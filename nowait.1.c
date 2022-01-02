#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<omp.h>
void nowait_example(int n,int m,float *a,float *b,float *y,float *z)
{
    int i;
    #pragma omp parallel
    {
        #pragma omp for nowait
	for(i=1;i<n;i++)
	    b[i]=(a[i]+a[i-1])/2.0;
 	#pragma omp for nowait
	for(i=0;i<m;i++)
	    y[i]=sqrt(z[i]);
    }
}

int main()
{
    float *a=NULL;
    float *b=NULL;
    float *y=NULL;
    float *z=NULL;
    int n,m;

    printf("input the number of n:\n");
    scanf("%d",&n);
    printf("input the number of m:\n");
    scanf("%d",&m);
    a=(float *)malloc(n*sizeof(float));
    b=(float *)malloc(n*sizeof(float));
    y=(float *)malloc(m*sizeof(float)); 
    z=(float *)malloc(m*sizeof(float));
    for(int i=0;i<n;i++)
    {
	a[i]=(rand()/(float)RAND_MAX);
	printf("%f ",a[i]);
    }
    printf("\n");
    for(int i=0;i<m;i++)
    {
	z[i]=(rand()/(float)RAND_MAX);
	printf("%f ",z[i]);
    }
    printf("\n");
    nowait_example(n,m,a,b,y,z);
    for(int i=1;i<n;i++)
	printf("%f ",b[i]);
    printf("\n");
    for(int i=0;i<m;i++)
	printf("%f ",y[i]);
    printf("\n");
    free(a);
    a=NULL;
    free(b);
    b=NULL;
    free(y);
    y=NULL;
    free(z);
    z=NULL;
    printf("\n");
    printf("\n");
    //for(int i=1;i<n;i++)
    //    printf("%f ",b[i]);    
    return 1;
}

