#include <stdio.h>
#include <stdlib.h>

/**
 a timer 定时器的实现
*/
static void f1(void* p){
	printf("f1():%s\n", p);
}

static void f2(void* p){
	printf("f2():%s\n", p);
}


//sequence is Begin!End!**bbb***aaa***ccc******
int main(int argc, char* argv[]){
	
	int job1,job2,job3;
	puts("Begin!");
	job1 = at_addjob(5,f1,"aaa");
	if(job < 0){
		fprintf(stderr, "at_addjob():%s\n", -job1);
	}
	at_addjob(2,f2,"bbb");

	at_addjob(7,f1,"ccc");

	puts("End!");

	while(1){
		write(1,"*",1);
		sleep(1);
	}
	exit(0);
}