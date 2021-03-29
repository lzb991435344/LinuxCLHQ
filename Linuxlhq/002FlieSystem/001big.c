#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


//做一个5G的大文件
/**
 ./big /tmp/bigfile
 ls -l /tmp/bigfile 
 -rw------- 1 blake blake 5368709120 8月  27 21:23 /tmp/bigfile
 cp  /tmp/bigfile  /tmp/bigfile.bck

*/

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage.....\n");
		exit(1);
	}


	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if(fd < 0){
		perror("open");
		exit(1);
	}

	off_t size = lseek(fd, 5LL*1024LL*1024LL*1024LL - 1LL, SEEK_SET);
	if(size < 0){
		perror("lseek()");
		exit(1);
	}

	write(fd, "", 1);

	close(fd);
	exit(0);
}