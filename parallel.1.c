#include <iostream>
#include <omp.h>

using namespace std;

void subdomain(float *x,int istart,int ipoints){
    int i;
    for(i=0;i<ipoints;i++){
        x[i+istart]=123.456;
    }
}

void sub(float *x,int npoints){
    int iam,nt,ipoints,istart;
    #pragma omp parallel default(shared) private(iam,nt,ipoints,istart)
    {
        iam=omp_get_thread_num();
	nt=omp_get_num_threads();
	ipoints=npoints/nt;
	istart=iam*ipoints;
	if(iam == nt-1){
	    ipoints=npoints-istart;
	}
	subdomain(x,istart,ipoints);
    }
}

int main(){
    float array[10000];
    sub(array,10000);
    for(int i=0;i<10000;i++)
    {
    	cout<<array[i]<<" ";
    }
    return 0;
}
