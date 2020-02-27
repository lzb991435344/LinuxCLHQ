#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//40
//test kill -40 + pid
#define MYRTSIG (SIGRTMIN + 6)
//the second line 
static void  mysig_handler (int s){
	write(1, "!", 1);
}

int main(int argc, char* argv[]){

	int i,j;
	sigset_t set,oset,saveset;
	//SIG_IGN 忽略信号
	//signal(SIGINT, SIG_IGN);
	//signal()执行条件
	//1.程序未结束
	//2.信号到达

	//SIGINT maybe is block or unblock
    signal(MYRTSIG, mysig_handler);
    sigemptyset(&set);
    sigaddset(&set, MYRTSIG);

    //import
    sigprocmask(SIG_UNBLOCK, &set, &saveset);
    sigprocmask(SIG_BLOCK, &set, &oset);
	for(j = 0; j < 100; ++j){
		//sigprocmask(SIG_BLOCK, &set, &oset);
		for(i = 0; i < 5; ++i){
			write(1, "*", 1);
	      	sleep(1);
		} 
		write(1, "\n", 1);
		sigsuspend(&oset);
		/**
		sigset_t tmpset;
		sigprocmask(SIG_SETMASK, &oset, &tmpset);
		pause();
		sigprocmask(SIG_SETMASK, &tmpset, NULL);
		*/
		//sigprocmask(SIG_SETMASK, &oset, NULL);  
		//pause(); //not atomic 
	}
	//set the original set
	sigprocmask(SIG_SETMASK, &saveset, NULL);
	exit(0);
}