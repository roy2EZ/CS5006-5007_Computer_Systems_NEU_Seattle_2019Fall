#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N_Large 50000
#define N_Med 5000
#define N_Small 500
#define MAX 100000000

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int start, int end){

    swap(&arr[start],&arr[end]);
    int value = arr[end];
    int index = start;
    for(int i=start;i<end;++i){
        if(arr[i]<value){
            swap(&arr[index++],&arr[i]);
        }
    }
    swap(&arr[index],&arr[end]);
    return index;
}

void fastsort(int arr[],int start,int end){

    if(end>start){
        int index = partition(arr,start,end);
        fastsort(arr, start, index-1);
        fastsort(arr, index+1, end);
    }
}

int arr1[N_Large], arr2[N_Med], arr3[N_Small];

int main(){
    //generate random large array
    srand((unsigned)time(NULL));
    for(int i = 0; i < N_Large; ++i){
        arr1[i] = rand()% MAX;
    }
    //generate random med array
    srand((unsigned)time(NULL));
    for(int i = 0; i < N_Med; ++i){
        arr2[i] = rand()% MAX;
    }
    //generate random small array
    srand((unsigned)time(NULL));
    for(int i = 0; i < N_Small; ++i){
        arr3[i] = rand()% MAX;
    }
    // sort the large array
    clock_t start_time_large=clock();
    {
        fastsort(arr1,0,N_Large-1);
    }
    clock_t end_time_large=clock();

    // sort the med array
    clock_t start_time_med=clock();
    {
        fastsort(arr2,0,N_Med-1);
    }
    clock_t end_time_med=clock();

    // sort the small array
    clock_t start_time_small=clock();
    {
        fastsort(arr3,0,N_Small-1);
    }
    clock_t end_time_small=clock();
    
    //print array size and sorting running time
    printf("---------------------\nFAST SORT: INSERTION\n---------------------\n");
    printf("Large: %d elements",N_Large);
    printf("    Running time: %lf ms \n", (double)(end_time_large-start_time_large)/CLOCKS_PER_SEC*1000);
    printf("Med:   %d  elements",N_Med);
    printf("    Running time: %lf ms \n", (double)(end_time_med-start_time_med)/CLOCKS_PER_SEC*1000);
    printf("Small: %d   elements",N_Small);
    printf("    Running time: %lf ms \n", (double)(end_time_small-start_time_small)/CLOCKS_PER_SEC*1000);
    return 0;
}

