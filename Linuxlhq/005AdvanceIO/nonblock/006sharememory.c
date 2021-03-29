#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

 #include <sys/types.h>
 #include <sys/wait.h>

#define MEMSIZE 1024

int main(int argc, char* argv[]){


	char* ptr;
	int pid;
	//parent read,chile write
	ptr = mmap(NULL, MEMSIZE, PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(ptr == MAP_FAILED){
		perror("mmap())");
		exit(1);
	}
	pid = fork();
	if(pid < 0){
		perror("fork()");
		munmap(ptr, MEMSIZE);
		exit(1);
	} 
	if(pid == 0){
		//write
		strcpy(ptr, "hello!");
		munmap(ptr, MEMSIZE);
		exit(0);
	}else{//parent
		wait(NULL);
		//read
		puts(ptr);
		munmap(ptr, MEMSIZE);
		exit(0);
	}


	exit(0);	
}
