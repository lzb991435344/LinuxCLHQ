#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFSIZE 1024

/**
 拷贝文件到特定位置，比对文件大小
./mycpy /etc/service /tmp/out
diff /etc/service /tmp/out  比对文件大小
*/
int main(int argc, char* argv[])
{

	int sfd,dfd;
	char buf[BUFSIZE];
	int len, ret, pos;
	if(argc < 3)
	{
		fprintf(stderr, "Usage:%s <src_file> <des_file>\n");
		exit(1);
	}

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
	
    do{
		dfd = open(argv[2], O_WRONLY | O_CREAT, O_TRUNC, 0600);
		if(dfd < 0)
		{
			close(sfd);
			perror("open()");
			exit(1);
		}
    }while(dfd < 0);
    


	while(1)
	{
		len = read(sfd, buf, BUFSIZE);
		if(len < 0)
		{
			//假错
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
		while(len > 0)
		{ 
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
			pos +=ret;
			len -= ret;
		}
	}
	close(sfd);
	close(dfd);
	exit(0);
}

