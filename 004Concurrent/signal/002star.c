#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static void  int_handler (int s){
	write(1, "!", 1);
}

int main(int argc, char* argv[]){

	int i;
	//SIG_IGN 忽略信号
	//signal(SIGINT, SIG_IGN);
	//signal()执行条件
	//1.程序未结束
	//2.信号到达
    signal(SIGINT, int_handler);
	for(i = 0 ; i < 10; ++i){
      write(1, "*", 1);
      sleep(1);
	}
	exit(0);
}