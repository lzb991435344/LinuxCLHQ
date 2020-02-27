#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


static int ftype(const char* fname)
{
	struct stat st;
	if(stat(fname, &st) == -1)
	{
		perror("stat");
		exit(1);
	}

	if(S_ISREG(st.st_mode))
	{
		return '-';
	}
	else if(S_ISDIR(st.st_mode))
	{
		return 'd';
	}
	else if(S_ISSOCK(st.st_mode))
	{
		return 's';
	}
	else
	{
		return '?';
	}

}
int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage:.....\n");
		exit(1);
	}
	printf("%c\n", ftype(argv[1]));
	exit(0);
}