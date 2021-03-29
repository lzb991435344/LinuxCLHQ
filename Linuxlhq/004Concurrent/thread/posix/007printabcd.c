#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define THRNUM 4  


//4个线程持续输出 abcd 

//print abcd to stdout

//使用4个互斥量进行操作，4把锁，4个线程
static pthread_mutex_t mut[THRNUM];

static int next(int n){
	if(n + 1 == THRNUM){
		return 0;
	}
	return n + 1;
}

static void* thr_fun(void* p){
	int n = (int)p;//p = 0 1 2 3
	int c = 'a' + n;

	//构造锁链，互斥量锁住的是一段代码，而不是一个变量
	//lock a,unlock b
	while(1){
		//锁住自己，往终端打印
		pthread_mutex_lock(mut + n);
		write(1, &c, 1);

		//解下一个人的锁，最后解自己的锁
		pthread_mutex_unlock(mut + next(n));
	}

	pthread_exit(NULL);
}

int main(){
	pthread_t tid[THRNUM];//保存tid的数组
	int err,i;

	for(i = 0; i < THRNUM; ++i){
		//动态初始化mut数组
		pthread_mutex_init(mut + i, NULL);

		//锁住互斥量
		pthread_mutex_lock(mut + i);
		err = pthread_create(tid + i, NULL, thr_fun, (void*)i);
		if(err){
			fprintf(stderr, "pthread_create():%s\n", strerror(err));
			exit(1);
		}
	}

	//解锁，打印‘a’的锁解锁
	pthread_mutex_unlock(mut + 0);


	alarm(5);//5秒钟之后被信号杀死

	//回收资源
	for(i = 0; i < THRNUM; ++i){
		pthread_join(tid[i], NULL);
	}

	exit(0);
}