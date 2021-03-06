#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//search busy

//num > 0  task 
//num = 0  no task
//num = -1 exit

#define LEFT 30000000
#define RIGHT 30000200 
#define THRNUM 4
static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

static void* thr_primer(void*);

int main(int argc, char* argv[]){

	int i,j,mark;
    int err;
    pthread_t tid[THRNUM];


    for(i = 0; i < THRNUM; ++i){
    	//err = pthread_create(tid + (i - LEFT), NULL, thr_primer, &i);
    	//the same address will race 
    	err = pthread_create(tid + i , NULL, thr_primer, (void*)i);
    	if(err){
    		fprintf(stderr, "pthread_create() is %s\n",strerror(err));
    		exit(1);
    	}
    }


    
    for(i = LEFT; i <= RIGHT; ++i){
    	pthread_mutex_lock(&mut_num);
    	while(num != 0){
    		pthread_mutex_unlock(&mut_num);
    		//sleep(1);
    		//get the same mutex
    		sched_yield();//giveup the sched
    		pthread_mutex_lock(&mut_num);
    	}
    	num = i;
    	pthread_mutex_unlock(&mut_num);
    }




    //the last task must be consumed
    //number is changed, lock anf unlock 
    pthread_mutex_lock(&mut_num);

    //当num不为0的时候，让出调度器
    while(num != 0){
    	pthread_mutex_lock(&mut_num);
    	sched_yield();
    	pthread_mutex_unlock(&mut_num);
    }
    num = -1;
    pthread_mutex_unlock(&mut_num);



    //等待线程结束
    for(i = 0; i < THRNUM; ++i){
    	pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mut_num);
	exit(0);  
}
static void* thr_primer(void* p){
	int i,j;
	int mark;
    
    while(1)
    {


		pthread_mutex_lock(&mut_num);
		while(num == 0 ){
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
		}

		
		if(num == -1){
			//unlock,//avoid the deadlock
			pthread_mutex_unlock(&mut_num);
			break;
		}
		//i = *(int*)p;
		i = num;
		num = 0;

		// linjiequ break,continue,goto ,jmp
		//first unlock,then jmp 
		pthread_mutex_unlock(&mut_num);



		mark = 1;

		for(j = 2; i < i/2; ++j){
		    if(i % j == 0){
		    	mark = 0;
		    	break;
		    }
		}
		
		if(mark){
			printf("[%d]%d is a primer\n", (int)p,i);
		}
	}
	pthread_exit(NULL);
}