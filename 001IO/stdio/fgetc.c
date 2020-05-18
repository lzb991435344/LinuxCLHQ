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
	int count = 0;
	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		perror("fopen");
		exit(1);
	}
	//计算文件中字符的数量
	while(fgetc(fp) != EOF)
	{
		count++;
	}

	printf("count = %d\n",count);
	fclose(fp);
	exit(0);
}

