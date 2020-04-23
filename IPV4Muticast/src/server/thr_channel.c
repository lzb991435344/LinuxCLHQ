#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <proto.h>
/**
1.socketaddr_in 结构体
2.htons
*/

#include "medialib.h"
#include "thr_channel.h"
//#include "proto.h"
#include "server_conf.h"


struct thr_channel_ent_st{
	chnid_t chnid;
	pthread_t tid;
};

struct thr_channel_ent_st thr_channel[CHNNR];
static int tid_nextpos = 0;

//via socket to send
static void* thr_channel_snder(void* ptr){
	struct msg_channel_st* sbufp;
	//int datasize;
	struct mlib_listentry_st* ent = ptr;
	int len;

	sbufp = malloc(MSG_CHANNEL_MAX);
	if(sbufp == NULL){
		syslog(LOG_ERR, "malloc() err:%s", strerror(errno));
		exit(1);
	}
	syslog(LOG_INFO, "len is:%d",len);
	//datasize = MAX_DATA;
	 sbufp->chnid = ent->chnid;
	 while(1){
	 	//readchn...
	 	len = mlib_readchn(ent->chnid, sbufp->data, MAX_DATA);
	 
	 	if(sendto(serversd, sbufp, len + sizeof(chnid_t), 0, (void*)& sndaddr, sizeof(sndaddr)) < 0){
	 		syslog(LOG_ERR, "%d:sendto(serverd, sbufp..):%s",ent->chnid, strerror(errno));
	 		exit(1);
	 	}
	 	sched_yield();
	}
	syslog(LOG_INFO, "len2 is:%d",len);
	pthread_exit(NULL);
}

int thr_channel_create(struct mlib_listentry_st* ptr){
	
	int err;
	err = pthread_create(&thr_channel[tid_nextpos].tid, NULL, 
		thr_channel_snder, ptr);

	if(err){
		syslog(LOG_WARNING, "pthread_create()%s", strerror(errno));
		return -err;
	}
	thr_channel[tid_nextpos].tid = ptr->chnid;
	tid_nextpos++;
	return 0;
}

int thr_channel_destroy(struct mlib_listentry_st* ptr){
	int i;

	for(i = 0; i < CHNNR; ++i){
		if(thr_channel[i].tid == ptr->chnid){
			if(pthread_cancel(thr_channel[i].tid) < 0){
				syslog(LOG_ERR, "pthread_cancel():error %s", strerror(errno));
				return -ESRCH;
			}
		}
		pthread_join(thr_channel[i].tid, NULL);
		thr_channel[i].chnid = -1;
	}
	return 0;

}
int thr_channel_destroyall(){

	int i;
	for(i = 0; i < CHNNR; i++){
		if(thr_channel[i].chnid > 0){
			if(pthread_cancel(thr_channel[i].tid) < 0){
				syslog(LOG_ERR, "pthread_cancel() fail:%ld",thr_channel[i].tid);
				return -ESRCH;
			}
		pthread_join(thr_channel[i].tid, NULL);
		thr_channel[i].chnid = -1;
		}
	}
	return 0;

}