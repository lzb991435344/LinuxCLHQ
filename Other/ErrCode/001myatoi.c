#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

int myatoi(char* str){
	if(str == NULL){
		return -1;
	}

	int Ret_Integer = 0;
	int Integer_sign = 1;

	while(isspace(*str)){
		str++;
	}

	if(*str == '-'){
		Integer_sign = -1;
	}
	if(*str == '-' || *str == '+'){
		str++;
	}


	while(*str >= '0' && *str <= '9'){
		Ret_Integer = Ret_Integer * 10 + *str - '0';
		str++;
	}

	Ret_Integer = Integer_sign * Ret_Integer;
	return Ret_Integer;

}



int main(){

	char a[] = "-100";
	char b[] = "456";
	int c = 0;
	
	//int my_atoi(char*);	
 
	c = myatoi(a) + myatoi(b);
	
	printf("atoi(a)=%d\n",myatoi(a));
	printf("atoi(b)=%d\n",myatoi(b));
	printf("c = %d\n",c);

	return 0;
}