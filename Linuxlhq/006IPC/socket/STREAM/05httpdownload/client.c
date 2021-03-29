#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
//#include "proto.h"

int main(int argc, char*  argv[]){

	int sd;

	struct sockaddr_in raddr;
	FILE* fp;
	long long stamp;
	char rbuf[BUFSIZE];
	int len;

	if(argc < 2){
		fprintf(stderr, "Usage,,\n");
		exit(1);
	}

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("socket()");
		exit(1);
	}
	//bind();

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(80);
	inet_pton(AF_INET,argv[1], &raddr.sin_addr);

	if(connect(sd, (void*)& raddr, sizeof(raddr)) < 0){
		perror("connect()");
		exit(1);
	}

	fp = fdopen(sd, "r+");
	if(fp == NULL){
		perror("fdopen()");
		exit(1);
	}

	fprintf(fp, "GET /tays.jpg\r\n\n");
	fflush(fp);

	while(1){
		len = fread(rbuf, 1, BUFSIZE, fp);
		if(len <= 0){
			break;
		}
		fwrite(rbuf, 1, len, stdout);
	}

	/**if(fscanf(fp, FMT_STAM, &stamp) < 1){
		fprintf(stderr, "Bad format\n");
	}else{
		fprintf(stdout, "stamp is:%lld\n", &stamp);
	}*/
	fclose(fp);
	//recv();
	//close();

	exit(0);
}

//test
// ./client 127.0.0.1 > tmp/out
//  eog /tmp/out