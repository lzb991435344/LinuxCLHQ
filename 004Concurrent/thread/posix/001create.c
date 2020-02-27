#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


static void* function(void* p){
	puts("Thread is working!");
	//return NULL;
	pthread_exit(NULL);
}


int main(int argc, char* argv[]){
	pthread_t tid;
	int err;

	puts("Begin!");
	err = pthread_create(&tid, NULL, function, NULL);
    
    if(err){
    	fprintf(stderr, "pthread_create():%s\n", strerror(err));
    	exit(1);
    }
    pthread_join(tid, NULL);//wait for the created thread return 
	puts("End!");
	exit(0);
}