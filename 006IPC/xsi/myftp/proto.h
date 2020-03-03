#ifndef _PROTO_H__
#define _PROTO_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'a'
#define PATH_MAX 1024
#define DATA_MAX 1024


enum{
	MSG_PATH = 1;
	MSG_DATA,
	MSG_EOT
};

typedef struct msg_path_st{
	char path[PATH_MAX]; //ASCIIZ, string contain '0'
}msg_path_t;


typedef struct msg_data_st{ //MSG_DATA
	long mtype;
	char data[DATA_MAX];
	int datalen;
}msg_data_t;


typedef struct msg_eot_st {//MSG_EOT
	long mtype;	
}msg_eot_t;

union msg_s2c_un{
    long mtype; //4 + 4
	msg_data_t datamsg;
	msg_eot_t eotmsg;
};

#endif