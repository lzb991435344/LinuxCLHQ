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
    fd = fileno(fp); //if error

    lockf(fd, F_LOCK, 0);
	fgets(linebuf, LINESIZE, fp);
	fseek(fp, 0, SEEK_SET);
	sleep(1);
	fprintf(fp, "%d\n", atoi(linebuf) + 1);//full buff,line buffer
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
		wait(NULL);
	}

	exit(0);
}