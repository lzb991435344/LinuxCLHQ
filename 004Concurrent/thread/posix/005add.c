#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#define THRNUM 20
//echo 1 > /tmp/out ; cat /tmp/out
#define FNAME "/tmp/out"
#define LINESIZE 1024


static void* thr_add(void* p){
	FILE* fp;
	char linebuf[LINESIZE];
	fp = fopen(FNAME, "r+");
	if(fp == NULL){
		perror("fopen()");
		exit(1);
	}
	fgets(linebuf, LINESIZE, fp);
	fseek(fp, 0, SEEK_SET);
	sleep(1);
	fprintf(fp, "%d\n", atoi(linebuf) + 1);//full buff
	fclose(fp);

	pthread_exit(NULL);

}
int main(int argc, char* argv[]){

	int err,i;
	pthread_t tid[THRNUM];
	for(i = 0 ; i < THRNUM; ++i){
		err = pthread_create(tid + i, NULL, thr_add, NULL);
		if(err){
			fprintf(stderr,"Pthread_create():%s\n", strerror(err));
			exit(1);
		}
	}

	for(i = 0 ; i < THRNUM; ++i){
		pthread_join(tid[i], NULL);
	}
	exit(0);
}