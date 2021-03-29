//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
//#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


#include "mytbf.h"
#define CPS 10
#define BUFSIZE 1024
#define BURST 100 //最大上限

//场景：播放，发数据包
//使用alarm()来做流量控制，每秒写多少字符
//信号令牌桶的实现


static volatile int token = 0;

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
	int size;
	struct mytbf_st* tbf;
	if(argc < 2)
	{
		fprintf(stderr, "Usage:%s <src_file> <des_file>\n",strerror(errno));
		exit(1);
	}
	
    tbf = mytbf_init(CPS, BURST);
    if(tbf == NULL){
    	fprintf(stderr,"init failure\n");
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
	  
	while(1)
	{
	    size = mytbf_fetchtoken(tbf, BUFSIZE);
		if(size < 0){
			fprintf(stderr,"mytbf_fetchtoken failed\n");
		}
		//sfd是一个设备，当大量的数据到来时，只能等待读取
		while((len = read(sfd, buf, size)) < 0){
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

		if(size - len > 0){
			//归还令牌
			mytbf_returntoken(tbf, size - len);
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
		//方案1：sleep(1);
	}
	close(sfd);
	mytbf_destory(tbf);
	exit(0);
}
