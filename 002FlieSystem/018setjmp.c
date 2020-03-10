#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <unistd.h>
static jmp_buf save;
static void d(){

	printf("%s():begin\n", __FUNCTION__);
	printf("%s():jmp now!\n",__FUNCTION__);
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
static void a(){

	int ret;
	
	printf("%s():begin\n", __FUNCTION__);
	ret = setjmp(save);
	if(ret == 0){
		printf("%s():call b()\n", __FUNCTION__);
		b();
		printf("%s():return b()\n", __FUNCTION__);
	}else{
		printf("%s():jmpback here with code %d!\n", __FUNCTION__,ret);
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