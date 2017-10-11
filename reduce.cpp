#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);
  
#ifdef __cplusplus
}
#endif


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
  
  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }
  
 

  int * arr = new int [atoi(argv[1])];

  generateReduceData (arr, atoi(argv[1]));
  
  int nbthreads = atoi(argv[2]);
  omp_set_num_threads(nbthreads); 
  
  int min_val;
  int n = atoi(argv[1]);
  
  char *schedule_type = argv[3];
 
  
  unsigned long granularity = atoi(argv[4]);
  
  min_val = arr[0];
 // arr[0] = 0;
  auto clock_start = std::chrono::system_clock::now();
  if(strcmp(schedule_type , "static") == 0)
  {
  
  	#pragma omp parallel for reduction (min : min_val) schedule(static)
  	for (int i = 1 ; i < n; i++)
  	{
  		if(arr[i] < min_val)
  		{
  			min_val = arr[i];
  		}
  	}
  }
  else if(strcmp(schedule_type, "dynamic") == 0)
  {
  	#pragma omp parallel for reduction (min : min_val) schedule(dynamic, granularity)
  	for(int i = 0; i < n; i++)
  	{
  		if(arr[i] < min_val)
  		{
  			min_val = arr[i];
  		}
  	}
  }
  
  auto clock_end = std::chrono::system_clock::now();
  std::chrono::duration<double>diff = clock_end - clock_start;
  std::cout<<"min val"<<min_val<<std::endl;
  /*for(int i = 0; i < n; i++)
  {
  	std::cout<<arr[i]<<std::endl;
  }*/
  std::cerr<<diff.count();
  
  delete[] arr;

  return 0;
}
