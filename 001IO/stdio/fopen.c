#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv)
{
	FILE* fp;

	fp = fopen("tmp", "r");

	if(fp == NULL)
	{
		//fprintf(stderr, "fopen() failure! errno = %d\n",errno);
		//perror("fopen()");
		fprintf(stderr, "fopen() failure! errno = %d\n",strerror(errno));
		exit(1);
	}
	puts("ok!");
    fclose(fp);
	exit(0);
}