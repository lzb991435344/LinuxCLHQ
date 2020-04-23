#ifndef _PROTO_H_
#define _PROTO_H_
#include <stdint.h>

#define NAMESIZE 11 
#define RCVPORT "19890" //use char,use atoi to integer 
//ip in argv

struct msg_st{
	uint8_t name[NAMESIZE];
	uint32_t math;
	uint32_t chinese;
}__attribute__((packed));




#endif 