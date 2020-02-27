#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>



#define LEFT 30000000
#define RIGHT 30000200 
#define THRNUM (RIGHT - LEFT + 1)

static void* thr_primer(void*);

//201 thread will hava an address 
struct  thr_arg_st{
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

    	p->n = i;

    	err = pthread_create(tid + (i - LEFT), NULL, thr_primer, p);
    	if(err){
    		fprintf(stderr, "pthread_create() is %s\n",strerror(err));
    		exit(1);
    	}
    }

    for(i = LEFT; i <= RIGHT; ++i){
    	//pthread_join(tid[i - LEFT], NULL);
    	pthread_join(tid[i - LEFT], &ptr);
    	free(ptr);
    }
	exit(0);  
}
static void* thr_primer(void* p){
	int i,j;
	int mark;

	//i = *(int*)p;
	i = ((struct thr_arg_st*)p)->n;
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
	pthread_exit(p);
}