#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
static void  int_handler (int s){
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
	//ctrl + c发出一次中断信号
    signal(SIGINT, int_handler);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    //import
    //检查或修改与指定信号相关联的处理动作
    sigprocmask(SIG_UNBLOCK, &set, &saveset);

	for(j = 0; j < 100; ++j){
		//设置为block信号
		sigprocmask(SIG_BLOCK, &set, &oset);
		for(i = 0; i < 5; ++i){
			write(1, "*", 1);
	      	sleep(1);
		} 
		write(1, "\n", 1);
		//恢复之前的信号
		sigprocmask(SIG_SETMASK, &oset, NULL);  
	}
	//set the original set
	//模块出去之前恢复为原来进模块之前的状态
	sigprocmask(SIG_SETMASK, &saveset, NULL);
	exit(0);
}