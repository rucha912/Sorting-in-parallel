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

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

  
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

  int * arr = new int [atoi(argv[1])];

  generateMergeSortData (arr, atoi(argv[1]));
  
  int nbthreads = atoi(argv[2]);
  int n = atoi(argv[1]);
  
  int current, left;
  
  omp_set_num_threads(nbthreads);
  
  
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  for( current = 1; current <= n-1; current = current * 2)
  {
  	#pragma omp parallel for
  	for(left = 0; left < n-1; left += 2*current)
  	{
  		int mid = std::min(left + current -1, n-1);
  		int right = std::min(left + current*2 - 1, n-1);
		int temp_left, temp_right, temp_mid;
  		temp_left = left;
  		temp_mid = mid;
  		temp_right = right;
  		
  		int nl = temp_mid - temp_left + 1;
  		int nr = temp_right - temp_mid;
  		
		int *left_arr, *right_arr;
  		left_arr = (int *)malloc(sizeof(int)*nl);
  		right_arr = (int *)malloc(sizeof(int)*nr); 

		int i, j, k;
  		
  		#pragma omp parallel for
  		for( i = 0; i < nl; i++)
  		{
  			left_arr[i] = arr[temp_left + i];
		}
		#pragma omp parallel for
		for( i = 0; i < nl; i++)
  		{
  			right_arr[i] = arr[1 + temp_mid + i];
		}
		i = 0;
		j = 0;
		k = temp_left;
		
		while(i < nl && j < nr)
		{
			if(left_arr[i] < right_arr[j])
			{
				arr[k] = left_arr[i];
				i++;
			}
			else
			{
				arr[k] = right_arr[j];
				j++;
			}
			k++;
		}
		while(i < nl)
		{
			arr[k] = left_arr[i];
			i++;
			k++;
		}
		while(j < nr)
		{
			arr[k] = right_arr[j];
			j++;
			k++;
		}
  	}
  }
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  checkMergeSortResult (arr, atoi(argv[1]));
  
  delete[] arr;
  

  return 0;
}
