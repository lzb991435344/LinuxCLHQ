#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define LEFT 30000000
#define RIGHT 30000200
#define N 3

// ./003primer | wc -l  result is 18
// time  ./003primer

int main(){

	int i;
	int j;
	int mark,n;
	pid_t pid;
    
    for(n = 0; n < N; ++n){

    	pid = fork();
    	if(pid < 0){
    		perror("fork()");
    		exit(1);
    	}

    	if(pid == 0){
    		for(i = LEFT + n; i <= RIGHT; i += N){
				mark = 1;
				for(j = 2; j < i/2; ++j){
					if(i % j == 0){
						mark = 0;
						break;
					}
				}
				if(mark){	
					printf("[%d]%d is a primer\n", n, i);
				}
			}
			exit(0);	
    	}
    }


    for(n = 0; n < N; ++n){
    	wait(NULL);
    }
	

	exit(0);
}

/**[0] cannot get the task
[2]30000023 is a primer
[1]30000001 is a primer
[2]30000041 is a primer
[1]30000037 is a primer
[2]30000059 is a primer
[1]30000049 is a primer
[2]30000071 is a primer
[1]30000079 is a primer
[2]30000083 is a primer
[1]30000109 is a primer
[2]30000137 is a primer
[1]30000133 is a primer
[2]30000149 is a primer
[1]30000163 is a primer
[2]30000167 is a primer
[1]30000169 is a primer
[1]30000193 is a primer
[1]30000199 is a primer


*/