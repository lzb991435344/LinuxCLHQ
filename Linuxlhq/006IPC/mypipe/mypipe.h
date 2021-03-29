
#ifndef  _MYPIPE_H__
#define _MYPIPE_H__

#define PIPESIZE 1024

#define MYPIPE_READ 0x00000001UL
#define MYPIPE_WRITE 0x0000002UL

typedef void mypipe_t;


mypipe_t* mypipe_init(void);

int mypipe_register(mypipe_t*,int opmap);
int mypipe_unregister(mypipe_t*,int opmap);
int mypipt_read(mypipe_t*, void* buf, size_t count);
int mypipe_write(mypipe_t*, const void* buf, size_t size);
int mypipe_destroy(mypipe_t*);








#endif 
