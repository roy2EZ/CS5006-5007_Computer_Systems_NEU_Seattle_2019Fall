#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N_Large 50000
#define N_Med 5000
#define N_Small 500
#define MAX 100000000

//insertion sort
void slowsort(int arr[], int len){
    int i, j, key;
    for(i = 1; i < len; i++){
        key = arr[i];
        for(j = i-1; j >= 0; --j){
            if(arr[j] > key)
                arr[j+1] = arr[j];
            else
            break;    
        }
        arr[j+1] = key;
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
        slowsort(arr1,N_Large);
    }
    clock_t end_time_large=clock();

    // sort the med array
    clock_t start_time_med=clock();
    {
        slowsort(arr2,N_Med);
    }
    clock_t end_time_med=clock();

    // sort the small array
    clock_t start_time_small=clock();
    {
        slowsort(arr3,N_Small);
    }
    clock_t end_time_small=clock();
    
    //print array size and sorting running time
    printf("---------------------\nSLOW SORT: INSERTION\n---------------------\n");
    printf("Large: %d elements",N_Large);
    printf("    Running time: %lf ms \n", (double)(end_time_large-start_time_large)/CLOCKS_PER_SEC*1000);
    printf("Med:   %d  elements",N_Med);
    printf("    Running time: %lf ms \n", (double)(end_time_med-start_time_med)/CLOCKS_PER_SEC*1000);
    printf("Small: %d   elements",N_Small);
    printf("    Running time: %lf ms \n", (double)(end_time_small-start_time_small)/CLOCKS_PER_SEC*1000);
    return 0;

}
