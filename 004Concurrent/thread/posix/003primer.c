#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//

#define LEFT 30000000
#define RIGHT 30000200 
#define THRNUM (RIGHT - LEFT + 1)

static void* thr_primer(void*);

int main(int argc, char* argv[]){

	int i,j,mark;
    int err;
    pthread_t tid[THRNUM];


    for(i = LEFT; i <= RIGHT; ++i){
    	//err = pthread_create(tid + (i - LEFT), NULL, thr_primer, &i);
    	//the same address will race 
    	err = pthread_create(tid + (i - LEFT), NULL, thr_primer, (void*)i);
    	if(err){
    		fprintf(stderr, "pthread_create() is %s\n",strerror(err));
    		exit(1);
    	}
    }

    for(i = LEFT; i <= RIGHT; ++i){
    	pthread_join(tid[i - LEFT], NULL);
    }
	exit(0);  
}
static void* thr_primer(void* p){
	int i,j;
	int mark = 1;

	//i = *(int*)p;
	i =(int)p;
	mark = 1;

	for(j = 2; i < i/2; ++j){
	    if(i % j == 0){
	    	mark = 0;
	    	break;
	    }
	}
	
	if(mark){
		printf("%d is a primer\n", i);
	}
	pthread_exit(NULL);
}