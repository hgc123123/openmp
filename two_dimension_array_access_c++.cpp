#include<iostream>
#include<omp.h>
using namespace std;

float **create_rand_nums(int rows,int columns){
    float **rand_nums=new float*[rows];
    for (int i=0;i<rows;i++){
	rand_nums[i]=new float[columns];
        for(int j=0;j<columns;j++){
	    rand_nums[i][j]=(rand()/(float)RAND_MAX);
	}
    }
    return rand_nums;
}

int main()
{
    float **two_dimensions=NULL;
    float sum=0;
    two_dimensions=create_rand_nums(10000,10000);
    float startTime=omp_get_wtime();
    cout<<startTime<<endl;
    for(int i=0;i<10000;i++){
        for(int j=0;j<10000;j++){
	    sum+=two_dimensions[i][j];
	    //cout<<two_dimensions[i][j]<<" ";
	}
    }
    float endTime=omp_get_wtime();
    cout<<endTime<<endl;
    cout<<"sum is: "<<sum<<endl;
    cout<<"cost time of row-wise access  is: "<<(endTime-startTime)<<endl;
    cout<<endl;
    cout<<endl;

    startTime=omp_get_wtime();
    cout<<startTime<<endl;
    for(int j=0;j<10000;j++){
        for(int i=0;i<10000;i++){
            sum+=two_dimensions[i][j];
	}
    }
    endTime=omp_get_wtime();
    cout<<endTime<<endl;
    cout<<"sum is: "<<sum<<endl;
    cout<<"cost time of column-wise access is: "<<(endTime-startTime)<<endl;

    for(int i=0;i<10;i++){
	delete []two_dimensions[i];
    }
    delete []two_dimensions;
    /*for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cout<<two_dimensions[i][j]<<" ";
        }
    }*/

    return 1;
}
