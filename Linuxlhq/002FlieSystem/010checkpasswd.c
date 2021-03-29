#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>
#include <crypt.h>


//root用户执行

int main(int argc, char** argv){
	
	char* input_pass;
	struct spwd* shadowline;
	char* crypted_pass;

	if(argc < 2){
		fprintf(stderr, "Usage....\n");
		exit(1);
	}

	// getpass - get a password
	// char *getpass(const char *prompt); 获取密码
	input_pass = getpass("passwd:");
	
	//通过username获取shadow文件中的一行
	shadowline = getspnam(argv[1]); //argv[1]是用户名
	
	// char *sp_pwdp; Encrypted password 加密密码
	crypted_pass = crypt(input_pass, shadowline->sp_pwdp);
	if(strcmp(shadowline->sp_pwdp, crypted_pass) == 0){
		puts("Ok!");
	}else{
		puts("Fail!");
	}

	exit(0);
}