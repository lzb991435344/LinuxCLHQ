#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//多线程并发实现筛质数

#define LEFT 30000000
#define RIGHT 30000200 
#define THRNUM (RIGHT - LEFT + 1)

//主调在被调之前，需要声明
static void* thr_primer(void*);

int main(int argc, char* argv[]){

	int i,j,mark;
    int err;
    //保存tid的值，在pthread_join中参数需要传入
    pthread_t tid[THRNUM];


    for(i = LEFT; i <= RIGHT; ++i){
    	//201个线程同时使用一块i的空间地址，刚创建结束的时候，i的值还没被取走
    	//后面的地址进行覆盖，地址传参产生竞争和冲突
    	//改成(void*)i,直接传变量地址，转成void*
    	//err = pthread_create(tid + (i - LEFT), NULL, thr_primer, &i);
    	//the same address will race 
    	err = pthread_create(tid + (i - LEFT), NULL, thr_primer, (void*)i);
    	if(err){
    		fprintf(stderr, "pthread_create() is %s\n",strerror(err));
    		exit(1);
    	}
    }

    //收尸
    for(i = LEFT; i <= RIGHT; ++i){
    	pthread_join(tid[i - LEFT], NULL);
    }
	exit(0);  
}
static void* thr_primer(void* p){
	int i,j;
	int mark = 1;

	//i = *(int*)p;
    //从pthread_create中传过来参数转换成int型
	i =(int)p;
	mark = 1;

	for(j = 2; i < i/2; ++j){
	    if(i % j == 0){
	    	mark = 0;
	    	break;
	    }
	}
	
	if(mark){
		printf("%d is a primer\n", i);
	}
	pthread_exit(NULL);
}

/**

201个线程同时计算质数

30000005 is a primer
30000006 is a primer
30000007 is a primer
30000004 is a primer
30000008 is a primer
30000009 is a primer
30000010 is a primer
30000003 is a primer
30000011 is a primer
30000012 is a primer
30000013 is a primer
30000014 is a primer
30000015 is a primer
30000002 is a primer
30000016 is a primer
30000017 is a primer
30000018 is a primer
30000019 is a primer
30000020 is a primer
30000021 is a primer
30000022 is a primer
30000023 is a primer
30000001 is a primer
30000024 is a primer
30000025 is a primer
30000026 is a primer
30000027 is a primer
30000028 is a primer
30000029 is a primer
30000030 is a primer
30000031 is a primer
30000032 is a primer
30000000 is a primer
30000033 is a primer
30000034 is a primer
30000035 is a primer
30000036 is a primer
30000037 is a primer
30000038 is a primer
30000039 is a primer
30000040 is a primer
30000041 is a primer
30000042 is a primer
30000043 is a primer
30000044 is a primer
30000045 is a primer
30000046 is a primer
30000047 is a primer
30000048 is a primer
30000049 is a primer
30000050 is a primer
30000051 is a primer
30000052 is a primer
30000053 is a primer
30000054 is a primer
30000055 is a primer
30000056 is a primer
30000057 is a primer
30000058 is a primer
30000059 is a primer
30000060 is a primer
30000061 is a primer
30000062 is a primer
30000063 is a primer
30000064 is a primer
30000065 is a primer
30000066 is a primer
30000067 is a primer
30000068 is a primer
30000069 is a primer
30000070 is a primer
30000071 is a primer
30000072 is a primer
30000073 is a primer
30000074 is a primer
30000075 is a primer
30000076 is a primer
30000077 is a primer
30000078 is a primer
30000079 is a primer
30000080 is a primer
30000081 is a primer
30000082 is a primer
30000083 is a primer
30000084 is a primer
30000085 is a primer
30000086 is a primer
30000087 is a primer
30000088 is a primer
30000089 is a primer
30000090 is a primer
30000091 is a primer
30000092 is a primer
30000093 is a primer
30000094 is a primer
30000095 is a primer
30000096 is a primer
30000097 is a primer
30000098 is a primer
30000099 is a primer
30000100 is a primer
30000101 is a primer
30000102 is a primer
30000103 is a primer
30000104 is a primer
30000105 is a primer
30000106 is a primer
30000107 is a primer
30000108 is a primer
30000109 is a primer
30000110 is a primer
30000111 is a primer
30000112 is a primer
30000113 is a primer
30000114 is a primer
30000115 is a primer
30000116 is a primer
30000117 is a primer
30000118 is a primer
30000119 is a primer
30000120 is a primer
30000121 is a primer
30000122 is a primer
30000123 is a primer
30000124 is a primer
30000125 is a primer
30000126 is a primer
30000127 is a primer
30000128 is a primer
30000129 is a primer
30000130 is a primer
30000131 is a primer
30000132 is a primer
30000133 is a primer
30000134 is a primer
30000135 is a primer
30000136 is a primer
30000137 is a primer
30000138 is a primer
30000139 is a primer
30000140 is a primer
30000141 is a primer
30000142 is a primer
30000143 is a primer
30000144 is a primer
30000145 is a primer
30000146 is a primer
30000147 is a primer
30000148 is a primer
30000149 is a primer
30000150 is a primer
30000151 is a primer
30000152 is a primer
30000153 is a primer
30000154 is a primer
30000155 is a primer
30000156 is a primer
30000157 is a primer
30000158 is a primer
30000159 is a primer
30000160 is a primer
30000161 is a primer
30000162 is a primer
30000163 is a primer
30000164 is a primer
30000165 is a primer
30000166 is a primer
30000167 is a primer
30000168 is a primer
30000169 is a primer
30000170 is a primer
30000171 is a primer
30000172 is a primer
30000173 is a primer
30000174 is a primer
30000175 is a primer
30000176 is a primer
30000177 is a primer
30000178 is a primer
30000179 is a primer
30000180 is a primer
30000181 is a primer
30000182 is a primer
30000183 is a primer
30000184 is a primer
30000185 is a primer
30000186 is a primer
30000187 is a primer
30000188 is a primer
30000189 is a primer
30000190 is a primer
30000191 is a primer
30000192 is a primer
30000193 is a primer
30000194 is a primer
30000195 is a primer
30000196 is a primer
30000197 is a primer
30000198 is a primer
30000199 is a primer
30000200 is a primer



*/