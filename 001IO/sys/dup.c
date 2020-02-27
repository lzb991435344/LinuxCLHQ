#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define FNAME "/tmp/out"

int main(int argc, char const *argv[])
{
	int fd;
	fd = open(FNAME, O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if(fd < 0)
	{
		perror("open()");
		exit(1);
	}

	/*以下兩句相當與dup2(fd,1);但是不是原子操作
	close(1);
	dup(fd);
	*/
	// 1號是標準輸出，會有特定的輸出位置，需要做一個還原
	// 內存泄漏
	// 模塊思想，不只是在寫main函數 
	dup2(fd, 1); //原子操作

	if(fd != 1)
	{
		close(fd);
	}

    /***************************限定以下內容不可改變，對輸出的內容進行重定向*/
	puts("i am a boy!\n");
	exit(0);
}
