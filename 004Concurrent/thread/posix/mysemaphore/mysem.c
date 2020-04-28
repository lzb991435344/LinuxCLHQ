
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mysem.h"

struct mysem_st{
	int value;//资源总量
	pthread_mutex_t mut;//互斥量
	pthread_cond_t cond;//条件变量
};

//test:ps ax -L

mysem_t* mysem_init(int initval){
	struct  mysem_st* me;
	//malloc内存空间
	me = malloc(sizeof(*me));
	if(me == NULL){
		return NULL;
	}
	me->value = initval;
	pthread_mutex_init(&me->mut, NULL);
	pthread_cond_init(&me->cond, NULL);

 	return me;
}

int mysem_add(mysem_t* ptr, int n){
	struct mysem_st* me = ptr;

	pthread_mutex_lock(& me->mut);

	me->value += n;//归还数量

	//pthread_cond_signal() or pthread_cond_boardcast;
	//maybe can satisfied several thread
	//满足减去的需求量，满足多人的需求，直接唤醒等待资源量的线程
	//产生惊群，有可能多个线程正在等待该资源
	pthread_cond_broadcast(&me->cond);
	pthread_mutex_unlock(&me->mut);

	return n;
}


int mysem_sub(mysem_t* ptr, int n){
	struct mysem_st* me = ptr;//转换指针

	pthread_mutex_lock(&me->mut);

	//wait for value >=n 等待
	while(me->value < n){
		pthread_cond_wait(&me->cond, &me->mut);
	}
	//
	me->value -= n;
	pthread_mutex_unlock(&me->mut);

	return n;//减去的数量
}

int  mysem_destroy(mysem_t* ptr){
	struct mysem_st* me = ptr;

	pthread_mutex_destroy(&me->mut);
	pthread_cond_destroy(&me->cond);
	free(me);//free 当前指针
}
