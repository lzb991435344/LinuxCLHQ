#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// ./mysu 0 cat /etc/shadow

//test 
//change to root 
/**
(1)chown root 009mysu
(2) chmod u+s 009mysu
(3) ./009mysu 0 cat /etc/shadow
*/
int main(int argc, char* argv[]){

	pid_t pid;
	if(argc < 3){
		fprintf(stderr, "Usage...\n");
		exit(1);
	}
	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(1);
	}
	if(pid == 0){
		setuid(atoi(argv[1]));
		execvp(argv[2], argv + 2);
		
		perror("execvp()");
		exit(1);
	}

	wait(NULL);
	exit(0);
}