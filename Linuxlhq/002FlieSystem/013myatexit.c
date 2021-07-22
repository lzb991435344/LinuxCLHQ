#include <stdlib.h>
#include <stdio.h>


static void f1(void){
	printf("f1() is working!\n");
}

static void f2(void){
	printf("f2() is working!\n");
}

static void f3(void){
	printf("f3() is working!\n");
}
int main(){

	puts("Begin!");
	//钩子函数调用时间：即将执行exit()之前调用
	//调用顺序和C++中的虚构函数调用顺序相同
	//先将函数压栈，先调用的最后退出
	atexit(f1);
	atexit(f2);
	atexit(f3);
	puts("End!");
	
   /**
	Begin!
	f3
	f2
	f1
	End!
   */

	// void exit(int status);
    // status & 0377    011 111 111  -2^8 ---+127(256个状态) 
	exit(0);
}