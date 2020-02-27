#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{

   /*
   printf("before while\n");
   while(1);
   printf("before while\n");
   */
	printf("before while");
	fflush(stdout);
	while(1);
  printf("before while");
  fflush(NULL);
  exit(0);
}