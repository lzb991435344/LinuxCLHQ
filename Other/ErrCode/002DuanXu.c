#include <stdio.h>
#include <iostream>

//int--char的强制转换，是将低地址的数值截断赋给char
void isBigOrSmall(int data){
	
	char ch = static_cast<char>(data);
	if(ch == 0x12){//低地址存高字节数据
		printf("big endian");
	}else if( ch == 0x34){
		printf("little endian");
	}
}


union Endian{
	int a;
	char ch;
};

void isBigOrSmall2(Endian en){
	if(en.ch == 0x12){
		printf("big endian");
	}else if(en.ch == 0x34){
		printf("little endian");
	}

}


//g++ 002DuanXu.c -o 002DuanXu
int main(){

	int data = 0x1234;

	Endian en;
	en.a = 0x1234;

	isBigOrSmall(data);
	isBigOrSmall2(en);
	return 0;
}