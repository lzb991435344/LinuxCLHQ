#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]){

	int sfd,dfd;
	char buf[BUFSIZE];
	int len, ret, pos;
	if(argc < 3){
		fprintf(stderr, "Usage:%s <src_file> <des_file>\n");
		exit(1);
	}

	sfd = open(argv[1], O_RDONLY);
	if(sfd < 0){
		perror("open()");
		exit(1);
	}

	dfd = open(argv[2], O_WRONLY | O_CREAT, O_TRUNC, 0600);
	if(dfd < 0){
		close(sfd);
		perror("open()");
		exit(1);
	}

    
	while(1){
		len = read(sfd, buf, BUFSIZE);
		if(len < 0){
			perror("read()");
			exit(1);
		}
		if(len == 0){
			break;
		}

		pos = 0;//从0 位置开始读取
		//防止讀取的字節數一次讀不完
		//读取到len字节
		while(len > 0){
			//写进了ret个字节
			ret = write(dfd, buf + pos, len);
			//写入失败
			if(ret < 0) {
				perror("write()");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}
	}

	exit(0);
}
