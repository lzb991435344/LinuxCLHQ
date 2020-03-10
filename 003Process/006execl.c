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

//  ./006execl > /tmp/out
/**
1583830788
*/

//fflush