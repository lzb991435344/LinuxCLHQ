#include <stdio.h>
#include <stdlib.h>

int main(){


//2 cores will create 2 threads
#pragma omp parallel
{
	puts("hello");
	puts("world");	
}	
	
	exit(0);
}