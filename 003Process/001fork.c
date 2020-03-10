#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



int main(){

	pid_t pid;


	printf("[%d]:Begin!\n", getpid()); //parent

	fflush(NULL);  //import  
               //test for the different


	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(1);
	}

	//child
	if(pid == 0){
		printf("[%d]:child is working!\n", getpid());
	}else{
		//sleep(1);
		printf("[%d]:parent is working!\n", getpid());
	}

	//getchar();
	//pause();

	printf("[%d]End!\n", getpid());
	exit(0);
}