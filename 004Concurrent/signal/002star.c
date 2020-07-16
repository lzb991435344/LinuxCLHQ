#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static void  int_handler (int s){

	//参数1：stdout
	//参数2： void* buf 用char*好操作
	write(1, "!", 1);
}

int main(int argc, char* argv[]){

	int i;
	//SIG_IGN 忽略信号
	// The signals SIGKILL and SIGSTOP cannot be caught or ignored.

	//signal(SIGINT, SIG_IGN);
	//signal()执行条件
	//1.程序未结束
	//2.信号到达
    signal(SIGINT, int_handler);
	for(i = 0 ; i < 10; ++i){
      write(1, "*", 1);//往终端上打印
      sleep(1);
	}
	exit(0);
}

/*打印10个*，遇到中断之后执行函数，打印！
********^C!**

*/