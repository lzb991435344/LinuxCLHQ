#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//返回值 off_t 增加Makefile中的文件
static off_t flen(const char* fname)
{
	struct stat st;
	if(stat(fname, &st) == -1)
	{
		perror("stat()");
		exit(1);
	}
	return st.st_size;
}
int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage....\n");
		exit(1);
	}
	printf("%lld\n",(long long)flen(argv[1]));
	exit(0);
}