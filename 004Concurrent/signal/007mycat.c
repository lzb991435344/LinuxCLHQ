#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFSIZE 1024

/**
 查看文件内容
 
*/

int main(int argc, char* argv[])
{

	int sfd,dfd = 1;
	char buf[BUFSIZE];
	int len, ret, pos;
	if(argc < 2)
	{
		fprintf(stderr, "Usage: <src_file> <des_file>\n");
		exit(1);
	}
	/**

	进程在一个慢系统调用(slow system call)中阻塞时，
	当捕获到某个信号且相应信号处理函数返回时，这个系统调
	用被中断，调用返回错误，设置errno为EINTR*/
    //修改点：当被信号打断的时候，需要加上控制
    do{
    	sfd = open(argv[1], O_RDONLY);
		if(sfd < 0)
		{   
			if(errno != EINTR){ //真错
				perror("open()");
			  	exit(1);
		    }
		}
	//当sfd < 0 时再做一次open()
    }while(sfd < 0);
	  
	while(1)
	{
		len = read(sfd, buf, BUFSIZE);
		if(len < 0)
		{
			//假错，被信号中断
			if(errno == EINTR){
				continue;
			}
			//真错
			perror("read()");
			exit(1);
		}
		if(len == 0)
		{
			break;
		}

		pos = 0;
		//防止讀取的字節數一次讀不完
		//读取的长度作为循环条件
		while(len > 0)
		{ 
			//dfd == 1 标准输出
			ret = write(dfd, buf + pos, len);
			if(ret < 0)
			{
				//被信号打断
				if (errno == EINTR){
					continue;
				}
				//出现错误
				perror("write()");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}
	}
	close(sfd);
	exit(0);
}
