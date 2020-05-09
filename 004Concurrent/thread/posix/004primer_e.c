#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//筛质数加强版，无须用void*强转
//201个线程都有自己的空间，进行动态分配，创建线程的时候直接
//传指针

#define LEFT 30000000
#define RIGHT 30000200 
//确定资源的上限
#define THRNUM (RIGHT - LEFT + 1)

static void* thr_primer(void*);

//201 thread will hava an address 
struct  thr_arg_st{
    //待计算的数值
	int n;
};

//test ./004_primer_e | wc -l
//ulimit -a/-s  change the stack size to create more threads
int main(int argc, char* argv[]){

	int i,j,mark;
    int err;
    pthread_t tid[THRNUM];
    struct  thr_arg_st* p;
    void* ptr;


    for(i = LEFT; i <= RIGHT; ++i){
    	//err = pthread_create(tid + (i - LEFT), NULL, thr_primer, &i);
    	//the same address will race 
    	p = malloc(sizeof(*p));
    	if(p == NULL){
    		perror("malloc()");
    		exit(1);
    	}

        //将i值赋值给结构体中的n值
    	p->n = i;

        //question:未必一个进程中能创建出这么多的线程，取决于外在的资源量
        //201个线程的代码段是共用的，但是栈是独立的
        //ulimit -a ,查栈的大小
    	err = pthread_create(tid + (i - LEFT), NULL, thr_primer, p);
    	if(err){
    		fprintf(stderr, "pthread_create() is %s\n",strerror(err));
    		exit(1);
    	}
    }

    for(i = LEFT; i <= RIGHT; ++i){
    	//pthread_join(tid[i - LEFT], NULL);
        //返回值是一个void的指针
    	pthread_join(tid[i - LEFT], &ptr);
    	free(ptr);
    }
	exit(0);  
}
static void* thr_primer(void* p){
	int i,j;
	int mark;

	//i = *(int*)p;
    //转指针，i值是唯一的
	i = ((struct thr_arg_st*)p)->n;
    //malloc和free尽量放在同一个函数，或同一模块中
	//free(p);
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
	//pthread_exit(NULL);
    //赋了指针值，记得释放201块结构体的空间大小
    //
	pthread_exit(p);
}