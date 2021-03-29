#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#define THRNUM 20
//echo 1 > /tmp/out ; cat /tmp/out
#define FNAME "/tmp/out"
#define LINESIZE 1024

//线程竞争代码示例
// 20个线程同时打开文件进行操作，读取数据，读取到数据之后进行加1，写回数据


static void* thr_add(void* p){
	FILE* fp;
	char linebuf[LINESIZE];
	//必须保证文件存在，而且可读可写
	fp = fopen(FNAME, "r+");
	if(fp == NULL){
		perror("fopen()");
		exit(1);
	}
	//确保读出来一行
	fgets(linebuf, LINESIZE, fp);

	//定位文件的位置，定位到文件首
	fseek(fp, 0, SEEK_SET);

	// echo 1 > /tmp/out
	//sleep(1)  20个线程都拿到了1值,都在sleep(1),覆盖写了20次2的值
	//线程同步解决问题---互斥量，资源发生竞争和冲突，锁住
	sleep(1);
	//转换成整型数atoi()
	fprintf(fp, "%d\n", atoi(linebuf) + 1);//full buff
	fclose(fp);

	pthread_exit(NULL);

}
int main(int argc, char* argv[]){

	int err,i;
	pthread_t tid[THRNUM];
	for(i = 0 ; i < THRNUM; ++i){
		//创建20个线程
		err = pthread_create(tid + i, NULL, thr_add, NULL);
		if(err){
			fprintf(stderr,"Pthread_create():%s\n", strerror(err));
			exit(1);
		}
	}

	//回收资源
	for(i = 0 ; i < THRNUM; ++i){
		pthread_join(tid[i], NULL);
	}
	exit(0);
}

/**
2
*/
//结果为2,20个线程同时写了2