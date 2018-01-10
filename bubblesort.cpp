#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <chrono>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

  
#ifdef __cplusplus
}
#endif

int changes = 1; 

void bubbleSort(int arr[], int start, int n)
{
    // Base case
    if (n == 1)
        return;
 	int temp;
    // One pass of bubble sort. After
    // this pass, the largest element
    // is moved (or bubbled) to end.
    for (int i=start; i<n; i+=2)
        if (arr[i] > arr[i+1])
        {
        	temp = arr[i];
        	arr[i] = arr[i+1];
        	arr[i+1] = temp;
        	++changes;
        }
 
    // Largest element is fixed,
    // recur for remaining array
    
   		
}


int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];

  generateMergeSortData (arr, atoi(argv[1]));
  
  int nbthreads = atoi(argv[2]);
  int n = atoi(argv[1]);
  int *temp;
  temp = (int*)malloc(sizeof(int)*n);
  omp_set_num_threads(nbthreads);
  char *schedule_type = argv[3];
 
  
  unsigned long granularity = atoi(argv[4]);
  omp_sched_t kind;
  if(strcmp(schedule_type , "static") == 0)
  	kind = (omp_sched_t)1;
  else
  	kind = (omp_sched_t)2;
  
  omp_set_schedule(kind, granularity);
  
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
 /* while(changes)
  {
  	changes = 0;
  #pragma omp parallel
  {
 		#pragma omp task	
  		bubbleSort(arr, 0, n);
   		//std::cout<<"changes"<<changes<<std::endl;
   		#pragma omp task
   		bubbleSort(arr, 1, n);
   		//std::cout<<"changes"<<changes<<std::endl;
   		#pragma omp taskwait
  }
  }*/
  
  for(int i = 0; i < n; i++)
  {
  	int first = i%2;
  	#pragma omp parallel for shared(arr, first)
  	for(int j = first; j < n-1; j+=2)
  	{
  		if(arr[j] > arr[j+1])
  			std::swap(arr[j],arr[j+1]);
  	}
  }
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;
 
  
  checkMergeSortResult (arr, atoi(argv[1]));
  
  delete[] arr;
  

  return 0;
}
