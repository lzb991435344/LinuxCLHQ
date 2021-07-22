#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFSIZE 1024

int main(){
	int pd[2];
	pid_t pid;
	int len;
	char buf[BUFSIZE];


	if(pipe(pd) < 0){
		perror("pipe()");
		exit(1);
	}

	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(1);
	}

	if(pid == 0){// read, close(pd[1])
		close(pd[1]);
		dup2(pd[0], 0);
		close(pd[0]);

		int fd = open("/dev/null", O_RDWR);
		dup2(fd, 1);
		dup2(fd, 2);
		execl("/usr/bin/mpg123", "mpg123","-",NULL);
	}else{//write close(pd[0])
		close(fd[0]);
		//parent receive data from internet,write to pipe
		close(pd[1]);
		wait(NULL);
		exit(0);
	}
	exit(0);
}