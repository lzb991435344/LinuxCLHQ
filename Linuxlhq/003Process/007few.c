#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//few   fork + exec + wait 函数

int main(){

	pid_t pid;

	puts("begin");//行缓冲，放在缓冲区中未输出，新的进程映像已被替换

	//import 调用execl之前先刷新流
	fflush(NULL);//重定向之后需要流刷新

    //终端和重定向之后的输出，未fflush()只会打印时间戳
	/**  
	begin
	1583831185
    */

	pid = fork();

	if(pid < 0){

		perror("fork()");
		exit(1);
	}
	if(pid == 0){
		execl("/bin/date", "date", "+%s", NULL);
		perror("execl()");
		exit(1);
	}
	//父进程等待子进程退出
	wait(NULL);

	puts("end");
	exit(0);
}

/**
begin
1583831185
end
*/