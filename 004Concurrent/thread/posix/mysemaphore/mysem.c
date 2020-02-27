
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mysem.h"

struct mysem_st{
	int value;
	pthread_mutex_t mut;
	pthread_cond_t cond;
};

//test:ps ax -L

mysem_t* mysem_init(int initval){
	struct  mysem_st* me;
	me = malloc(sizeof(*me));
	if(me == NULL){
		return NULL;
	}
	me->value = initval;

 	return me;
}

int mysem_add(mysem_t* ptr, int n){
	struct mysem_st* me = ptr;
	pthread_mutex_lock(& me->mut);

	me->value += n;

	//pthread_cond_signal() or pthread_cond_boardcast;
	//maybe can satisfied several thread
	pthread_cond_broadcast(&me->cond);
	pthread_mutex_unlock(&me->mut);

	return n;
}


int mysem_sub(mysem_t* ptr, int n){
	struct mysem_st* me = ptr;
	pthread_mutex_lock(&me->mut);
	//wait for value >=n 
	while(me->value < n){
		pthread_cond_wait(&me->cond, &me->mut);
	}
	//
	me->value -= n;
	pthread_mutex_unlock(&me->mut);

	return n;
}

int  mysem_destroy(mysem_t* ptr){
	struct mysem_st* me = ptr;

	pthread_mutex_destroy(&me->mut);
	pthread_cond_destroy(&me->cond);
	free(me);
}
