#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
// ./mycopy srcfile destfile
#define BUFSIZE 1024
int main(int argc, char* argv[])
{
	FILE* fps,*fpd;
	char buf[BUFSIZE];
	int n;
    if(argc < 3)
    {
    	fprintf(stderr,"Usage:%s <src_file> <dest_file>\n",argv[0]);
    	exit(1);
    }

	fps = fopen(argv[1],"r");
	if(fps == NULL)
	{
		perror("fopen()");
		exit(1);
	}

	fpd = fopen(argv[2],"w");
	if(fpd == NULL)
	{
		fclose(fps);
		perror("fopen()");
		exit(1);
	}
	while((n = fread(buf, 1, BUFSIZE, fps) > 0))
	{
		//ch = fgetc(fps);
		//if(ch = EOF)
		//{

		//	break;
		//}
		//fputc(ch,fpd);
		fwrite(buf,1,n,fpd);
	}

	fclose(fps);
	fclose(fpd);
	exit(0);

}