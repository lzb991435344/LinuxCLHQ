#include <stdio.h>
#include <stdlib.h>

int main(){


//2 cores will create 2 threads
#pragma omp parallel sections
{
	#pragma omp section 
	printf("[%d]Hello\n",omp_get_thread_num());

	#pragma omp section 
	printf("[%d]World\n",omp_get_thread_num());	
}	
	
	exit(0);
}