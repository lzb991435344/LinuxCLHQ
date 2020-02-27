//
//将slowcat2.c重构成线程库
#include "mytbf.h"
#include <stdio.h>
#include <stdlib.h>
//#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

typedef void (*sighandler_t)(int);

//signal(),alarm()只能被调用一次
//全0
static struct mytbf_st* job[MYTBF_MAX];
//PTHREAD_MUTEX_INITIALIZER
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
//static int inited = 0;

static sighandler_t alarm_handler_save;
static pthread_t tid_alrm;

static pthread_once_t init_once = PTHREAD_ONCE_INIT;


struct mytbf_st{
	int cps;
	int burst;
	int token;
	int pos;
	pthread_mutex_t mut;
};

static void* thr_alarm(void* p){
	int i;

	//alarm(1);//function
	while(1){
		pthread_mutex_lock(&mut_job);
		for(i = 0; i < MYTBF_MAX; ++i){
			if(job[i] != NULL){
				pthread_mutex_lock(&job[i]->mut);
				job[i]->token += job[i]->cps;
				if(job[i]->token > job[i]->burst){
					job[i]->token = job[i]->burst;
				}
				pthread_mutex_unlock(&job[i]->mut);
			}
		}
		pthread_mutex_unlock(&mut_job);
		sleep(1);
	}
}

//异常终止将令牌桶释放
//调用时机：在异常处理机制中调用
//
static void module_unload(void){
	//使用signal()的返回值，不能使用默认值
	//使用全局变量
	//signal(SIGALRM, alarm_handler_save);
	//alarm(0);
	int i;

	pthread_cancel(tid_alrm);
	pthread_join(tid_alrm, NULL);
	for(i = 0; i < MYTBF_MAX; ++i){
		if(job[i] != NULL){
			mytbf_destory(job[i]);
		}
		//free(job[i]);//释放一个void*不会出错
	}
	pthread_mutex_destroy(&mut_job);
}

static void module_load(void){
	//alarm_handler_save = signal(SIGALRM, alarm_handler);
	//alarm(1);
	
	int err;

	err = pthread_create(&tid_alrm, NULL, thr_alarm, NULL);
	if(err){
		fprintf(stderr, "pthread_create():%s\n", strerror(err));
		exit(1);
	}
	//钩子函数调用，出错的时候调用
	atexit(module_unload);
}

static int min(int a, int b){
	return (a) < (b) ? (a) : (b);
}

static int get_free_pos_unlock(void){
	int i;

	for(i = 0; i < MYTBF_MAX; ++i){
		if(job[i] == NULL){
			return i;
		}
	}
	return -1;
}
struct mytbf_st* mytbf_init(int cps, int burst){
   struct mytbf_st* me;
   int pos;

   //模块只加载一次，设置一个标志位
  /** if(!inited){
	module_load();
	inited = 1;
   }*/
   pthread_once(&init_once, module_load);
   

   me = (struct mytbf_st*)malloc(sizeof(struct mytbf_st*));
   if(me == NULL){
   	return NULL;
   }
   me->token = 0;
   me->cps = cps;
   me->burst = burst;
   pthread_mutex_init(&me->mut, NULL);


   pthread_mutex_lock(&mut_job);

   //mutex function 
   pos = get_free_pos_unlock();
   if(pos < 0){
   	pthread_mutex_unlock(&mut_job);
   	free(me);
   	return NULL;
   }
   me-> pos = pos;

   job[pos] = me;

   pthread_mutex_unlock(&mut_job);

   return me;
}

//cpu busy
int mytbf_fetchtoken(mytbf_t* ptr, int size){
	struct mytbf_st* me = ptr;
	int n;

	if(size <= 0){
		return -EINVAL;
	}

	pthread_mutex_lock(&me->mut);
	//search, cpu busy 
	while(me->token <= 0){
		pthread_mutex_unlock(&me->mut);
		//pause();
		sched_yield();
		pthread_mutex_lock(&me->mut);
	}
	n = min(me->token,size);
	me->token -= n;
	pthread_mutex_unlock(&me->mut);

	return n;
}
int mytbf_returntoken(mytbf_t* ptr, int size){
	struct mytbf_st* me = ptr;

	if(size <= 0){
		return -EINVAL;
	}

	pthread_mutex_lock(&me->mut);
	me->token +=size;
	if(me->token > me->burst){
		me->token = me->burst;
	}
	pthread_mutex_unlock(&me->mut);
	return size;

}
int mytbf_destory(mytbf_t* ptr){
   struct mytbf_st* me = ptr;

   pthread_mutex_lock(&mut_job);
   job[me->pos] = NULL;
   pthread_mutex_unlock(&mut_job);

   pthread_mutex_destroy(&me->mut);
   free(ptr);
   return 0;
}