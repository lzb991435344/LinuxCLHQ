
//将slowcat2.c重构成线程库
#include "mytbf.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>


//test bash; while true;do kill -ALRM 3654(pid);done
typedef void (*sighandler_t)(int);

//signal(),alarm()只能被调用一次
//全0
static struct mytbf_st* job[MYTBF_MAX];
static int inited = 0;

//static sighandler_t alarm_handler_save;
static struct sigaction alarm_save;

struct mytbf_st{
	int cps;
	int burst;
	int token;
	int pos;
};

static void alarm_action(int s, siginfo_t* infop, void* unuse){
	int i;

	//alarm(1);//function
	//signal from kernel or user or from others 
	if(infop->si_code != SI_KERNEL){
		return ;
	}

	for(i = 0; i < MYTBF_MAX; ++i){
		if(job[i] != NULL){
			job[i]->token += job[i]->cps;
			if(job[i]->token > job[i]->burst){
				job[i]->token = job[i]->burst;
			}
		}
	}
}

//异常终止将令牌桶释放
//调用时机：在异常处理机制中调用
static void module_unload(void){
	//使用signal()的返回值，不能使用默认值
	//使用全局变量
	//signal(SIGALRM, alarm_handler_save);
	//alarm(0);
	struct itimerval itv;
    sigaction(SIGALRM, &alarm_save, NULL);

    itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &itv, NULL);

	int i;
	for(i = 0; i < MYTBF_MAX; ++i){
		free(job[i]);//释放一个void*不会出错
	}
}

static void module_load(void){
	//alarm_handler_save = signal(SIGALRM, alarm_handler);
	//alarm(1);
	struct sigaction sa;
	struct itimerval itv;

	sa.sa_sigaction = alarm_action;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;

	sigaction(SIGALRM, &sa, &alarm_save);
	/**if error*/

	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &itv, NULL);
	/**if error*/
	//钩子函数调用，出错的时候调用
	atexit(module_unload);
}

static int min(int a, int b){
	return (a) < (b) ? (a) : (b);
}
static int get_free_pos(void){
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
   if(!inited){
	module_load();
	inited = 1;
   }
   

   pos = get_free_pos();
   if(pos < 0){
   	return NULL;
   }

   me = (struct mytbf_st*)malloc(sizeof(struct mytbf_st*));
   if(me == NULL){
   	return NULL;
   }
   me->token = 0;
   me->cps = cps;
   me->burst = burst;
   me-> pos = pos;

   job[pos] = me;

   return me;
}
int mytbf_fetchtoken(mytbf_t* ptr, int size){
	struct mytbf_st* me = ptr;
	int n;

	if(size <= 0){
		return -EINVAL;
	}

	while(me->token <= 0){
		pause();
	}
	n = min(me->token,size);
	me->token -= n;
	return n;
}
int mytbf_returntoken(mytbf_t* ptr, int size){
	struct mytbf_st* me = ptr;

	if(size <= 0){
		return -EINVAL;
	}
	me->token +=size;
	if(me->token > me->burst){
		me->token = me->burst;
	}
	return size;

}
int mytbf_destory(mytbf_t* ptr){
   struct mytbf_st* me = ptr;

   job[me->pos] = NULL;
   free(ptr);
   return 0;
}