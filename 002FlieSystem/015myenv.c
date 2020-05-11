#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char** environ; //全局的环境指针

int main(){
	
	int i;
	//打印环境参数
	for(i = 0; environ[i] != NULL; ++i){
		puts(environ[i]);
	}
	exit(0);
}