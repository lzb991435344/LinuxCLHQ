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
#include <signal.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>



#include "proto.h"
#define MINPARSERVER 5
#define MAXPARSERVER 10

#define MAXCLIENTS 20

#define SIG_NOTIFY SIGUSR2
#define IPSTRSIZE 1024
#define LINEBUFSIZE 80



enum{
	STATE_IDEL = 0,
	STATE_BUSY
};

struct server_st{
	pid_t pid;
	int state;
	//int reuse;//count 
};

static struct server_st* serverpool;
static int idl_count = 0;
static int busy_count = 0;
static int sd;

static void server_job(int pos){
	pid_t ppid;
	struct sockaddr_in raddr;
	socklen_t raddr_len;
	int client_sd;
	long long  stamp;
	int len;
	char ipstr[IPSTRSIZE];
	char linebuf[LINEBUFSIZE];

	raddr_len = sizeof(raddr);
	ppid = getppid();
	while(1){
		serverpool[pos].state = STATE_IDEL;
		kill(ppid, SIG_NOTIFY);

		client_sd = accept(sd, (void*)&raddr, &raddr_len);

		if(client_sd < 0){
			if(errno != EINTR || errno != EAGAIN){
				perror("accept()");
				exit(1);
			}
		}

		serverpool[pos].state = STATE_BUSY;
		kill(ppid, SIG_NOTIFY);
		inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
		printf("[%d]client:%s:%d\n", getpid(),ipstr, ntohs(raddr.sin_port));

		stamp = time(NULL);
		len = snprintf(linebuf, LINEBUFSIZE, FMT_STAMP, stamp);
		send(client_sd, linebuf, len, 0);
		/**if error**/

		sleep(10);
		close(client_sd);
	}

}
static void usr2_handler(int s){
	return ;
}
static int add_one_server(){
	int slot;
	pid_t pid;
	if(idl_count + busy_count >= MAXCLIENTS){
		return -1;
	}
	for(slot = 0; slot < MAXCLIENTS; ++slot){
		if(serverpool[slot].pid = -1){
			break;
		}
		serverpool[slot].state = STATE_IDEL;
		pid = fork();
		if(pid < 0){
			perror("fork()");
			exit(1);
		}
		if(pid == 0){
			server_job(slot);
			exit(0);
		}else{
			serverpool[slot].pid = pid;
			idl_count++;
		}
	}
	return 0;
}


static int del_one_server(){

	int i;
	if(idl_count == 0){
		return -1;
	}

	for(i = 0; i < MAXCLIENTS; i++){
		if(serverpool[i].pid != -1 && serverpool[i].state == STATE_IDEL){
			kill(serverpool[i].pid, SIGTERM);//jingzheng
			serverpool[i].pid = -1;
			idl_count--;
			break;
		}
	}
	return 0;
}

static int scan_pool(){
	int i;
	int idel,busy;
	for(i = 0; i < MAXCLIENTS; ++i){
		if(serverpool[i].pid == -1){
			continue;
		}
		if(kill(serverpool[i].pid, 0)){
			serverpool[i].pid = -1;
			continue;
		}
		if(serverpool[i].state = STATE_IDEL){
			idel++;
		}else if(serverpool[i].state = STATE_BUSY){
			busy++;
		}else{
			fprintf(stderr, "Unknow State\n");
			abort();
		}		
	}
	idl_count = idel;
	busy_count = busy;

	return 0;
}
int main(int argc, char* argv[]){

	struct sigaction sa, osa;
	int val = 1;
	struct sockaddr_in laddr;
	int i;
	sigset_t set,oset;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_NOCLDWAIT;// no need to wait
	sigaction(SIGCHLD, &sa, &osa);

	sa.sa_handler = usr2_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIG_NOTIFY, &sa, &osa);

	sigemptyset(&set);
	sigaddset(&set, SIG_NOTIFY);
	sigprocmask(SIG_BLOCK, &set, &oset);//do not block 

	serverpool = mmap(NULL, sizeof(struct server_st) * MAXCLIENTS, 
		PROT_READ|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if(serverpool == MAP_FAILED){
		perror("mmap()");
		exit(1);
	}

	//init
	for(i = 0; i < MAXCLIENTS; i++){
		serverpool[i].pid = -1;
	}

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("socket()");
		exit(1);
	}

	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0){
		perror("setsockopt()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(SERVERPORT));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
	if(bind(sd, (void*)&laddr, sizeof(laddr)) < 0){
		perror("bind()");
		exit(1);
	}


	if(listen(sd, 100)){
		perror("listen()");
		exit(1);
	}

	//parent
	for(i = 0; i < MINPARSERVER; ++i){
		add_one_server();
	}
	

	while(1){
		sigsuspend(&oset);
		scan_pool(); //scan status
		if(idl_count > MAXPARSERVER){

			for(i = 0; i < (idl_count - MAXPARSERVER); ++i){
				del_one_server();
			}
		}else if(idl_count < MINPARSERVER){
			for( i = 0; i  < (MINPARSERVER - idl_count); ++i){
				add_one_server();
			}
		}
		for(i = 0; i < MAXCLIENTS; ++i){
			if(serverpool[i].pid == -1){
				putchar(' ');
			}else if(serverpool[i].state == STATE_IDEL){
				putchar('.');
			}else{
				putchar('x');
			}
		}
		putchar('\n');

		//control the pool
	}

	sigprocmask(SIG_SETMASK, &oset, NULL);

	exit(0);
}
