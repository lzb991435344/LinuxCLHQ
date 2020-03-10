#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define DELIMS "\t\n"

struct cmd_st
{
	glob_t globres;
};
//print $
static void promt(void){
	printf("mysh-0.1$");
}

//argv[0],argv[1]

//strtok()
//strsep()

static void  parse(char* line, struct cmd_st* res){
	char* tok;
	struct cmd_st cmd;
	int i = 0;

	while(1){
		tok = strsep(&line, DELIMS);
		if(tok == NULL){
			break;
		}
		if(tok[0] == '\0'){
			continue;
		}

		//act like argv[]   glob_t flag:GLOB_NOCHECK
		glob(tok, GLOB_NOCHECK | GLOB_APPEND*i, NULL, &res->globres);
		i = 1;//first time cannot be appended,
	}

}


int main(int argc, char* argv[]){

	pid_t pid;
	//must be inited
	char* linebuf = NULL;
	size_t linbufsize = 0;
	struct cmd_st cmd;

	while(1){

		promt();
		if(getline(&linebuf, &linbufsize, stdin) < 0){
			break;
		}

		parse(linebuf, &cmd);

		//inner command
		if(0){

		}else{//outer command 
			pid = fork();
			if(pid < 0){
				perror("fork()");
				exit(1);
			}

			if(pid == 0){
				execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
				perror("execvp()");
				exit(1);
			}else{
				wait(NULL);
			}
		}
	}

	exit(0);
}