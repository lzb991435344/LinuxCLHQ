#include <stdio.h>
#include <stdlib.h>
#include "anytime.h"
#include <unistd.h>
/**
 a timer 
*/
static void f1(void* p){
	printf("f1():%s\n", (char*)p);
}

static void f2(void* p){
	printf("f2():%s\n", (char*)p);
}


//sequence is Begin!End!**bbb***aaa***ccc******
int main(int argc, char* argv[]){
	
	int job1,job2,job3;
	puts("Begin!");
	job1 = at_addjob(2,f1,"aaa");
	//job1 = at_addjob_repeat(5,f1,"aaa");
	//job1 = at_addjob_repeat(2, f1, "aaa");
	if(job1 < 0){
		fprintf(stderr, "at_addjob():%d\n", -job1);
		exit(1);
	}

#if 0	
	at_addjob(2,f2,"bbb");
	at_addjob(7,f1,"ccc");

	puts("End!");

#endif

	while(1){
		write(1,"*",1);
		sleep(1);
	}
	exit(0);
}