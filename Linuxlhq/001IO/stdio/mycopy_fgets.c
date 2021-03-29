#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
// ./mycopy srcfile destfile
#define BUFSIZE 1024
int main(int argc, char* argv[])
{
	FILE* fps,*fpd;
	char buf[BUFSIZE];

    if(argc < 3){
    	fprintf(stderr,"Usage:%s <src_file> <dest_file>\n",argv[0]);
    	exit(1);
    }

	fps = fopen(argv[1],"r");
	if(fps == NULL){
		perror("fopen()");
		exit(1);
	}

	fpd = fopen(argv[2],"w");
	if(fpd == NULL){
		fclose(fps);
		perror("fopen()");
		exit(1);
	}
	//读取到以下两个结束
	//size - 1，剩余的一个字节给补全的‘0’
	//'\n'
	//fgets() returns s on success, and NULL on error or when end of file occurs while no characters have been read.
	while(fgets(buf, BUFSIZE, fps) != NULL){
		//ch = fgetc(fps);
		//if(ch = EOF)
		//{

		//	break;
		//}
		//fputc(ch,fpd);
		fputs(buf, fpd);
	}

	fclose(fps);
	fclose(fpd);
	exit(0);
}