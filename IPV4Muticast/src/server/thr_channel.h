#ifndef THR_CHANNEL_H__
#define THR_CHANNEL_H__

int thr_channel_create(struct mlib_listentry_st* ptr);
int thr_channel_destroy(struct mlib_listentry_st* ptr);
int thr_channel_destroyall();

#endif 