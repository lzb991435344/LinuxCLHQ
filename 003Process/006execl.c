#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


/**
date +%s  print timestamp
1583830496
*/
int main(){

	puts("begin!");

	fflush(NULL);
	execl("/bin/date", "date", "+%s", NULL);

	perror("execl()");
	exit(1);

	puts("end!");
	exit(0);
}
// ./006execl
/**
begin!
1583830788
*/

//只打印时间，未进行流刷新
//  ./006execl > /tmp/out
/**
1583830788
*/

//fflush

//以上问题  进程的pid是一样的