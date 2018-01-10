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
#ifdef __cplusplus
}
#endif

int max(int a, int b);
 
/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
int lcs( char *X, char *Y, int m, int n )
{
	int len1=0, len2=0;
   if (m == 0 || n == 0)
     return 0;
   if (X[m-1] == Y[n-1])
   {
   	 #pragma omp task
     len1 = lcs(X, Y, m-1, n-1);
     
     #pragma omp taskwait
     return len1 + 1;
   }
   else
   {
   		#pragma omp task
		len1 = lcs(X, Y, m, n-1);
		
		
		#pragma omp task
		len2 =  lcs(X, Y, m-1, n);
		
		#pragma omp taskwait
		return max(len1, len2);
		
   }
}

void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}
 
/* Utility function to get max of 2 integers */
int max(int a, int b)
{
    return (a > b)? a : b;
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
  int length = atoi(argv[1]);
  
  int nbthreads = atoi(argv[2]);
  
  omp_set_num_threads(nbthreads);
  char *schedule_type = argv[3];
 
  
  unsigned long granularity = atoi(argv[4]);
  omp_sched_t kind;
  if(strcmp(schedule_type , "static") == 0)
  	kind = (omp_sched_t)1;
  else
  	kind = (omp_sched_t)2;
  
  omp_set_schedule(kind, granularity);
  
 
  
  char *X, *Y;
  
  X = (char*)malloc(sizeof(char)*length);
  Y = (char*)malloc(sizeof(char)*length);
  
  #pragma omp parallel
  {
  	#pragma omp task
  	rand_str(X, length);
  	#pragma omp task
  	rand_str(Y, length);
 }
 
 #pragma omp taskwait
 
  int m = strlen(X);
  int n = strlen(Y);
  
  //std::cout<<X<<std::endl;
 // std::cout<<Y<<std::endl;
  
  int len;
 
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	#pragma omp parallel
	{
		#pragma omp single
		{
			 len = lcs( X, Y, m, n );
		}
	}
 
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout<<len<<std::endl;
  std::cerr<<elapsed_seconds.count()<<std::endl;
 

    
    
 return 0;
}
