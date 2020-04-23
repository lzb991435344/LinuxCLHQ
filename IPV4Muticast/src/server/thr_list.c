#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <proto.h>

//#include "proto.h"
#include "thr_list.h"
#include "server_conf.h"


static pthread_t tid_list;
static int nr_list_ent;
static struct mlib_listentry_st* list_ent;


static void* thr_list(void* p){

	int totalsize;
	struct msg_list_st* entlistp;
	struct msg_listentry_st* entryp;

	totalsize = sizeof(chnid_t);

	int i;
	int size;
	int ret;
	//syslog(LOG_INFO,"desc size is %d", strlen(list_ent[i].desc));
	for(i = 0; i < nr_list_ent; ++i){
		//totalsize += sizeof(struct msg_listentry_st) + strnlen(list_ent[i].desc, MAX_ENTRY); 
		totalsize += sizeof(struct msg_listentry_st) + strlen(list_ent[i].desc);
	}

	entlistp = malloc(totalsize);

	if(entlistp == NULL){
		syslog(LOG_ERR, "malloc:%s", strerror(errno));
	}

	syslog(LOG_INFO, "totalsize is:%d",totalsize);

	entlistp->chnid = LISTCHIND;
	entryp = entlistp->entry;


	for(i = 0; i < nr_list_ent; ++i){
		size = sizeof(struct msg_listentry_st) + strlen(list_ent[i].desc); 
		entryp->chnid = list_ent[i].chnid;
		entryp->len = htons(size);

		strcpy(entryp->desc, list_ent[i].desc);
		entryp = (void*)((char*) entryp + size);
	}


	while(1){
		ret = sendto(serversd, entlistp, totalsize, 0, (void*)&sndaddr, sizeof(sndaddr));
		if(ret < 0){
			syslog(LOG_WARNING, "sendto(serversd, entlistp..)fail:%s", strerror(errno));
		}else{
			syslog(LOG_ERR, "sendto(serversd, entlistp..)success:%s", strerror(errno));
		}
		sleep(1);
	}

}


int thr_list_create(struct mlib_listentry_st* listp, int nr_ent){
	int err;
	list_ent = listp;
	nr_list_ent = nr_ent;
	err = pthread_create(&tid_list, NULL, thr_list, NULL);

	if(err){
		syslog(LOG_ERR, "pthread_create():%s", strerror(errno));
		return -1;
	}

	//syslog(LOG_INFO,"777666666");
	return 0;

}

int thr_list_destroy(void){
	pthread_cancel(tid_list);
	pthread_join(tid_list, NULL);
	return 0;
}