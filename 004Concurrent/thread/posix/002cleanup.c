#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <pthread.h>

static void cleanup_func(void* p){
	puts(p);
}

static void* func(void* p){
	puts("Thread is working!");

	//挂钩子函数
	// push and pop thread cancellation clean-up handlers

	pthread_cleanup_push(cleanup_func, "cleanup1");
	pthread_cleanup_push(cleanup_func, "cleanup2");
	pthread_cleanup_push(cleanup_func, "cleanup3");

	puts("push over!");

	//the value must be 1
	pthread_cleanup_pop(1);//clean3在栈顶
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	//线程退出
	pthread_exit(NULL);
}
int main(int argc, char* argv[]){
	
	pthread_t tid;
	int err;
	puts("Begin!");

	err = pthread_create(&tid, NULL, func, NULL);
	if(err){
		fprintf(stderr, "pthread_create() is %s\n",strerror(err));
		exit(1);
	}

	puts("End!");
	pthread_join(tid, NULL);
	exit(0);
}

/**
Begin!
End!
Thread is working!
push over!
cleanup3



**/