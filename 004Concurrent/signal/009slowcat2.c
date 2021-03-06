
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
#define BURST 100 //最大上限

//场景：播放，发数据包
//使用alarm()来做流量控制，每秒写多少字符
//信号令牌桶的实现
//static volatile int token = 0;
//to ensure one instruction can finish the job
static volatile sig_atomic token = 0;

//函数每秒执行一次
static void alrm_handler(int s){
	alarm(1);
	token++;
	if(token > BURST){
		token = BURST;
	}
}

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
	
    signal(SIGALRM, alrm_handler);
    alarm(1);

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
		while(token <= 0){
			//alarm signal is comming
			pause();
		}
		//token = token -1;
		//may be is not one instruction
		token--;

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

		if(len == 0)
		{
			break;
		}

		pos = 0;//从文件初始位置进行读取
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
			pos += ret;//改变读取到字节数的位置
			len -= ret;//计算剩余的字节数，待继续读取
		}
		//方案1：sleep(1);
	}
	close(sfd);
	exit(0);
}
