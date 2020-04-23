#include <stdio.h>
#include <stdlib.h>
//#include "../include/proto.h"
#include <proto.h>
#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <net/if.h>

//change port and address
/** short and long command
*  -M  --mgroup
*  -P  --port
*  -p  --player
*  -H  --help
**/

 static ssize_t writen(int fd, const char* buf, size_t len);

struct client_conf_st client_conf = {\
	.rcvport = DEFAULT_RCVPORT,\
	.mgroup = DEFAULT_MGROUP,\
	.player_cmd = DEFAULT_PLAYERCMD
};

static void printfhelp(void){

	printf("-P --port use the port \n");
	printf("-M --mgroup use the group\n");
	printf("-p --player use the player\n");
	//printf("");

}

 static ssize_t writen(int fd, const char* buf, size_t len){
	size_t pos = 0;
	int ret;
	while(len > 0){
	    ret = write(fd, buf + pos, len);
		if(ret < 0){
			if(errno == EINTR){
				continue;
			}
			perror("write()");
			return -1;
		}
		len -= ret;
		pos += ret;
	}
	return pos;	
}

int main(int argc,char* argv[]){

	/**
	* init 	
	*(1)default  (2)config file (3)PATH (4)argv
	*
	**/
	int ret;
	int index = 0;
	int val;
	int c;
	int sd;
	int pd[2];
	pid_t pid;

	struct ip_mreqn mreq;
	struct sockaddr_in laddr;
	struct msg_list_st* msg_list;
	struct sockaddr_in serveraddr;
	socklen_t serveraddr_len;
	int len;

	struct msg_listentry_st* pos;

	int choseid;

	struct msg_channel_st* msg_channel;
	struct sockaddr_in raddr;
	socklen_t raddr_len;
	//int ret;
	//int pos;

	struct option argarr[] ={{"port", 1, NULL, 'P'},{"mgroup", 1, NULL, 'M'},\
	{"player", 1, NULL, 'p'},{"H", 1, NULL, 'H'},{NULL, 0, NULL, 0}};

	while(1){
		c = getopt_long(argc, argv, "P:M:p:H",argarr, &index);
		if(c < 0){
			break;
		}
		switch(c){
			case 'P':
			client_conf.rcvport = optarg;
			break;

			case 'M':
			client_conf.mgroup = optarg;
			break;

			case 'p':
			client_conf.player_cmd = optarg;
			break;

			case 'H':
			printfhelp();
			exit(0);
			break;

			default:
			abort();
			break;

		}
	}
    
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0){
		perror("socket()");
		exit(1);
	}

	 inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
	 inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
	 mreq.imr_ifindex = if_nametoindex("eth0");
	 /*if err*/
	//man 7 ip
	if(setsockopt(sd, IPPROTO_IP,  IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
		perror("setsockopt()");
		exit(1);
	}

	val = 1;
	if(setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &val, sizeof(val)) < 0){

		perror("setsockopt()");
		exit(1);
	}


	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(client_conf.rcvport));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
	if(bind(sd, (void*)&laddr, sizeof(laddr)) < 0){
		perror("bind()");
		exit(1);
	}

	//recvfrom();
	if(pipe(pd) < 0){
		perror("pipe()");
		exit(1);
	}

	pid = fork();

    if(pid < 0){
    	perror("fork()");
    	exit(1);
    }

    //decode
    if(pid == 0){
    	close(sd);
    	close(pd[1]);
    	
    	dup2(pd[0], 0); //stdin
    	if(pd[0] > 0){
    		close(pd[0]);
    	}

        ///execlp();
        execl("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL);
        perror("execl()");
        exit(1);

    	//mpg123 aa.mp3

    }else if(pid > 0){ 
    //recv list
    	

    	msg_list = malloc(MSG_LIST_MAX);
    	if(msg_list == NULL){
    		perror("malloc()");
    		exit(1);
    	}

    	serveraddr = sizeof(serveraddr);
    	while(1){
    		len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void*)&serveraddr, &serveraddr_len);
    		if(len < sizeof(struct msg_list_st)){
    			fprintf(stderr, "message is too small\n");
    			continue;
    		}
    		if(msg_list->chnid != LISTCHIND){
    			fprintf(stderr, "chnid is not match\n");
    			continue;
    		}
    		break;
    	}
    }

    //choose chnnel and print

    for(pos = msg_list->entry; (char*)pos < (((char*)msg_list) + len);pos = (void*)(((char*)pos) + ntohs(pos->len))){
    	printf("channel %d : %s\n", pos->chnid, pos->desc);

    }

    free(msg_list);
     while(1){
     	ret = scanf("%d\n", &choseid);
     	if(ret != 1){
     		exit(1);
     	}
     }

    	
   
    //recv package,send chnnel data to child
     fprintf(stdout, "choseid = %d\n", ret);

     msg_channel = malloc(MSG_CHANNEL_MAX);
     if(msg_channel == NULL){
     	perror("malloc()");
     	exit(1);
     }

     while(1){
     	len = recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (void*)&raddr, &raddr_len);
     	//recv addr equals chanel addr
     	if(raddr.sin_addr.s_addr != serveraddr.sin_addr.s_addr
     		|| raddr.sin_port != serveraddr.sin_port){
     		fprintf(stderr, "Ignore:address not match");
     		continue;
     	}
     	if(len < sizeof(struct msg_channel_st)){
     		fprintf(stderr, "Ignore:meaasge too small\n");
     		continue;
     	}
     	if(msg_channel->chnid == choseid){
     		fprintf(stdout, "accepted msg:%d receive\n", msg_channel->chnid);
     		if (writen(pd[1], (const char*)msg_channel->data, len - sizeof(chnid_t)) < 0){
     			exit(1);
			} 
     	}
     }

     free(msg_channel);
     close(sd);

	exit(0);
}