#ifndef _PROTO_H_
#define _PROTO_H_

#include <site_type.h>

#define DEFAULT_MGROUP "224.2.2.2"  //224 start
#define DEFAULT_RCVPORT "19899"

#define CHNNR  200
#define LISTCHIND 0
#define MINCHNID  1
#define MAXCHINID (MINCHNID+CHNNR-1)

#define MSG_CHANNEL_MAX (65536-20-8)  //2 HEADER LENGTH 
#define MAX_DATA  (MSG_CHANNEL_MAX-sizeof(chnid_t))

#define MSG_LIST_MAX (65536-20-8)
#define MAX_ENTRY (MSG_LIST_MAX-sizeof(chnid_t))

struct msg_channel_st{
	chnid_t  chnid; //[MINCHNID, MAXCHINID]
	uint8_t data[1];//after c99 can use data[0]

}__attribute__((packed)); // no need to duiqi

/** pattern
 1 music xxxxx;
 2 sport xxxx;
 3 
*/
struct msg_listentry_st{
	chnid_t chnid;
	uint16_t len;
	uint8_t desc[1];
}__attribute__((packed));


struct msg_list_st
{
	chnid_t chnid;
	struct msg_listentry_st entry[1];
}__attribute__((packed));

#endif
