#ifndef MEDIALIB_H__
#define MEDIALIB_H__

//#include "proto.h"
#include <proto.h>

struct mlib_listentry_st{
	chnid_t chnid;
	char* desc;
};

int mlib_getchnlist(struct mlib_listentry_st** result, int* resnum);
ssize_t mlib_readchn(chnid_t chnid, void* buf, size_t size);
int mlib_freechnlist(struct mlib_listentry_st*);
#endif 