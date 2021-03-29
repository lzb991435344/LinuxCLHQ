#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


//我们使用的shell是一个是循环


//自己实现的shell
//外部命令：在磁盘中能够调用的二进制文件
//内部命令：会话功能，登录等

//分隔符
//从文件中取数据，从终端中取数据
#define DELIMS "\t\n"


//使用变长的结构
struct cmd_st {
	glob_t globres;
};
//print $
//打印提示符号
static void promt(void){
	printf("mysh-0.1$");
}

//argv[0],argv[1]

//strtok()
//strsep()


//解析拿到的一行命令
// ls -al /etc /aa/bb/cc   abcdef
static void  parse(char* line, struct cmd_st* res){
	char* tok;
	struct cmd_st cmd;
	int i = 0;

	//死循环
	while(1){
		//extract token from string
		tok = strsep(&line, DELIMS);
		if(tok == NULL){
			break;
		}
		if(tok[0] == '\0'){
			continue;
		}

		//act like argv[]   glob_t flag:GLOB_NOCHECK
		//GLOB_NOCHECK 内容不匹配，返回，直接存
		//GLOB_APPEND 追加功能，将接收到的命令直接追加到一起，第一次追加，
		//以后都不追加，按位或追加

		glob(tok, GLOB_NOCHECK | GLOB_APPEND*i, NULL, &res->globres);
		i = 1;//first time cannot be appended,i = 0
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

		//获取一行
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
				//   ls 与 /bin/ls
				//实际命令是写ls
				execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
				perror("execvp()");
				exit(1);
			}else{
				//等待子进程消亡
				wait(NULL);
			}
		}
	}

	exit(0);
}