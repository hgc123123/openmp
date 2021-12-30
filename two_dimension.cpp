#include<iostream>
#include<omp.h>
using namespace std;

float **create_rand_nums(int rows,int columns){
    float **rand_nums=(float **)malloc(sizeof(float *)*rows);
    for (int i=0;i<rows;i++){
	rand_nums[i]=(float *)malloc(columns*sizeof(float));
        for(int j=0;j<columns;j++){
	    rand_nums[i][j]=(rand()/(float)RAND_MAX);
	}
    }
    return rand_nums;
}

int main()
{
    float **two_dimensions=NULL;
    two_dimensions=create_rand_nums(4,4);
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
	    cout<<two_dimensions[i][j]<<" ";
	}
	cout<<endl;
    }
    return 1;
    free(two_dimensions);
}
