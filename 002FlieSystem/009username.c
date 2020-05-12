
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>

int main(int argc, char* argv[]){

	struct passwd* passwdline;

	if(argc < 2){
		fprintf(stderr, "Usage.....\n");
		exit(1);
	}

	//get password file entry
	//struct passwd *getpwuid(uid_t uid);
	passwdline = getpwuid(atoi(argv[1]));

	//struct passwd *getpwuid(uid_t uid);
	puts(passwdline->pw_name);//打印用户名
	exit(0);
}