#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

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
  
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }
  
  float result, x_val=0, x_int;
    clock_t start, end;
    float a, b;
    unsigned long n;
    double cpu_time;
    int func, intensity, nbthreads, granularity;
    char *schedule_type;
    func = atoi(argv[1]);
    a = atof(argv[2]);
    b = atof(argv[3]);
    n = atof(argv[4]);
    intensity = atoi(argv[5]);
    nbthreads = atoi(argv[6]);
    granularity = atoi(argv[8]);
    schedule_type = argv[7];
    omp_lock_t writelock;
	    
    omp_set_num_threads(nbthreads); 
    omp_init_lock(&writelock);	
	
    start = clock();
    
    if( strcmp (schedule_type, "static") == 0)
    {
    	#pragma omp parallel for schedule(static)
    	for(int i = 0; i <= n-1; i++)
    	{
			omp_set_lock(&writelock);
			x_int = (a + (i + 0.5) * ((b - a) / (float)n));
			x_val = x_val + x_int;
			switch(func)
        		{
      			case 1: result = f1(x_val, intensity) * ((b - a)/n);
						break;
        		case 2: result = f2(x_val, intensity) * ((b - a)/n);
						break;
          		case 3: result = f3(x_val, intensity) * ((b - a)/n);
						break;
      	  		case 4: result = f4(x_val, intensity) * ((b - a)/n);
						break;
          		default: std::cout<<"\nWrong function id"<<std::endl;
      			}
			omp_unset_lock(&writelock);	
      	}
     }
     else if(strcmp(schedule_type, "dynamic") == 0)
     {
     	
    	#pragma omp parallel for schedule(dynamic, granularity)
    	for(int i = 0; i <= n-1; i++)
    	{
			omp_set_lock(&writelock);
			x_int = (a + (i + 0.5) * ((b - a) / (float)n));
			x_val = x_val + x_int;
			switch(func)
        	{
      			case 1: result = f1(x_val, intensity) * ((b - a)/n);
						break;
        		case 2: result = f2(x_val, intensity) * ((b - a)/n);
						break;
          		case 3: result = f3(x_val, intensity) * ((b - a)/n);
						break;
      	  		case 4: result = f4(x_val, intensity) * ((b - a)/n);
						break;
          		default: std::cout<<"\nWrong function id"<<std::endl;
      		}
	omp_unset_lock(&writelock);
      	}
     }
	
    end = clock();
    omp_destroy_lock(&writelock);
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    std::cout<<result<<std::endl;
    std::cerr<<cpu_time<<std::endl;
  
  return 0;
  
  

  return 0;
}
