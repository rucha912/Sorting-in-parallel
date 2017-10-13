#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif

  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
  
  
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
  
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  int * arr = new int [n];
  int* newarr = new int [n+1];

  generatePrefixSumData (arr, n);
  omp_set_num_threads(nbthreads);
  
  #pragma omp parallel 
  for ( int i = 0; i < n+1; i++)
  {
  	newarr[i] = newarr[i] + arr[i];
  }

	
  for(int i = 0; i<n; i++)
  {
  	std::cout<<arr[i]<<std::endl;
  }
  std::cout<<"newArray"<<std::endl;
  for(int i = 0; i<n; i++)
  {
  	std::cout<<newarr[i]<<std::endl;
  }



  checkPrefixSumResult(newarr, atoi(argv[1]));
  
  delete[] arr;

  return 0;
}
