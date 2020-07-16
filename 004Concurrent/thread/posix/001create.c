#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


static void* function(void* p){
	puts("Thread is working!");
	//return NULL;
	pthread_exit(NULL);//退出线程
}


int main(int argc, char* argv[]){
	pthread_t tid;
	int err;

	puts("Begin!");
	//产生一个线程
	err = pthread_create(&tid, NULL, function, NULL);
    
    if(err){
    	//打印errno
    	fprintf(stderr, "pthread_create():%s\n", strerror(err));
    	exit(1);
    }
    //等待线程返回
    pthread_join(tid, NULL);//wait for the created thread return 
	puts("End!");
	exit(0);
}

/**
Begin!
Thread is working!
End!


*/