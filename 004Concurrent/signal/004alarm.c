#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(){

	/**alarm(10);
	alarm(1)
	alarm(5);*/

	//无法实现多个计时器的叠加
	/**alarm(10);
	alarm(5);
	alarm(1);*/
	//while(1);
	while(1){
		//pause等待一个信号
		pause();
	}
	//移植程序的时候不要使用sleep()
	//sleep-> alarm() + pause()
	//sleep -> nanosleep()
	exit(0);
}