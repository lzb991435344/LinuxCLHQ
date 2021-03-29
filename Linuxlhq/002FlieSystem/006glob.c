#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glob.h>


//test 
//ls /etc/a*.conf
#define PAT "/etc/a*.conf"

#if 0
static int errfunc_(const char* errpath, int eerrno){

	puts(errpath);
	fprintf(stderr, "ERR is:%s\n", strerror(eerrno));

	return 0;
}

#endif


int main(int argc, char* argv[]){
	

	glob_t globres;
	int err;
	int i;

	//find pathnames matching a pattern, free memory from glob()
	err = glob(PAT, 0, NULL, &globres);
	if(err){
		printf("Errcode is %d", err);
		exit(1);
	}

	for(i = 0; i < globres.gl_pathc; ++i){
		puts(globres.gl_pathv[i]);
	}
	globfree(&globres);
	exit(0);
}