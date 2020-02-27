#include <stdio.h>
#include <stdlib.h>



int main(int argc, char* argv[])
{

	//char str[] = "123a456";
	//printf("%d\n",atoi(str));
	char buf[1024];
	int year = 2019,month = 8,day = 22;
	//printf("%d-%d-%d\n",year,month,day);
	sprintf(buf,"%d-%d-%d",year,month,day);
	puts(buf);

	exit(0);
}

