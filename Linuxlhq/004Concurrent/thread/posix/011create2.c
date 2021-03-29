#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* thr_fun(void* p){

	//循环等待
	while(1){
		pause();
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	int err,i;
	pthread_t tid;
	pthread_attr_t attr;

	//man pthread_attr_init 的see also
	pthread_attr_init(&attr);

	//设置线程栈大小为1M = 1024*1024
	pthread_attr_setstacksize(&attr, 1024*1024);
	for(i = 0; ; ++i){
		err = pthread_create(&tid, &attr, thr_fun, NULL);
		if(err){
			fprintf(stderr, "pthread_create():%s\n", strerror(err));
			break;
		}

	}
	printf("%d\n", i);
	pthread_attr_destroy(&attr);
	exit(0);
}