#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){

	time_t end;
	int64_t count = 0;
    
    //测试 time + ./005timeCiru > tmp/out
    //time(NULL) 返回当前时间
    
    //get time in seconds
	end = time(NULL) + 5;
	while(time(NULL) <= end){
		count++;
	}
	printf("%lld\n",count);
	exit(0);
}

/**
time的打印 + 时间
real	0m5.128s
user	0m5.106s
sys	0m0.016s

677731538
*/