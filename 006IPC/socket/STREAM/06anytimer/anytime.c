#include "anytime.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
 #include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

enum{
	STATE_RUNNING = 1,
	STATE_CANCEL,
	STATE_OVER
};
struct at_job_st{
	
	int job_state;
	int sec;
	int time_remain;//time_remain == sec; 
	int repeat;  //0:no   1:yes(reapeat job)
	at_jobfunc_t* jobp;
	void* arg;
};

static struct at_job_st* job[JOB_MAX];
static int inited = 1;
static struct sigaction alrm_sa_save;



static void alrm_action(int s, siginfo_t* infop, void* unused){
	
	int i;
	if(infop->si_code != SI_KERNEL){
		return;
	}
	for(i = 0; i < JOB_MAX; ++i){
		if(job[i] != NULL && job[i]->job_state == STATE_RUNNING){
			job[i]->time_remain--;
			if(job[i]->time_remain == 0){
				job[i]->jobp(job[i]->arg);
				if(job[i]->repeat == 1){
					job[i]->time_remain = job[i]->sec;
				}else{
					job[i]->job_state = STATE_OVER;
				}
				
			}
		}
	}
}

static void module_unload(){

	struct itimerval itv;

	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &itv, NULL);

	if(sigaction(SIGALRM, &alrm_sa_save, NULL) < 0){
		perror("sigaction()");
		exit(1);
	}
}

static void module_load(void){

	struct sigaction sa;
	struct itimerval itv;
	//149 10
	sa.sa_sigaction = alrm_action;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;

	if(sigaction(SIGALRM, &sa, &alrm_sa_save) < 0){
		perror("sigaction()");
		exit(1);
	}

	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;

	//ignore old time
	if(setitimer(ITIMER_REAL, &itv, NULL) < 0){
		perror("setitimer()");
		exit(1);
	} 
	atexit(module_unload);
}


static int get_free_pos(void){
	int i;
	for(i = 0; i < JOB_MAX; ++i){
		if(job[i] == NULL){
			return i;
		}
	}
	return -1;
}

/**
@return  >=0  success,return job id
         == -EINVAL fail,param is invalid
         == -ENOSPC fail,arr is full
         == -ENOMEM fail,
*/

int at_addjob(int sec, at_jobfunc_t *jobp, void* arg){

	int pos;
	struct at_job_st* me;

	if(sec < 0){
		return -EINVAL;
	}


   	if(!inited){
   		module_load();
   		inited = 1;
   	}

	pos = get_free_pos();
	if(pos < 0){
		return -ENOSPC;
	}

	me = malloc(sizeof(*me));
	if(me == NULL){
		return -ENOMEM;
	}

	me->job_state = STATE_RUNNING;
	me->sec = sec;
	me->time_remain = me->sec;
	me->jobp = jobp;
	me->arg = arg;
	me->repeat = 0;

	job[pos] = me;

	printf("pos is %d\n", pos);
	return pos;
}

int at_addjob_repeat(int sec, at_jobfunc_t* jobp, void* arg){
	int pos;
	struct at_job_st* me;

	if(sec < 0){
		return -EINVAL;
	}


   	if(!inited){
   		module_load();
   		inited = 1;
   	}

	pos = get_free_pos();
	if(pos < 0){
		return -ENOSPC;
	}

	me = malloc(sizeof(*me));
	if(me == NULL){
		return -ENOMEM;
	}

	me->job_state = STATE_RUNNING;
	me->sec = sec;
	me->time_remain = me->sec;
	me->jobp = jobp;
	me->arg = arg;
	me->repeat = 1;

	job[pos] = me;

	return pos;
}



/**
@function check the flag == running
@param job id
@return  == 0 success, the job is canceled
  		 ==-EINVAL fail,invalid param 
  		 ==-EBUSY  	fail,the job is finished
		 ==-ECANCELED  fail,the job is repeat cancel
  //wait()   parent should wait the child die      
*/
int at_cacnceljob(int id){
	if(id < 0 || id > JOB_MAX || job[id] == NULL){
		return -EINVAL;
	}

	if(job[id]->job_state == STATE_CANCEL){
		return -ECANCELED;
	}
	if(job[id]->job_state == STATE_OVER){
		return -EBUSY;
	}
	job[id]->job_state = STATE_CANCEL;

	return 0;
}


/**
@function 
@param job id---->cancel or over
@return == 0 success,the job is free
        == -EINVAL fail,param is invalid
*/
int at_waitjob(int id){
	if(id < 0 || id > JOB_MAX || job[id] == NULL){
		return -EINVAL;
	}

	if(job[id]->repeat == 1){
		return -EBUSY;
	}
	while(job[id]->job_state == STATE_RUNNING){
		pause();
	}
	if(job[id]->job_state == STATE_CANCEL || job[id]->job_state == STATE_OVER){
		free(job[id]);
		job[id] = NULL;
	}

	return 0;
}


int at_pausejob(){

}
int at_resumejob(){

}
