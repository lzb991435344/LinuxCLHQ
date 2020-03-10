#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage:%s <src_file>",argv[0]);
		exit(1);
	}

    FILE* fp;
    /**初始化空間*/
    char* linebuf = NULL;
    size_t linesize = 0;

    fp = fopen(argv[1], "r");
    if(NULL = fp)//修改，产生core文件
    {
    	perror("fopen()");
    	exit(1);
    }
    while(1)
    {
        //参数1：二级指针
        //参数2：size_t类型的指针
        //参数3：一个流
    	if(getline(&linebuf, &linesize, fp) < 0)
    	{
    		break;
    	}
    	printf("%d\n",strlen(linebuf));
    	printf("%d\n",linesize);
    }

    fclose(fp);
	exit(0);
}