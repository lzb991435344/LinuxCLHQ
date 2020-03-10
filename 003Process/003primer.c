#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT 30000000
#define RIGHT 30000200


// ./003primer | wc -l  result is 18
// time  ./003primer

int main(){

	int i;
	int j;
	int mark;

	for(i = LEFT; i <= RIGHT; ++i){
		mark = 1;
		for(j = 2; j < i/2; ++j){
			if(i % j == 0){
				mark = 0;
				break;
			}
		}

		if(mark){
			printf("%d is a primer\n",i);
		}
	}
	

	exit(0);
}

