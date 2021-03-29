#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]){

	if(argc < 2){
		fprintf(stderr, "Usage....\n");
		exit(1);
	}
	int fd;
	int i;
	//int64_t
	int count;
	struct stat statres;
	char* str;
    fd = open(argv[1], O_RDONLY);
    if(fd < 0){
    	perror("open()");
    	exit(1);
    }
    if(fstat(fd, &statres) < 0){
    	perror("fstat()");
    	exit(1);
    }

    //将文件的一块数据映射到内存
    //返回值是一个宏  MAP_FAILED (void *) -1)
    str = mmap(NULL, statres.st_size, PROT_READ, MAP_SHARED, fd,0);
    if(str == MAP_FAILED){
    	perror("mmap()");
    	exit(1);
    }
    close(fd);

    for(i = 0; i < statres.st_size; ++i){
    	if(str[i] == 'a'){
    		count++;
    	}
    }
    printf("%d\n", count);
    munmap(str, statres.st_size);
	exit(0);
}