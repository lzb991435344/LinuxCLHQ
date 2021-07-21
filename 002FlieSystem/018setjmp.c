#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <unistd.h>
static jmp_buf save;

/*
C/C++提供了三个宏
__FUNCTION__,
_FILE_和
_LINE_
定位程序运行时的错误。
程序预编译时预编译器将用所在的函数名，
文件名和行号替换。当运行时错误产生后这三个宏分别能返回错误所在的函数，
所在的文件名和所在的行号
*/

//在函数d中设置longjmp
static void d(){

	printf("%s():begin\n", __FUNCTION__);
	printf("%s():jmp now!\n",__FUNCTION__);

	//longjmp
	longjmp(save, 6);
	printf("%s():end\n", __FUNCTION__);
}

static void c(){

	printf("%s():begin\n", __FUNCTION__);
	printf("%s():call d()\n", __FUNCTION__);

	d();
	printf("%s():return d()\n", __FUNCTION__);
	printf("%s():end\n", __FUNCTION__);

}


static void b(){

	printf("%s():begin\n", __FUNCTION__);
	printf("%s():call c()\n", __FUNCTION__);

	c();
	printf("%s():return c()\n", __FUNCTION__);
	printf("%s():end\n", __FUNCTION__);

}

//在a中设置跳转点
static void a(){

	int ret;
	
	printf("%s():begin\n", __FUNCTION__);
	ret = setjmp(save);
	//返回值为0时是直接返回
	//执行一次，返回两次
	if(ret == 0){ 
		printf("%s():call b()\n", __FUNCTION__);
		b();
		printf("%s():return b()\n", __FUNCTION__);
	}else{ //返回值为非0
		printf("%s():jmpback here with code %d!\n", __FUNCTION__, ret);
	}
	
	printf("%s():end\n", __FUNCTION__);

}

int main(){

	printf("%s():begin\n", __FUNCTION__);
	printf("%s():call a()\n", __FUNCTION__);

	a();
	printf("%s():return a()\n", __FUNCTION__);
	printf("%s():end\n", __FUNCTION__);
	exit(0);
}
/**
main():begin
main():call a()
a():begin
a():call b()
b():begin
b():call c()
c():begin
c():call d()
d():begin
d():jmp now!
a():jmpback here with code 6!
a():end
main():return a()
main():end


*/