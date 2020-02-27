#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//用volatile的原因？
static volatile int loop = 1;
static void alarm_handler(int s){
	loop = 0;
}
int main(){

	int64_t count = 0 ;

	//alarm(5);
	//.......
	signal(SIGALRM,alarm_handler);
    alarm(5);
	while(loop){
		count++;
	}
	printf("%lld\n",count);
	exit(0);
}