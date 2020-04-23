
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <net/if.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <proto.h>


#include "server_conf.h"
#include "medialib.h"
#include "thr_channel.h"
#include "thr_list.h"
#include "mytbf.h"
//#include "proto.h"
//


//core find seg err
//1. Makefile + -g
//2.ulimit -c unlimited
//3. ./servertest -F
//4.gdb servertest core or gdb -q servertest core
//6. bt


struct server_conf_st server_conf = {\
	.rcvport = DEFAULT_RCVPORT,\
	.mgroup = DEFAULT_MGROUP,\
	.media_dir = DEFAULT_MEDIADIR,\
	.runmode = RUN_DAEMON,\
	.ifname = DEFAULT_IF
};

//global
int serversd;
struct sockaddr_in sndaddr;
struct mlib_listentry_st* list;

void printhelp(void){
	printf("-M  multicast\n");
	printf("-P  recv a port\n");
	printf("-F  run in front\n");
	printf("-D  midia place ");
	printf("-I  interface eth0,network device");
	printf("-H  help");

}
static void daemon_exit(int s){

	thr_list_destroy();
	thr_channel_destroyall();
	mlib_freechnlist(list);

	syslog(LOG_WARNING, "signal-%d caught,exit", s);

	closelog();
	exit(0);
}


static int  daemonize(void){
	/*creates a new session if the calling process is not a process
     group leader.The calling process is the leader of the new session
     The calling process also becomes the process group leader of a new 
     process group in the session*/


	pid_t pid;
	int fd;
	pid = fork();
	if(pid < 0){
		//perror("fork()");
		syslog(LOG_ERR,"fork():%s",strerror(errno));
		return -1;
	}

	if(pid > 0){
		exit(0);
	}


	fd = open("/dev/null",O_RDWR);
	if(fd < 0){
		//perror("open()");
		syslog(LOG_WARNING,"open():%s",strerror(errno));
		return -2;
	}else{
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);

		if(fd > 2){
			close(fd);
		}
	}

	
	setsid();//new session,

	chdir("/");
	umask(0);

	return 0;
}

static int socketinit(void){
	
	struct ip_mreqn mreq;

	serversd = socket(AF_INET, SOCK_DGRAM, 0);
	if(serversd < 0){
		syslog(LOG_ERR, "socket():%s",strerror(errno));
	}

    inet_pton(AF_INET, server_conf.mgroup, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0",&mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex(server_conf.ifname);
    if (setsockopt(serversd,IPPROTO_IP, IP_MULTICAST_IF, &mreq,sizeof(mreq)) < 0){
    	syslog(LOG_ERR,"setsockopt(IP_MULTICAST_IF):%s" ,strerror(errno));
    	exit(1);
    }
    
    sndaddr.sin_family = AF_INET;
    sndaddr.sin_port = htons(atoi(server_conf.rcvport));
    inet_pton(AF_INET, server_conf.mgroup, &sndaddr.sin_addr.s_addr);
}


int main(int argc, char* argv[]){

  //argc,argv---command,short
  /**
	-M  multicast
	-P  recv a port
	-F  run in front
	-D  midia place 
	-I  interface eth0,network device
	-H  help
  */ 
	int c;
	struct sigaction sa;


	//signal teminate process
	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT | SIGTERM | SIGQUIT);


	sigaction(SIGINT | SIGTERM | SIGQUIT, &sa, NULL);
	
	openlog("ipv4Muticast",LOG_PID | LOG_PERROR, LOG_PERROR);
	while(1){
		c = getopt(argc, argv, "M:P:FD:I:H");
		if(c < 0){
			break;
		}

		switch(c){
			case 'M':
			server_conf.mgroup = optarg;
			break;

			case 'P':
			server_conf.rcvport = optarg;
			break;

			case 'F':
			server_conf.runmode = RUN_FOREGROUND;
			break;

			case 'D':
			server_conf.media_dir = optarg;
			break;

			case 'I':
			server_conf.ifname = optarg;
			break;

			case 'H':
			printhelp();
			exit(0);
			break;

			default:
			break;
		}
	}
	


    //daemon
	if(server_conf.runmode == RUN_DAEMON){
		if(daemonize() != 0){
			exit(1);
		}
	}else if(server_conf.runmode == RUN_FOREGROUND){
		/**do nothing*/
	}else{
		//fprintf(stderr, "EINVAL\n");
		syslog(LOG_ERR,"EINVAL server_conf.runmode");
		exit(1);
	}
  

   //init socket
	socketinit();

	
	int listsize;
	int err;
  //get channel message  medialib.c
	err = mlib_getchnlist(&list, &listsize);

  //create list thread  thr_list

	thr_list_create(list, listsize);
    
    syslog(LOG_INFO, "listsize is %d",listsize);
  //create chanel thread  thr_channel
   int i;
   for(i = 0; i < listsize; ++i){
	 err = thr_channel_create(list + i);
	 if(err){
	 	fprintf(stderr, "thr_channel_create()fail:%s\n", strerror(errno));
	 	exit(1);
	 }
   }

   syslog(LOG_INFO, "i is %d",i);
   syslog(LOG_DEBUG,"%d channel threads create", i);
   while(1){
	 pause();
   }

    
	exit(0);
}