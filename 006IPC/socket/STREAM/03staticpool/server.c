#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <unistd.h>
 #include <time.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "proto.h"
#define IPSTRSIZE 40
#define BUFSIZE 1024

#define PROCNUM 4
//netstat -antp/-anup
//telnet
//nc 127.0.0.1 19899

//140 
static void server_loop(int sd);

static void server_job(int sd){

	char buf[BUFSIZE];
	int len;

	//save to buf
	len = sprintf(buf, FMT_STAM, (long long)time(NULL));
	if(send(sd, buf, len, 0) < 0){
		perror("send()");
		exit(1);
	}

	//sleep(10);
}


int main(){


	int sd;
	int val = 1;
	struct sockaddr_in laddr;//local
	pid_t pid;
	int i;

	sd = socket(AF_INET, SOCK_STREAM, 0/**IPPROTO_TCP,IPPROTO_STCP*/);
	if(sd < 0){
		perror("socket()");
		exit(1);
	}

	//SO_REUSEADDR--reuse addr
	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))  < 0){
		perror("setsockopt()");
		exit(1);
	}


	laddr.sin_family = AF_INET;
	laddr.sin_port =htons(atoi(SERVERPORT));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
	if(bind(sd, (void*)&laddr, sizeof(laddr)) < 0){
		perror("bind()");
		exit(1);
	}
	//backlog  maxconnect
	if(listen(sd, 200) < 0){
		perror("listen()");
		exit(1);
	}
	for(i = 0; i< PROCNUM; ++i){
		pid = fork();
		if(pid < 0){
			perror("fork()");
			exit(1);
		}
		if(pid == 0){
			server_loop(sd);
			exit(0);
		}
	}

	for(i = 0; i < PROCNUM; ++i){
		wait(NULL);
	}

	close(sd);
	exit(0);
}

static void server_loop(int sd){
	struct sockaddr_in raddr;//remote
	socklen_t raddr_len;
	raddr_len = sizeof(raddr);
	
	int newsd;
	char ipstr[IPSTRSIZE];
	while(1){
		//newsd------clientfd
		//accept.....lock,signal connect
		newsd = accept(sd, (void*)&raddr, &raddr_len); 
		if(newsd < 0){
			perror("accept()");
			exit(1);
		}

		inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
		printf("[%d]Client:%s:%d\n", getpid(), ipstr, ntohs(raddr.sin_port));
		
		server_job(newsd); //bingfa  process or thread
		close(newsd);	
	}
	
	close(sd);
}