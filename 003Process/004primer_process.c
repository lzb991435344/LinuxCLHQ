#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define LEFT 30000000
#define RIGHT 30000200


// ./003primer | wc -l  result is 18
// time  ./003primer

int main(){

	int i;
	int j;
	int mark;
	pid_t pid;

	for(i = LEFT; i <= RIGHT; ++i){

		pid = fork();

		if(pid < 0){
			perror("fork()");
			exit(1);
		}

		if(pid == 0){
			mark = 1;
			for(j = 2; j < i/2; ++j){
				if(i % j == 0){
					mark = 0;
					break;
				}
			}
			if(mark){
				printf("%d is a primer\n",i);
			}
			//sleep(1000); //process status S
			exit(0);//must,or else 200! processes
		}	
	}
	//sleep(1000); //process status Z

	//int st;
	for(i = LEFT; i <= RIGHT; ++i){
		wait(NULL);
	}
	exit(0);
}

