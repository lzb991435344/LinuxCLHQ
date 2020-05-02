
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define CPS 10
#define BUFSIZE CPS


//场景：播放，发数据包
//使用alarm()来做流量控制，每秒写多少字符
//漏桶的实现
static volatile int loop = 0;

static void alrm_handler(int s){
	alarm(1);//为自己发出下一秒的时钟信号
	loop = 1;
}

int main(int argc, char* argv[])
{

	int sfd,dfd = 1;
	char buf[BUFSIZE];
	int len, ret, pos;
	if(argc < 2)
	{
		fprintf(stderr, "Usage:<src_file> <des_file>\n");
		exit(1);
	}
	
    signal(SIGALRM, alrm_handler);
    alarm(1);

    //修改点：当被信号打断的时候，需要加上控制
    do{
    	sfd = open(argv[1], O_RDONLY);
		if(sfd < 0){   
			if(errno != EINTR){ //真错
				perror("open()");
			  	exit(1);
		    }
		}
	//当sfd < 0 时再做一次open()
    }while(sfd < 0);
	  
	while(1){
		while(!loop){ //loop == 0时等待alarm信号到来
			pause();//阻塞，等待信号到来
		}
		//loop == 1 时改变loop的值
		loop = 0;

		//sfd是一个设备，当大量的数据到来时，只能等待读取
		while((len = read(sfd, buf, BUFSIZE)) < 0){
			//假错
			if(errno == EINTR){
				continue;
			}
			//真错
			perror("read()");
			exit(1);
		}

		if(len == 0){
			break;
		}

		pos = 0;
		//防止讀取的字節數一次讀不完
		while(len > 0){ 
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
		//方案1：sleep(1);
	}
	close(sfd);
	exit(0);
}
