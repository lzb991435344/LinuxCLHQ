#include <stdio.h>
#include <stdlib.h>



int main(int argc, char* argv[])
{

	if(argc < 2)
	{
		fprintf(stderr,"Usage:%s <src_file> <dest_file>\n",argv[0]);
		exit(1);
	}

	FILE* fp;
	//int count = 0;
	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		perror("fopen");
		exit(1);
	}
	
	// int fseek(FILE *stream, long offset, int whence);
	fseek(fp, 0, SEEK_END);

	// off_t ftello(FILE *stream); 文件指针相对于文件首偏移的位置
	printf("%ld\n", ftell(fp));
	fclose(fp);
	exit(0);
}

