#include "proto.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#define IPSTRSIZE 40


int main(int argc, char* argv[]){

	int sd;
	struct sockaddr_in laddr,raddr;
	socklen_t raddr_len;
	struct msg_st rbuf;
	char ipstr[IPSTRSIZE];
	printf("5555");
	sd = socket(AF_INET, SOCK_DGRAM, 0/**IPPROTO_UDP*/);
	if(sd < 0){
		perror("bind()");
		exit(1);
	}
	printf("6666");


	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(RCVPORT)); //host to net
	inet_pton(AF_INET,"0.0.0.0" ,&laddr.sin_addr);//ip

	if(bind(sd, (void*)&laddr, sizeof(laddr)) < 0){
		perror("bind()");
		exit(1);
	}



	/**import*/
	raddr_len = sizeof(raddr);
	while(1){
		recvfrom(sd, &rbuf, sizeof(rbuf), 0, (void*)&raddr, &raddr_len);
		inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
		printf("-----Message From:%s:%d\n", ipstr, ntohs(raddr.sin_port));
		
		printf("Name = %s\n",rbuf.name);
		printf("Math = %d\n",ntohl(rbuf.math));
		printf("Chinese = %d\n",ntohl(rbuf.chinese));
	}


	close(sd);

	exit(0);
}

//netstat -anu

