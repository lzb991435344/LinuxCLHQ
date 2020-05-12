
/** timelog.c
1 2014-01-31 10:30:56
2 2014-01-31 10:31:56
3 2014-02-02 10:28:50
*/

//tail -f /tmp/out  //写文件的时候注意刷新缓冲区
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
		time(&stamp);//从内核中取时间
		tm = localtime(&stamp);
		//\n已经起不到刷新缓冲区的作用
		//年份从1900年开始计算，加上1900
		//月份范围是0-11，计算的时候加上1
		fprintf(fp, "%-4d%d-%d-%d %d:%d:%d\n", ++count,\
			tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,\
			tm->tm_hour, tm->tm_min, tm->tm_sec);
		
		//why fflush()
		fflush(fp);//刷新流，全缓冲
		sleep(1);
	}

	//cannot run here
	fclose(fp);
	exit(0);
}

