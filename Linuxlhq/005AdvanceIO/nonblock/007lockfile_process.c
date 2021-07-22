#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define PROCNUM 20
//echo 1 > /tmp/out ; cat /tmp/out
#define FNAME "/tmp/out"
#define LINESIZE 1024


static void  func_add(void){
	FILE* fp;
	char linebuf[LINESIZE];
	int fd;


	fp = fopen(FNAME, "r+");	
	if(fp == NULL){
		perror("fopen()");
		exit(1);
	}
    
    //get the fd from filestream
	//标准io与系统IO之间的转换函数
    fd = fileno(fp); //if error
	if(fd < 0){
		perror("fileno()");
		exit(1);
	}

    lockf(fd, F_LOCK, 0);
	fgets(linebuf, LINESIZE, fp);
	fseek(fp, 0, SEEK_SET);
	//sleep() may be implemented using SIGALRM
	//使用sleep() 可能会产生未定义行为
	sleep(1);
	//格式化写入文件,写文件，全缓冲，需要fflush
	fprintf(fp, "%d\n", atoi(linebuf) + 1);//full buff,line buffer
	//ps:fflush() 仅仅刷新用户空间的由 C 库提供的缓冲。
	//要保证数据被物理地存储到磁盘上，必须也刷新内核缓冲(sync & fsync) 
	//sync --commit buffer cache to disk
	fflush(fp);
	lockf(fd, F_ULOCK, 0);
	fclose(fp);
	

}
int main(int argc, char* argv[]){

	int i;
	pid_t pid;

	for(i = 0 ; i < PROCNUM; ++i){
		
		pid = fork();
		if(pid < 0){
			perror("fork()");
			exit(1);
		}
		if(pid == 0){
			func_add();
			exit(0);
		}
	}

	for(i = 0 ; i < PROCNUM; ++i){
		/*wait的相关行为 等待子进程相关状态改变
		(1)子进程退出
		(2)被信号终止 stop
		(3)被信号唤醒 resume
		等价于   waitpid(-1, &status, 0);
		*/
		wait(NULL);//状态改变立即返回，否则忙等或者被信号终止
	}

	exit(0);
}