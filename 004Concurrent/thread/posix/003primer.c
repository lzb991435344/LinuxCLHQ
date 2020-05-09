#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//多线程并发实现筛质数

#define LEFT 30000000
#define RIGHT 30000200 
#define THRNUM (RIGHT - LEFT + 1)

//主调在被调之前，需要声明
static void* thr_primer(void*);

int main(int argc, char* argv[]){

	int i,j,mark;
    int err;
    //保存tid的值，在pthread_join中参数需要传入
    pthread_t tid[THRNUM];


    for(i = LEFT; i <= RIGHT; ++i){
    	//201个线程同时使用一块i的空间地址，刚创建结束的时候，i的值还没被取走
    	//后面的地址进行覆盖，地址传参产生竞争和冲突
    	//改成(void*)i,直接传变量地址，转成void*
    	//err = pthread_create(tid + (i - LEFT), NULL, thr_primer, &i);
    	//the same address will race 
    	err = pthread_create(tid + (i - LEFT), NULL, thr_primer, (void*)i);
    	if(err){
    		fprintf(stderr, "pthread_create() is %s\n",strerror(err));
    		exit(1);
    	}
    }

    //收尸
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