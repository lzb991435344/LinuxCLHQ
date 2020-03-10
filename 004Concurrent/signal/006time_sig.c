#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//用volatile的原因？
// volatile的本意是“易变的” 因为访问
//寄存器要比访问内存单元快的多,所以编译器一般都
//会作减少存取内存的优化，但有可能会读脏数据。当要求使
//用volatile声明变量值的时候，系统总是重新从它所在的内存读取
//数据，即使它前面的指令刚刚从该处读取过数据。
static volatile int loop = 1;
static void alarm_handler(int s){
	loop = 0;
}
int main(){

	int64_t count = 0 ;

	//alarm(5);
	//.......
	signal(SIGALRM,alarm_handler);
    alarm(5);
	while(loop){
		count++;
	}
	printf("%lld\n",count);
	exit(0);
}

/**
2839411325

real	0m5.001s
user	0m4.994s
sys	0m0.000s

*/