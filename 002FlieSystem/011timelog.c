
/** timelog.c
1 2014-01-31 10:30:56
2 2014-01-31 10:31:56
3 2014-02-02 10:28:50
*/

//tail -f /tmp/out
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>



#define FNAME "/tmp/out"
#define BUFSIZE 1024


int main(){
	FILE* fp;
	char buf[BUFSIZE];
	int count = 0;
	time_t stamp;
	struct tm* tm;
	fp = fopen(FNAME, "a+");
	if(fp == NULL){
		perror("fopen()");
		exit(1);
	}

	while(fgets(buf, BUFSIZE, fp) != NULL){
		count++;
	}

	while(1){
		time(&stamp);
		tm = localtime(&stamp);
		fprintf(fp, "%-4d%d-%d-%d %d:%d:%d\n", ++count,\
			tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,\
			tm->tm_hour, tm->tm_min, tm->tm_sec);
		
		//why fflush()
		fflush(fp);
		sleep(1);
	}

	//cannot run here
	fclose(fp);
	exit(0);
}

