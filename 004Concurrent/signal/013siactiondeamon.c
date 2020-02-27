#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define FNAME "/tmp/out"

static  FILE* fp;
//SIGINT,SIG
//a,b,c can use the same function to exit
void deamon_exit(int s){
	fclose(fp);
	closelog();
	exit(0); //normal end the program
}

int daemonize(){
	pid_t pid;
	int fd;

	pid = fork();
	if(pid < 0){
		//perror("fork()");
		return -1;
	}
	if(pid > 0){ //parent
		exit(0);
	}

	//child
	fd = open("/dev/null", O_RDWR);
	if(fd < 0){
		//perror("open()");
		return -1;
	}

	//redirect file description
	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);

	if(fd > 2){
		close(fd);
	}

	setsid();
	chdir("/");//it must have a directory

	umask(0);//if your program will not produce file
	return 0;
}


int main(int argc, char*argv[]){
	
	int i;
	struct sigaction sa;

	openlog("mydaemon",LOG_PID, LOG_DAEMON);
	
	//call the dunction several times,it will
	//cause memory leak
	//signal(SIGINT, deamon_exit);
	//signal(SIGQUIT, deamon_exit);
	//signal(SIGTERM, deamon_exit);

    //avoid the reentrant function
	sa.sa_handler = deamon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGQUIT | SIGTERM |SIGINT);
	sa.sa_flags = 0;

	sigaction(SIGINT | SIGQUIT | SIGTERM , &sa, NULL);

	if(daemonize()){
		syslog(LOG_ERR, "daemonize() failed!");
		exit(1);
	}else{
		syslog(LOG_INFO,"daemonize() success!");
	}

	fp = fopen(FNAME,"w");
	if (fp == NULL){
		//perror("fopen()");
		syslog(LOG_ERR, "fopen:%s",strerror(errno));
		exit(1);
	}

	syslog(LOG_INFO, "%s was opend!",FNAME);
	for(i = 0; ; ++i){
		//line
		fprintf(fp, "%d\n", i);
		fflush(fp);//file is full pattern
		syslog(LOG_DEBUG,"%d is printed!",i);
		sleep(1);
	}
	//it will not run here
	//fclose(fp);
	//closelog();
	exit(0);
}