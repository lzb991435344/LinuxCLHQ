#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define THRNUM 4

//print abcd to stdout
static int num = 0;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


static int next(int n){
	if(n + 1 == THRNUM){
		return 0;
	}
	return n + 1;
}

static void* thr_fun(void* p){
	//n == thread id
	int n = (int)p;//0 1 2 3
	int c = 'a' + n;
	//lock a,unlock b
	while(1){
		//pthread_mutex_lock(mut + n);
		pthread_mutex_lock(&mut);

		while(num != n){
			//等待 mum = n
			pthread_cond_wait(&cond, &mut);
		}
		write(1, &c, 1);
		
		//取下一个
		num = next(num);
		//a在打印， bcd在等待
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mut);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	pthread_t tid[THRNUM];
	int err,i;

	for(i = 0; i < THRNUM; ++i){
		//pthread_mutex_init(mut + i, NULL);
		//pthread_mutex_lock(mut + i);

		err = pthread_create(tid + i, NULL, thr_fun, (void*)i);
		if(err){
			fprintf(stderr, "pthread_create():%s\n", strerror(err));
			exit(1);
		}
	}
	//pthread_mutex_unlock(mut + 0);
	alarm(5);// 非正常结束
	//正常结束：添加一个处理函数，注册行为，join and destroy


	for(i = 0; i < THRNUM; ++i){
		pthread_join(tid[i], NULL);
	}
	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cond);

	exit(0);
}