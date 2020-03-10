#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	printf("argc is %d\n",argc);

	int i;
	for(i = 0; argv[i] != NULL; ++i){
		puts(argv[i]);
	}
	exit(0);
}