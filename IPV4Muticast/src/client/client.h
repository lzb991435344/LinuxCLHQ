#ifndef CLIENT_H_
#define CLIENT_H_


#define DEFAULT_PLAYERCMD "/usr/local/bin/mpg123  > /dev/null"
struct client_conf_st{
	char* rcvport; //use in local,same as server
	char* mgroup;
	char* player_cmd;

};

//global,increase the scope
extern struct client_conf_st client_conf;

//static ssize_t writen(int fd, const char* buf, size_t len );
#endif