#include <stdio.h>
#include <iostream>
#include <assert.h>
using namespace std;

//01
void test1() {
	
	//字符串1需要11个字节才能放下，还有‘\0’未考虑在内
	char str[10];
	const char* str1 = "0123456789";
	strcpy_s(str, str1);//将结束字符也拷贝进去，容易溢出
}

//02
void test2() {
	char string[10], str1[10];
	int i;
	for (i = 0; i < 10; i++)
	{
		//str1 = 'a'; //error,数组名str1为 char *const类型的右值类型，根本不能赋值。
		*str1 = 'a'; //ok
	}
	//strcpy会从源地址一直往后拷贝，直到遇到'\0'为止。所以拷贝的长度是不定的。
	//如果一直没有遇到'\0'导致越界访问非法内存，程序就崩了。
	strcpy(string, str1);
}

void test2_fix(){
	char string[10], str1[10];
	int i;
	for (i = 0; i < 9; i++)
	{
		str1[i] = 'a';
	}
	str1[9] = '\0';
	strcpy(string, str1);
}

//03
void test3(char* str1)
{
	if (str1 == NULL) {
		return;
	}
	char string[10];
	//if(strlen(str1) <= 10)应改为if(strlen(str1) < 10)，
	//因为strlen的结果未统计’\0’所占用的1个字节。
	if (strlen(str1) <= 10) 
	{
		strcpy(string, str1);//实际可拷贝的长度是size - 1
	}
}

//04
void strcpy_01(char *strDest, char *strSrc)
{
	while ((*strDest++ = *strSrc++) != '\0');
}
/**优先级问题
*P++可看成*（P++），即先执行p++，后执行*p
（--P++）等价于道 --（P++）实际执行效果等价于 先 p=p-1 后 p=p+1等价于（--p）++
*/


void strcpy_02(char *strDest, const char *strSrc)
//将源字符串加const，表明其为输入参数，加2分
{
	while ((*strDest++ = *strSrc++) != '\0');
}

void strcpy_03(char *strDest, const char *strSrc)
{
	//对源地址和目的地址加非0断言，加3分
	assert((strDest != NULL) && (strSrc != NULL));
	while ((*strDest++ = *strSrc++) != '\0');
}
char * strcpy_04(char *strDest, const char *strSrc)
{
	assert((strDest != NULL) && (strSrc != NULL));
	char *address = strDest;
	while ((*strDest++ = *strSrc++) != '\0');
	return address;
}

//05
//1：传入形参并不能真正改变形参的值，执行完之后为空；
//2：在函数GetMemory中和Test中没有malloc对应的free，造成内存泄露
void GetMemory(char *p)
{
	p = (char *)malloc(100);
	//free() ？？？
}
void Test(void)
{
	char *str = NULL;
	//在函数内部修改形参并不能真正的改变传入形参的实参值
	//str仍然为NULL；
	GetMemory(str);
	strcpy(str, "hello world");
	printf(str);
}

//06 变量生存周期
char *GetMemory(void)
{
	char p[] = "hello world";
	//p[]数组为函数内的局部自动变量，
	//在函数返回后，内存已经被释放
	return p;
}
void Test(void)
{
	char *str = NULL;
	str = GetMemory();
	printf(str);
}


//007
//1. 传入GetMemory的参数为字符串指针的指针，但是在GetMemory中执行申请内存及赋值语句
//后未判断内存是否申请成功及num判断
//2. 未释放堆内存 动态分配的内存在程序结束之前没有释放，应该调用free, 把malloc生成的内存释放掉
//3. printf(str) 改为 printf("%s", str), 否则可使用格式化 字符
void GetMemory(char **p, int num)
{
	*p = (char *)malloc(num);
}
void Test(void)
{
	char *str = NULL;
	GetMemory(&str, 100);
	strcpy(str, "hello");
	printf(str);
}


//008
//1.申请内存成功还是失败未判断
//2.free完成之后设置为 str = NULL;
void Test(void)
{
	char *str = (char *)malloc(100);
	strcpy(str, "hello");
	free(str);
	//... //省略的其它语句
}

//009
//1.在swap函数中，p是一个“野”指针，有可能指向系统区，导致程序运行的崩溃
void swap(int* p1, int* p2){
	int *p;
	*p = *p1;
	*p1 = *p2;
	*p2 = *p;
}

void swap_fix(int* p1, int* p2) {
	int p;//一块内存空间
	p = *p1;
	*p1 = *p2;
	*p2 = p;
}

//010
//分别给出BOOL，int，float，指针变量 与“零值”比较的 if 语句（假设变量名为var）
/**
BOOL型变量：if(!var)
int型变量： if (var == 0)
float型变量：
const float EPSINON = 0.00001;
if ((x >= -EPSINON) && (x <= EPSINON)
指针变量：　　if(var == NULL)
*/

//011
/**
void Func(char str[100])
{
	sizeof(str) = ?
}
void *p = malloc(100);
sizeof(p) = ?

sizeof(str) = 4
sizeof(p) = 4
	Func(char str[100])函数中数组名作为函数形参时，在函数体内，数组名失去了本身的内涵，
仅仅只是一个指针；在失去其内涵的同时，它还失去了其常量特性，可以作自增、自减等操作，
可以被修改。
数组名的本质如下：
（1）数组名指代一种数据结构，这种数据结构就是数组；
例如：
	char str[10];
	cout ＜＜ sizeof(str) ＜＜ endl;
	//输出结果为10，str指代数据结构char[10]。
（2）数组名可以转换为指向其指代实体的指针，而且是一个指针常量，不能作自增、自减等操作，
不能被修改；
	char str[10];
	str++; //编译出错，提示str不是左值 
（3）数组名作为函数形参时，沦为普通指针。
	Windows NT 32位平台下，指针的长度（占用内存的大小）为4字节，故sizeof(str) 、sizeof(p) 
都为4。
*/


//012
/**
写一个“标准”宏MIN，这个宏输入两个参数并返回较小的一个。另外，当你写下面的代码时会发生什么事？
least = MIN(*p++, b);
#define MIN(A,B) ((A) <= (B) ? (A) : (B))   
（1）谨慎地将宏定义中的“参数”和整个宏用用括弧括起来
	#define MIN(A,B) (A) <= (B) ? (A) : (B)
	#define MIN(A,B) (A <= B ? A : B )
（2）防止宏的副作用
	宏定义#define MIN(A, B) ((A) <= (B) ? (A) : (B))对MIN(*p++, b)的作用结果是：
	((*p++) <= (b) ? (*p++) : (b))
  这个表达式会产生副作用，指针p会作2次++自增操作。
*/

//013
/**
#ifndef　__INCvxWorksh
#define　__INCvxWorksh
#endif 
//防止被重复引用

void foo(int x, int y);
C:C编译器编译后在symbol库中的名字为_foo,
C++:C++编译器则会产生像_foo_int_int之类的名字。_foo_int_int这样的名字包含了函数名和函数参数数量及类型信息
，C++就是靠这种机制来实现函数重载的

C++提供了C连接交换指定符号extern "C"来解决名字匹配问题，函数声明前加上extern "C"后，则编译器就会按照C语言的方
式将该函数编译为_foo，这样C语言中就可以调用C++的函数了。
*/

//014
/**
编写一个函数，作用是把一个char组成的字符串循环右移n个。比如原来是“abcdefghi”如果n = 2，
移位后应该是“hiabcdefg”*/

#define MAXSIZE 256
void LoopMove(char *str, int steps)
{
	int len = strlen(str);
	char tmp[MAXSIZE];
	strcpy(tmp, str + len - steps);
	strcpy(tmp + steps, str);
	*(tmp + len) = '/0';
	strcpy(str, tmp);
}
void LoopMove(char *str, int steps)
{
	int len = strlen(str);
	char tmp[MAXSIZE];
	memcpy(tmp, str + len - steps, steps);
	memcpy(str + steps, str, len - steps);
	memcpy(str, tmp, len);
}

//015
//编写类String的构造函数、析构函数和赋值函数，已知类String的原型为：
class String
{
public:
	String(const char *str = NULL); // 普通构造函数 
	String(const String &other); // 拷贝构造函数 
	~String(void); // 析构函数 
	String & operator =(const String &other); // 赋值函数 
private:
	char *m_data; // 用于保存字符串 
};


//普通构造函数
String::String(const char *str)
{
	if (str == NULL)
	{
		m_data = new char[1]; // 得分点：对空字符串自动申请存放结束标志'\0'的空
		//加分点：对m_data加NULL 判断
		*m_data = '\0';
	}
	else
	{
		int length = strlen(str);
		m_data = new char[length + 1];
		strcpy(m_data, str);
	}
}
// String的析构函数
String::~String(void)
{
	delete[] m_data; // 或delete m_data;
}
//拷贝构造函数
String::String(const String &other) // 得分点：输入参数为const型
{
	int length = strlen(other.m_data);
	m_data = new char[length + 1];
	strcpy(m_data, other.m_data);
}
//赋值函数
String & String::operator =(const String &other) // 得分点：输入参数为const型
{
	if (this == &other) //得分点：检查自赋值
		return *this;
	delete[] m_data;//得分点：释放原有的内存资源
	int length = strlen(other.m_data);
	m_data = new char[length + 1];
	strcpy(m_data, other.m_data);
	return *this; //得分点：返回本对象的引用
}

//016
//请说出static和const关键字尽可能多的作用

/**
static关键字至少有下列n个作用：
（1）函数体内static变量的作用范围为该函数体，不同于auto变量，该变量的内存只被分配一次，
    因此其值在下次调用时仍维持上次的值；
（2）在模块内的static全局变量可以被模块内所用函数访问，但不能被模块外其它函数访问；
（3）在模块内的static函数只可被这一模块内的其它函数调用，这个函数的使用范围被限制在声明它的模块内；
（4）在类中的static成员变量属于整个类所拥有，对类的所有对象只有一份拷贝；
（5）在类中的static成员函数属于整个类所拥有，这个函数不接收this指针，因而只能访问类的static成员变量。
const关键字至少有下列n个作用：
（1）欲阻止一个变量被改变，可以使用const关键字。在定义该const变量时，通常需要对它进行初始化，因为以后就没有机会再去改变它了；
（2）对指针来说，可以指定指针本身为const，也可以指定指针所指的数据为const，或二者同时指定为const；
（3）在一个函数声明中，const可以修饰形参，表明它是一个输入参数，在函数内部不能改变其值；
（4）对于类的成员函数，若指定其为const类型，则表明其是一个常函数，不能修改类的 成员变量；
（5）对于类的成员函数，有时候必须指定其返回值为const类型，以使得其返回值不为“左值”。例如：
	const classA operator*(const classA& a1,const classA& a2);
	operator*的返回结果必须是一个const对象。如果不是，这样的变态代码也不会编译出错：
	classA a, b, c;
	(a * b) = c; // 对a*b的结果赋值
	操作(a * b) = c显然不符合编程者的初衷，也没有任何意义

*/


//017
//写一个C函数，若处理器是Big_endian的，则返回0；若是Little_endian的，则返回1
/**
//采用Little-endian模式的CPU对操作数的存放方式是从低字节到高字节(升序)，
//Big-endian模式对操作数的存放方式是从高字节到低字节(降序)
	32bit宽的数0x12345678
	Little-endian:
	0x4000 0x78
	0x4001 0x56
	0x4002 0x34
	0x4003 0x12
	Big-endian:
	0x4000 0x78
	0x4001 0x56
	0x4002 0x34
	0x4003 0x12
*/

//联合体union的存放顺序是所有成员都从低地址开始存放,轻松地获得了CPU对内存
//采用Little-endian还是Big-endian模式读写
int checkCPU(){
	
	{
		union w
		{
			int a;
			char b;
		} c;
		c.a = 1;
		return (c.b == 1);
	}
}


//其实本质还是取低8位数来判断。分析同上：a = 1, 变成二进制为：0000 0001.
//强转为char时，保留unsigned short a变量的低8位。
//因为1低位数据，如果char c不为0，表明1存放在低位，即是小端模式；如果char c变量
//为0，表明1存放在高位，即是大端模式
void checkCpu02() {
	unsigned short a = 1;
	char c = (char)a;
	(c) ? printf("Little\n") : printf("Big\n");
}

#define BigOrLittle(n) ((n) >> 8)?printf("Big\n"):printf("Little\n")
void checkCpu03() {
	unsigned short a = 1;
	BigOrLittle(a);
}
/**地址：高地址 <<= ==== = 低地址

小端模式存放：0000 0001 >> 8 == 0000 0000

大端模式存放：1000 0000 >> 8 == 0000 1000
*/



//018
//C++中四种类型转换是：static_cast, dynamic_cast, const_cast, reinterpret_cast
/**
1、const_cast
	用于将const变量转为非const
2、static_cast
	用于各种隐式转换，比如非const转const，void*转指针等, static_cast能用于多态向上转化，如果向下转能成功但是不安全，结果未知；
3、dynamic_cast
	用于动态类型转换。只能用于含有虚函数的类，用于类层次间的向上和向下转化。只能转指针或引用。向下转化时，如果是非法的对于指针返回NULL，对于引用抛异常。要深入了解内部转换的原理。
	向上转换：指的是子类向基类的转换
	向下转换：指的是基类向子类的转换
	它通过判断在执行到该语句的时候变量的运行时类型和要转换的类型是否相同来判断是否能够进行向下转换。
4、reinterpret_cast
	几乎什么都可以转，比如将int转指针，可能会出问题，尽量少用；
5、为什么不使用C的强制转换？
	C的强制转换表面上看起来功能强大什么都能转，但是转化不够明确，不能进行错误检查，容易出错
*/


//019
// C/C++ 中指针和引用的区别？
/**
1.指针有自己的一块空间，而引用只是一个别名；
2.使用sizeof看一个指针的大小是4，而引用则是被引用对象的大小；
3.指针可以被初始化为NULL，而引用必须被初始化且必须是一个已有对象 的引用；
4.作为参数传递时，指针需要被解引用才可以对对象进行操作，而直接对引 用的修改都会改变引用所指向的对象；
5.可以有const指针，但是没有const引用；
6.指针在使用中可以指向其它对象，但是引用只能是一个对象的引用，不能被改变；
7.指针可以有多级指针（**p），而引用至于一级；
8.指针和引用使用++运算符的意义不一样；
9.如果返回动态内存分配的对象或者内存，必须使用指针，引用可能引起内存泄露。

*/

//020
//给定三角形ABC和一点P(x,y,z)，判断点P是否在ABC内
//根据面积法，如果P在三角形ABC内，那么三角形ABP的面积+三角形BCP的面积+三角形ACP的面积应该等于三角形ABC的面积
//浮点数比较大小
#define ABS_FLOAT_0 0.0001
struct point_float
{
	float x;
	float y;
};
float GetTriangleSquar(const point_float pt0, const point_float pt1, const point_float pt2)
{
	point_float AB, BC;
	AB.x = pt1.x - pt0.x;
	AB.y = pt1.y - pt0.y;
	BC.x = pt2.x - pt1.x;
	BC.y = pt2.y - pt1.y;
	return fabs((AB.x * BC.y - AB.y * BC.x)) / 2.0f;
}
bool IsInTriangle(const point_float A, const point_float B, const point_float C, const point_float D)
{
	float SABC, SADB, SBDC, SADC;
	SABC = GetTriangleSquar(A, B, C);
	SADB = GetTriangleSquar(A, D, B);
	SBDC = GetTriangleSquar(B, D, C);
	SADC = GetTriangleSquar(A, D, C);
	float SumSuqar = SADB + SBDC + SADC;
	if ((-ABS_FLOAT_0 < (SABC - SumSuqar)) && ((SABC - SumSuqar) < ABS_FLOAT_0))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//021
//怎么判断一个数是二的倍数，怎么求一个数中有几个1

int fun(long x)
{
	int _count = 0;
	while (x)
	{
		if (x % 10 == 1)
			++_count;
		x /= 10;
	}
	return _count;
}
int main()
{
	cout << fun(123321) << endl;
	return 0;
}

//022  2020.04.16
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

/**
static：
(1)类的静态成员属于整个类 而不是某个对象，可以被类的所有方法访问，
	子类当然可以访问父类静态成员；
(2)静态方法属于整个类，在对象创建之前就已经分配空间，类的非静态成员要
	在对象创建后才有内存，
  所有静态方法只能访问静态成员，不能访问非静态成员；
(3)静态成员可以被任一对象修改，修改后的值可以被所有对象共享。
(4)静态成员在对象创建之前就已经分配空间，类的非静态成员要在对象创建后才有内存。
  静态成员（方法）属于整个类，而不属于任何单个对象。
(5)静态成员无多态特性
 多态性是要通过指针或者引用才能体现出来的。但是，如果有引用或者指针的话，那代表着什么？
说明，你必须申明对象。而我们知道静态成员函数是可以通过类直接访问的
*/

class A
{
	static int a;
public:
	//只有非静态成员函数可以使虚拟的
	/*virtual static void print()
	{
		cout << a << endl;
	}*/
};
int A::a = 1;
class B :public A
{
	static int b;
public:
	static void print()
	{
		cout << b << endl;
	}
};
int B::b = 2;
int main(void)
{
	A* aa = new B;
	B* b = new B;
	b->print();//2
	//B bb;
	//cout << sizeof(*aa) << endl;   //答案为1，说明不存在vptr来指向一个虚表，下同
	//cout << sizeof(bb) << endl;
	//aa->print();
	
	getchar();
	system("pause");
	return 0;
}

//023 参数转换
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

void test(int a)
{
}
void test(float a)
{
}
int main(void)
{
	test(1);
	test('c');
	test(2+'c');
	/**
	编译器会提示参数不匹配，但是不匹配的原因是：类型转换只能从下往上自动转换，
	而不能自动从上往下转换。
    0.5默认是double类型，既不能转换成float类型，也不能转换成int类型。
	故会提示参数不匹配
	*/
	//test(0.5);
	getchar();
	system("pause");
	return 0;
}

//024 大小端模式
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

/**
  (1)首先要知道大小端模式，80X86下是小端模式；
  当然可以编写下测试就可以了，short占2个字节，设左高地址，右低地址；
  a[1]           a[0]
  1111 1111     0000 0000
  short占用的是这a[1]、a[0]两个字节，最高位是1是一个负数，在计算机中采用补码表示，
  那么二进制表示为：1000 0001 0000 0000，转化为十进制就是-256.
  (2)char为8位，赋值时直接取低8位即可。
  a[0] = 0000 0000//低01
  a[1] = 1111 1111 //低02
  a[2] = 1111 1110 //高01
  a[3] = 1111 1101 //高02
  short为16位，从a的首地址开始前16位为b。
  80X86架构 小端寻址（低位低地址，高位高地址），故 b = 1111 1111 0000 0000 。
  符号位为1，因此b为负数。
  负数原码 = 取反(补码-1)=~（ 111 1111 0000 0000 - 1 ）= 000 0001 0000 0000=256。
  故b=-256。
*/
union Test
{
	char a[4];
	short b;//占据两个字节,a[0],a[1]
};

int main(void)
{
	Test test;
    test.a[0] = 256;
    test.a[1] = 255;
    test.a[2] = 254;
    test.a[3] = 253;
    printf("%d\n", test.b);//-256
	getchar();
	system("pause");
	return 0;
}

//025 二进制中0和1的个数
#include<stdio.h>
#include <iostream>

using namespace std;

template<class T>
T fun(T x, T y)//类型一致
{
	return x * x + y * y;
}

int numberofOne(unsigned int x)
{
	int count = 0;
	while (x)
	{
		count++;
		x = x & (x - 1);
	}
	return count;
}

int numberofZero(unsigned int x)
{
	int count = 0;
	while (x + 1)
	{
		count++;
		x = x | (x + 1);
	}
	return count;
}

//求二进制数中1的个数
int f(unsigned int n)
{
	unsigned int m = 0;
	for (m = 0 ; n ; m++)
	{
		n &= (n-1);
	}
	return m;
}
int main(void)
{
	/**int a = 3;
	a += a -= a * a;//9，-6，-12
	printf("%d\n",a);//-12
	*/
	/**/
	
	/**fun(1,2);
	//fun(1.0, 2); error类型不一致
	fun(2.0, 1.0);
	fun<float>(1, 2.0);*/
	//cout<< f(2486)<<endl; //7
	int m = 1;
	int n = 2;
	cout << numberofOne(m)<<endl;
	cout << numberofOne(n) << endl;

	cout << numberofZero(n) << endl;
	cout << numberofZero(n) << endl;
	getchar();
	system("pause");
	return 0;

}

//026访问控制符测试

// demoTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

class A {
	int a1;
protected:
	int a2;
public:
	int a3;
};
class B : public A {
	int b1;
protected:
	int b2;
public:
	int b3;
};
class C :private B {
	int c1;
protected:
	int c2;
public:
	int c3;
};
class D :protected B
{
	int d1;
protected:
	int d2;
public:
	int d3;

};
int main()
{
	A oaa;
	B obb;
	C occ;
	D odd;
	//oaa.a2 = 2;error
	//oaa.a1 = 1;error
    oaa.a3 = 1;

	//obb.a1 = 1;error,a1是A私有的，不能访问
	//obb.a2 = 2;error，a2是A私有的，不能访问
	obb.a3 = 1;

	//obb.b1 = 0;error
	//obb.b2 = 0;error;
	obb.b3 = 2;

	//无法访问
	//occ.a1 = 1;
	//occ.a2 = 1;
	//occ.a3 = 1;
	//occ.b1 = 1;
	//occ.b2 = 1;
	//occ.b3 = 1;
	//occ.c1 = 1;
	//occ.c2 = 1;
	occ.c3 = 1;//可以访问

	//odd.d1 = 1;
	//odd.d2 = 1;
	odd.d3 = 1;
	//odd.b1 = 1;
	//odd.b3 = 1;
	getchar();
	system("pause");
}


//027 函数参数入栈顺序
#include "pch.h"
#include <iostream>
using namespace std;
int* fun(int a, int b)
{
	int k = a + b;
	return &k;//返回局部变量或临时变量的地址
	//main种调用 fun(2,3)返回5
}

//考察函数入栈顺序的问题
void fun2(const int& a, const int& b)
{
	cout << a;
	cout << b;
}
int main()
{
	int i = 0;
	/**
	C语言函数参数入栈顺序为由右向左,func(++i, i++)可以分解为
    参数 i 先入栈 输出0 
    然后执行i++ 此时i为1 
    接着参数 i 先执行++i   i 此时为2  后入栈进行输出 2
	*/
	fun2(++i,i++);// 2,0
	getchar();
	system("pause");
}

//028 继承和虚函数
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

class A
{
public:
	A()
	{
		this->foo();
	}

	virtual void foo()
	{
		cout<<"A::foo()"<<endl;
	}

};

class B :public A
{
public:
	B()
	{
		this->foo();
	}
	
    virtual void foo()
	{
		cout<< "B::foo()"<<endl;
	}
};

int main(void)
{
	
	/** B bb; 
	结果：A::foo()
	      B::foo()*/
	A* aa = new  B;
	/** A* aa = new  B;
	结果：A::foo()
	      B::foo()
	*/

	/**A* aa = new  B;
	aa->foo();
	
	结果是：
	A::foo()
	B::foo()
	B::foo()
	*/
	aa->foo();
	free(aa);
	getchar();
	system("pause");
	return 0;
}

//029 结构体使用
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;
struct str_t {
	long long len;
	char data[32];
};
struct data1_t {
	long long len;
	int data[2];
};
struct data2_t {
	long long len;
	char *data[1];
};
struct data3_t {
	long long len;
	void *data[];
};
int main(void)
{
	struct str_t str;
	memset((void*)&str, 0, sizeof(struct str_t));
	str.len = sizeof(struct str_t) - sizeof(int);
	snprintf(str.data, str.len, "hello");//VS下为_snprintf
	
	
	//关键是找到h的首地址
    /**03
	  pData->data[0]的类型是void*,值为h
	  &(pData->data[0]))的类型是void**,值是h的地址
	*/
	//struct data3_t *pData = (struct data3_t*)&str; 
	//printf("data:%s%s\n", str.data, (char*)(&(pData->data[0])));
	

	/**02
	  pData->data[0]的类型是void*,值为h
	  此时输出的是不知名的内容，没有权限访问报段地址错误
	*/
	struct data2_t *pData = (struct data2_t*)&str; 
	printf("data:%s%s\n", str.data, (char*)(pData->data[0]));
	
	/**01
	pData->data的类型是int*,数组名被看做是元素的首地址，这里是8，强制转换之后没有错误
	*/
	//struct data1_t *pData = (struct data1_t*)&str; 
	//printf("data:%s%s\n", str.data, (char*)(pData->data));

	/**00
	理由同上
	*/
	//struct str_t *pData = (struct str_t*)&str;
	//printf("data:%s%s\n", str.data, (char*)(pData->data));


	getchar();
	system("pause");
	return 0;
}

//030 结构体和指针数组
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

//64位系统下
struct st_t
{
	int status; //8
	short* pdata;//8
	char errstr[32];//32
};
void func( char** m)
{
	++m;//指向afternoon
	cout << *m << endl;//提领值是指打印字符串
}
int main(void)
{
   /** static char* a[] = {"morning","afternoon","evening"};
	char** p;
	//p = a;
	func(p); //afternoon
	解析：双指针就是二维数组，始终指向每行的首元素，
	p + 1是指二维数组加一行
	*p + 1是指p指向行数的下一个元素
	*/
	
	/**bool first = true;
	int sum = 0;
	int value;
	unsigned short i = 0xFFFF;
	for (; i >= 0 ; --i)
	{
		if (first)
		{
			value = 65536;
			sum += value % 3;
			first = false;
		}
		else
		{
			sum += --value % 3;
			if (value <= 0)
			{
				cout << sum << "," << i; //65536,65535
				break;
			}
		}
	}	
	*/
	
	/**st_t st[16];
	char* p = (char*)(st[2].errstr + 32);//实际上指向st[3]
	printf("%d",(p-(char*)(st)));//&st[3] - &st[0],结果144
	*/
	getchar();
	system("pause");
	return 0;

}



//031 结构体指针和字符串比较
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

struct st
{
	int n;
	struct st* next;
};
static struct st a[3] = {5, &a[1], 7, &a[2], 9, 0},*p;
//比较两个字符串是否相等
int sss(char s[], char t[])
{
	int i = 0;
	while (s[i] && t[i] && t[i] == s[i])
	{
		i++;
	}
	return (s[i]-t[i]);
}
int main(void)
{
	/*int c;
	c = 13 | 5;
	printf("%d\n",c); //13
	*/
	
	/**int k = 1000;
	while (k > 1)
	{
		printf("%d\n",k); //k循环9次
		k = k / 2;
	}*/
	//p = &a[0];
	//cout<< p++->n <<endl;//5
	//cout<< ++p->n<<endl;//6
	//cout << p->n++ << endl;//5
	//cout << (*p).n++ << endl; //5
	char s[] = "hello";
	char t[] = "hello";
	cout<< sss(s,t)<<endl;
	getchar();
	system("pause");
	return 0;
}


//032 指针数组及函数参数传递
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

//x86架构下参数传递顺序
int f(int a, int b, int c)
{
	return 0;
}

//求质因数
void prim(int m, int n)
{
	if (m > n)
	{
		while (m % n)
		{
			n++;
		}
		m /= n;
		prim(m,n);
		cout<< n <<endl;
	}
}
int main(void)
{
	/**int b = 10;
	int a = -11;
	a %= b %= 4;
	cout<< "a="<<a<<" "<<"b="<<b<<endl; //-1,2
	//先计算b%4,再计算 a%2
	*/

	/**int a[4] = {1,2,3,4};
	int* ptr = (int*)(&a + 1);
	printf("%d",*(ptr-1)); //4  
	
	解析：a和&a都是数组首元素的地址
	      a是a+0,*(a+0)就是a[0]
		  &a + 1是a[]的数组类型的指针加1，此时到达数组的末尾
		  ptr指针是int*型，-1代表回退4个字节
	*/


	/*
	x86架构下的参数是通过栈传递的，从右往左依次入栈，结果是cba
	x86_64架构下参数是通过寄存器传递，结果是abc
	*/
	//return f(printf("a"), printf("b"), printf("c")); //c b a

	prim(6, 2);//3,2
	getchar();
	system("pause");
	return 0;
}

//033 指针数组和类

#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

class test
{
	char x;
public:
	test() { cout<< 'A'<<endl; }
	test(char c) { cout << c; }
	~test() { cout << 'B'; }
};

struct Foo
{
	Foo() {};
	Foo(int) {};
	void fun() {};

};
typedef struct list_t
{
	struct list_t* next;
	struct list_t* prev;
	char data[0];

}list_t;
int main(void)
{
	/**test p1, *p2;
	p2 = new test('x');
	delete p2;//AxBB
	*/
	/*int a[5] = {1,2,3,4,5};
	int* ptr = (int*)(&a + 1);//指向最后一个数字的的下一个整型
	printf("%d,%d\n",*(a+1),*(ptr-1)); //2,5*/
	/**int a[] = {1,2,3,4};
	int* b = a;
	*b += 2; //a[0]=3
	*(b + 2) = 2; //a[2]=2
	b++;
	printf("%d,%d\n", *b, *(b + 2));//2,4
	for (int i=0 ; i<4 ; i++)
	{
		cout << a[i];//3224
	}*/
	//cout<< sizeof(list_t)<<endl; //8

	Foo a(10);
	a.fun();
	Foo b();//返回一个Foo类型
	//b.fun();//表达式必须包含类型


	/**创建了几个对象
	 Foo a();//定义了一个函数
	 Foo b(2);//1
	 Foo c[3];//3
	 Foo &ra =b;
	 Foo d=b;//拷贝构造函数，1
	 Foo* pa =c;//0
	 Foo* p = new Foo(4);//4
	*/
	getchar();
	system("pause");
	return 0;
}

//044 洗牌发牌算法
--! @brief      初始化牌组
--! @return     { description_of_the_return_value }
--! @author     kun si
--! @date       2017-07-31
local function initCard()
	local cardsList = {}

	for i = 14, 15 do
		local tmpCard={value=i, color=0}
		table.insert(cardsList, tmpCard)
	end

	for i = 1, 4 do
		for j = 1, 13 do
			local tmpCard={value=j, color=i}
			table.insert(cardsList, tmpCard)
		end
	end
	return cardsList
end

--! @brief      洗牌
--! @param      cardsList  The cards list
--! @return     { description_of_the_return_value }
--! @author     kun si
--! @date       2017-07-31
local function shuffleCards(cardsList)
	math.randomseed(tostring(os.time()):reverse():sub(1, 7))
	local shuffleTime = math.random(300, 540)
	--print("shuffleTime", shuffleTime)
	for i = 1, shuffleTime do
		local tmpCard = {}
		card1 = math.random(1, 54)
		card2 = math.random(1, 54)
		while card1 == card2 do
            card2 = math.random(1, 54)
        end
        --print("card", card1, card2)
		tmpCard = cardsList[card1]
		cardsList[card1] = cardsList[card2]
		cardsList[card2] = tmpCard
	end
	return cardsList
end

--! @brief      发牌
--! @param      cardsList  The cards list
--! @return     { description_of_the_return_value }
--! @author     kun si
--! @date       2017-07-31
local function dealer(cardsList)

	local players = {}
	for i = 1, 5 do
		for j = 1, 6 do
			if not players[j] then
				players[j] = {}
			end
			local tmpCard = table.remove(cardsList)
			table.insert(players[j], tmpCard)
		end 
	end
	return players
end

local cardsList = initCard()

cardsList = shuffleCards(cardsList)

local players = dealer(cardsList)


for k, v in ipairs(players) do
	print(string.format("玩家%d手牌为：", k))
	for k1, v2 in ipairs(v) do
		print(v2.value, v2.color)
	end
end


//045 快速排序
/**
@common:快速排序-挖坑+分治（递归）
@param:a为待排序数组，left，right为两个指针
@return:void
@平均时间复杂度：O(N*logN)
@稳定性：不稳定
*/
void  quickSort(int* a, int left, int right)
{
	int i, j;
	int temp;
	if (left >= right)
	{
		return;
	}
	//初始化
	i = left;
	j = right;
	temp = a[i];
	while (i != j)
	{
		//从后向前扫描第一个比temp大的数
		//降序的时候改变从右往左扫描找第一个比temp小的数
		while (i < j && a[j] <= temp) j--;
		if (i < j)
		{
			//换到合适的位置
			a[i++] = a[j];
		}
		while (i<j && a[i] >= temp) i++;
		if (i<j)
		{
			a[j--] = a[i];
		}

	}
	//找到temp的位置
	a[i] = temp;
	//分治，递归左边和右边部分
	quickSort(a, left, i - 1);
	quickSort(a, i + 1, right);
}

//046 计算出现奇数次的数
1.有一个整型数组A，其中只有一个数出现了奇数次，其他的数都出现了偶数次，请打印这个数。
要求时间复杂度为O(N)，额外空间复杂度为O(1)。

给定整形数组A及它的大小n，请返回题目所求数字。

测试样例：
[1,2,3,2,1],5
返回：3


#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
using namespace std;


int findOddAppearce(vector<int> A, int n)
{
	int e = 0;
	//按位异或运算
	for (int i=0 ; i < n ; i++)
	{
		e = e ^ A[i];
		//1 3 0 2 3 //3
		cout << e<< endl;
	}
	return e;
}
int main()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(2); 
	v.push_back(1);
	cout << findOddAppearce(v,5)<< endl;
	getchar();
	return 0;
}

//047 句子反序
#include <iostream>
#include <string>
#include <map>
using namespace std;

/**
   句子逆序，“i love you”处理后变成 “you love i”
   

string reverseString(string s , int n)
{
	cout << s<< endl;
	reverse(s.begin(), s.end());
	cout << s<< endl;
	int i, j;
	for (int i = 0 ; i < n ; i++)
	{
		j = i;
		//while循环找到一个单词
		while (j < n && s[j] != ' ')
		{
			j++;
		}
		//找到单词进行翻转
		reverse(s.begin() + i,s.begin() + j);
		
		//把j赋值给i，更新到最新的位置，查找下一个单词
		i = j;
	}
	return s;
}

int main()
{
	string s = "i love you";
	cout << reverseString(s,10) << endl;
	getchar();
	return 0;
}

//048 寻找满足条件的两个数
题目：输入一个数组和一个数字，在数组中查找两个数，使得它们的和正好是输入的那个数字。
要求时间复杂度是O(n)。如果有多对数字的和等于输入的数字，输出任意一对即可。
例如输入数组1、2、4、7、11、15和数字15。由于4+11=15，因此输出4和11。

不论原序列是有序还是无序，解决这类题有以下三种办法：
1、二分（若无序，先排序后二分），时间复杂度总为O（n*logn），空间复杂度为O（1）；
2、扫描一遍X-S[i] 映射到一个数组或构造hash表，时间复杂度为O（n），空间复杂度为O（n）；
3、两个指针两端扫描（若无序，先排序后扫描），时间复杂度最后为：有序O（n），无序
	O（n*logn+n）=O（n*logn），空间复杂度都为O（1）。 
	所以，要想达到时间O（N），空间O（1）的目标，除非原数组是有序的（指针扫描法），不然，
	当数组无序的话，就只能先排序，后指针扫描法或二分（时间n*logn，空间O（1）），
	或映射或hash（时间O（n），空间O（n））。时间或空间，必须牺牲一个，自个权衡吧。 
	综上，若是数组有序的情况下，优先考虑两个指针两端扫描法，以达到最佳的时（O（N）），
	空（O（1））效应。否则，如果要排序的话，时间复杂度最快当然是只能达到N*logN，
	空间O（1）则是不在话下。
#include <iostream>
#include <stdio.h>
#include <algorithm>
//#include <vector>
using namespace std;


//两个指针扫描法
pair<int,int> findNumber(int* s,int n,int x)
{
	
	sort(s,s+n);  数组无序先排序

	//先确定首尾的位置
	int * begin = s;
	int * end = s + n-1;
	while(begin < end)
	{
		if(*begin + *end > x)
		{
			--end;//尾指针减1
		}else if(*begin + *end < x)
		{
			++begin;//头指针加1
		}else 
		{
			return pair<int,int>(*begin,*end);
		}
	}
   return pair<int,int>(-1,-1);
}

//另一种实现方法
bool  findnum2(int data[],unsigned int length, int sum, int& first_num, int& second_num)
{
	if(length<1)
	{
		return false;
	}
	int begin = 0;
	int end = length - 1;
	while(end > begin)
	{
		long current_sum = data[begin] + data[end];
		if(current_sum == sum)
		{
			first_num = data[begin];
			second_num = data[end];
			return true;
		}
		//当前值大于最大的和，排序的数组中要把尾端的指针减少
		else if(current_sum > sum)
			end--;
		else
			begin++;
	}
	return false; 
}
int main() 
{
	//pair<int,int> p;
	int arr[]={1,2,4,7,11,15};
	int m=1,n=2;
	int &i=m;
	int &j=n;
	//p=findNumber(arr,6,15);
	//cout<<p.first<<" "<<p.second<<endl;
	cout<< findnum2(arr,6,15,i,j)<<endl;
	getchar();
	return 0;
}



//048 析构函数和结构体指针
#include "pch.h"
#include<stdio.h>
#include <iostream>

using namespace std;

void fun(char str_arg[100])
{
	cout<< sizeof(str_arg)<<endl;
}
class A
{
	friend long fun(A s)
	{
		if (s.x < 3)
		{
			return 1;
		}
		return s.x + fun(A(s.x - 1));
	}
public:
	A(long a)
	{
		x = a--;
		//等价于 x = a; a--;不会对结果产生影响
	}
private:
	long x;

};
int sum(int a)
{
    int c = 0;
	static int b = 3;//静态全局变量，每次调用都会加2，全局作用域
	c += 1;
	b += 2;
	return a + b + c;
}


int  main(int argc,char* argv[])
{
	/**struct t
	{
		int x;
		int* y;
	}*p;
	int a[] = {1,2};
	int b[] = {3,4};
	struct t c[2] = {5,a,6,b};
	p = c;
	printf("%d,%d",*(p->y),++(*p).x);//1,6
	*/

	/**char str[] = "hello";
	char* p = str;
	cout<< sizeof(str)<<endl; //6
	cout << sizeof(p) << endl;//8,64位指针占8个字节
	char str2[] = "test";
	fun(str2);//8
	*/
	
	/**int sum = 0;
	for(int i = 0; i< 5 ; i++)
	{
		sum += fun(A(i));
	}
	cout << sum; //15
	
	  i= 0 1 2 3 4
   A(i)= 0 1 2 3 4
f(A(i))= 1 1 1 4 8
    sum= 1 2 3 7 15
*/

	/**int i;
	int a = 2;
	for(i = 0; i < 5; i++)
	{
		printf("%d",sum(a));//8 10 12 14 16
    }
	*/

	//PAT (*ad)[3];调用构造函数的次数为0
	//指针指向含有3个PAT对象，没有实例化其中的对象

	/*
	C c;
	void main()
	{
	 A* pa =new A();
	 B b;
	 static D d;
	 delete pa;
	}
	//调用析构函数的顺序是 A B D C
	解析：(1)全局变量和局部静态变量都在静态存储区划分，全局作用域>静态作用域
	(2)A是由new从系统堆中分配的，程序运行之后不会释放，用delete释放，
	(3)B是在栈中分配的，程序结束时候系统调用析构函数释放
	(4)B在main函数之后才释放，A在其之前释放
	*/
	//int a[2][]={{1,2},{3,4}};//error，行可省，列不可省
	getchar();
	system("pause");
	return 0;

}

//049 虚函数和数组
#include "pch.h"
#include<stdio.h>

#include <iostream>

using namespace std;

class CParent
{
public:
	virtual  void Intro()
	{
		printf("I am a parent!\n");
		Hobby();
	}
	virtual void Hobby()
	{
		printf("i am a football!\n");
	}

};

class CChild :public CParent
{
public:
	virtual void Intro()
	{
		printf("i am a child!\n");
		Hobby();
	}
	virtual void Hobby()
	{
		printf("i am a baseball!\n");
	}
};
int  main(int argc,char* argv[])
{
	/**CChild* pChild= new CChild();
	CParent* pParent = (CParent*)pChild;
	pParent->Intro();//i am a child!,i am a baseball!
	*/
	
	/*char c[] = "abc";
	int i = 0;
	do;
	while (c[i++] != '\0');
	printf("%d",i-1); //3=4-1
	*/

	const int N = 10;
	const int M = 2;
	int* a = new int[N];
	for (int i = 0 ; i < N ; i++)
		a[i] = (0 == i % 2) ? (i + 2) : (i + 0);

	/**
	(1)int* arr[5]--指针数组，里面装的是int*
	(2)int(*arr)[5]--数组指针，指向一个长度为5的数组
	(3)int(&arr)[5]--数组的引用
	*/
	int(*b)[N / M] = (int(*)[N / M])a;

	for (int i=0;i<M;i++)
		for (int j=0;j<N/M;j++)
			printf("%d",b[i][j]);//21436587109
	getchar();
	system("pause");
	return 0;

}


//050 内存拷贝和移动
#include "pch.h"
#include<stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

//memcpy存在内存重叠的问题，使用memmove
//实现一个memcpy()
void* memcpy(void* dst, const void* src, size_t len)
{
	if (NULL == dst || NULL == src)
	{
		return NULL;
	}
	void* ret = dst;
	//无内存重叠,从低地址复制
	if (dst <= src || (char*)dst > (char*)src + len)
	{
		while (len--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst + 1;
			src = (char*)src + 1;
		}
	}
	else//有内存重叠,从高地址复制
	{
		//先找到从高地址复制的位置
		dst = (char*)dst + len - 1;
		src = (char*)src + len - 1;
		while (len--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst - 1;
			src = (char*)src - 1;
		}
	}
	return ret;
}

//避免内存重叠,使用MyMemmove
void* MyMemmove(void* str1, void* str2,size_t n)
{
	char* pStr1 = (char*)str1;
	char* pStr2 = (char*)str2;
	if (pStr1 < pStr2)//从pStr1复制到pStr2,从低地址往高地址复制
	{
		for (size_t i = 0;i < n ; i++)
		{
			*(pStr1++) = (*pStr2++);
		}
	}
	else//从pStr2复制到pStr1,从高地址往低地址复制
	{
		pStr1 += n - 1;
		pStr2 += n - 1;
		for (size_t i = 0; i < n ; i++)
		{
			*(pStr1--) = *(pStr2--);
		}
	}
	return str1;
}
int main()
{

	unsigned char i = 1;//8位，0~2^8-1
	i -= 4;//-3
	printf("%d", i); //2^8-3
	getchar();
	system("pause");
	return 0;

}


//051 字符串操作
#include "pch.h"
#include<stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class B
{
public:
	B()
	{
		cout<<"default constructor!" <<endl;
	}
	~B()
	{
		cout<< "destructed"<<endl;
	}

	B(int i):data(i)
	{
		cout<<"construct by value"<< data<<endl;
	}
private:
	int data;
};
//类外定义的函数，返回值和参数都是B类的对象，在进行使用是要先隐式转换成B类的对象
B play(B b)//复制构造函数
{
	return b;
}
//'\0'实际在内存中表示为0
//int* p = new int(1000);  delete p;

char* mystring()
{
   //该buffer数组没有使用malloc或者new分配内存，数据存在栈区，函数结束，数组也不存在了
	//会产生野指针，内容未知
	char buffer[6] = {0};
	const char* s = "hello world!";
	for (int i = 0 ; i < sizeof(buffer) - 1 ; i++)
	{
		buffer[i] = *(s + i);
	}
	return buffer;//返回局部变量或临时变量的地址
}

int a = 1, b = 2;
int  main(int argc,char* argv[])
{
	/**char x[] = "STRING";
	x[0] = 0;
	x[1] = '\0';
	x[2] = '0';
	printf("%d%d\n",sizeof(x),strlen(x));//7,0
	解析：sizeof()是在编译时间就计算好，建立的最大对象的字节大小
	      strlen()参数必须是char*,从字符串的第一个地址开始遍历，遇到NULL(0)停止
	*/

	//B temp = play(5);//construct by value 5,destructed,destructed
	//play(5)包含隐式转换，调用带参数的构造函数;然后play()函数返回后，调用b的析构函数，释放形参;
	//最后mian()函数结束之后释放temp;
	/*B temp(6);//construct by value5,destructed*/


   /** char src[] = "hello,world";
	char* desc = NULL;
	int len = strlen(src);//字符串的长度，不包含'\0'
	desc = (char*)malloc(len + 1);//加上'\0'的长度
	char* d = desc;
	char* s = &src[len - 1];//记得加上&符号，指针变量指向的是一个地址
	while (len-- != 0)
	{
		*d++ = *s--;
	}
	*d = '\0';//记得加上'\0'
	printf("%s",desc);
	free(desc);//释放malloc申请的内存空间
	*/

	//printf("%s\n", mystring());//烫烫烫烫$?  内容未知

	/**char a = '1', b = '2';
	printf("%c",b++); //2,字符
	printf("%d\n",b - a);//2,整数，51-49=2
	*/

	//printf("%d%d\n",::a,::b);调用全局变量
	getchar();
	system("pause");
	return 0;

}

//052 字节对齐和const
#include "pch.h"
#include<stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

//线程同步的方法：临界区，互斥量，事件，信号量

#pragma pack(2)
class Bu
{
	int number;//4
	union UBuffer
	{
		char buffer[13];//13
		int number;//4
	}ubuf; //14,不写#pragma pack(2)为16

	/**void foo() {}//0
    typedef char* (*f)(void*);//0
	enum  //4
	{
	 hdd,
	 ssd,
	 blueray
	}disk;*/

	/*用函数包起来，sizeof(bu)就是18
	void foo() //0
	{
    typedef char* (*f)(void*);//0
	 enum  //4
	 {
	    hdd,
	    ssd,
	    blueray
	  }disk;
	}
	*/
}bu;

class CBuffer
{
	char* m_pBuffer;
	int m_size;
public:
	CBuffer()
	{
		m_pBuffer = NULL;
	}

	~CBuffer()
	{
		Free();
	}

	void Allocate(int size)//声明为私有函数比较好
	{
		m_size = size;
		m_pBuffer = new char[size];//先检查m_pBuffer是否为空，容易造成内存泄漏
	}
private:
	void Free()
	{
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}
	}
public:
	/**const规则：
	(1)不能在const函数中修改所在类的对象的数据，const函数中*this是常量
	(2)const函数只能调用其他的const函数(*this是常量)，不能调用非const
	    函数(其中的*this是一个变量)
	   在使用const_cast后，可以在const函数中调用非const函数
	(3)const与同名的非const函数是重载函数
	(4)const只能调用const函数，但是非const函数能调用const函数
	*/
	//常成员函数不应该对数据成员做出修改
	void SaveString(const char* pText) const
	{
		//Allocator(strlen(pText)+1);//并且将该函数声明为私有函数更合理
		//字符串拷贝时没有对空间做出判断，可能造成程序崩溃
		strcpy(m_pBuffer,pText);
	}
	char* GetBuffer() const 
	{
		return m_pBuffer;
	}
};

int main()
{
	//cout<< sizeof(bu)<<endl; //22

	CBuffer buffer1;
	buffer1.SaveString("hello");
	printf(buffer1.GetBuffer());
	getchar();
	system("pause");
	return 0;

}

//053随机数

#include<stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
using namespace std;
class  A
{
public:
	A()
	{
		printf("A");
	}
    //没加virtual只析构A,加了virtual之后析构顺序为deC,deB,deA,
	//会析构子类的构造函数(构造顺序为A B C)
	 virtual  ~A() 
	{
		printf("deA");
	}
};
class B
{
public:
	B()
	{
		printf("B");
	}
	~B()
	{
		printf("deB");
	}
};

//class C:public B ,public A 打印出B A C deA
class C :public B, public A
{
public:
	C()
	{
		printf("C");
	}
	~C()
	{
		printf("deC");
	}
};

//从0-N-1中等概率输出m个不重复的数
/**
证明：
1.输出不同的m个值：
由这个for循环循环n次，且在满足条件时才输出i,可知，输出不同值的要求已满足，因为每次输出的
都是i值，而i值每次都是不一样的，m--保证了程序在输出了m个值后就不再输出i。
2.等概率：
  在i=0时，rand()%(n-i)的取值范围为0到n-1，共n个数，此时要输出0只需要rand()%(n-i)小于m，
故i=0被输出的概率为m/n;
在i=1时，rand()%(n-i)的取值范围为0到n-2，共n-1个数，若i=0没有被输出，则m--未被执行，
此时i=1被输出的概率为m/(n-1)，若i=0已经被输出了，则m变为m-1，此时i=1被输出的概率为(m-1)/(n-1)；由概率论的知识，可知此时i=1被输出的概率为

P=(1-m/n)*(m/(n-1))+m/n*((m-1)/(n-1))=m/n；

以此类推，可知每个数被输出的概率都为m/n。
*/
void knuth(int n ,int m)
{
	srand((unsigned int)time(0));
	for (int i = 0 ; i < n ; i++)
	{
		if (rand()%(n - i) < m)
		{
			cout<< i<<endl;
			m--;
		}
	}
}

//给定一个返回0~6的随机自然数的函数，记为rand6()，则如何用rand6()函数构造
//返回0~9随机自然数的函数rand9()？

/**
  把第一个rand6()产生的随机数a放在个位上，而把第二个rand6()产生的随机数b放在十位上，
所有可能为：00~06,10~16,20~26,30~36，40~46,50~56，60~66。很容易看出这些就是7进制数，
所以我们用rand6()*7+rand6()就能生成0~49范围的随机数，而且每个数产生的概率相等，都是1/49。
产生40~49之间的随机数时不保留而重新产生一个新的随机数，这样产生0~39之间的数也是等概率的，
仍然都是1/49，对于产生的数我们记为r（0<= r <=39），则 r/4 即可返回0~9范围的数。而且 r 取
0~3这四个数时 r/4 返回0，r 取4~7这四个数时 r/4 返回1，……，r 取36~39这四个数时 r/4 返回9。
因此返回0~9范围的每个数字的概率都是4/49，满足等概率。
*/
// 随机产生0~6之间的数
int rand6()
{
	return rand() % 7;
}

// 用rand6()编写随机产生0~9之间的数
int rand9()
{
	int a, b, r;
	do {
		a = rand6();
		b = rand6();
		cout << "a = " << a << ", b = " << b << endl;
		r = a * 7 + b;
	} while (r >= 40);

	return r / 4;
}
int main()
{
	/*A* a = new C();
	delete a; //A B C deA */
	
	/*char a = 101;//char的范围是 -128--+127
	int sum = 200;
	a += 27; //溢出后a=-128
	sum += a;
	printf("%d\n",sum);*/

	/*int i = 0;
	i = ~i; //~n=-(n + 1)
	printf("%d\n",i);//-1
	*/
	//knuth(10,5);
	/**
   函数一：int rand(void)；
   返回一个[0,RAND_MAX]间的随机整数。
   函数二：void srand(unsigned seed)；
   参数seed是srand()的种子，用来初始化srand()的起始值
	*/
	srand((unsigned)time(NULL));
	printf("%d",rand9());

	getchar();
	system("pause");
	return 0;

}

//054 类
#include "pch.h"
#include<stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
using namespace std;


class  A
{
public:
	A(int i=0)
	{
		cout << i;
	}
	A(const A& x)
	{
		cout << 2;
	}
    
	A& operator=(const A& x)
	{
		cout << 3;
		return *this;
	}
	~A()
	{
	  cout << 4;
	}
};

class parent
{
public:
	virtual void output();
};
void parent::output()
{
	printf("parent");
}

class son :public parent
{
public:
	virtual void output();
};

void son::output()
{
	printf("son");
}

/**
  (1)为了让某个类只能在堆上分配内存，只需将析构函数设置为私有即可。
  解析：
   编译器在为类对象分配栈空间时，会先检查类的析构函数和非静态函数的访问性，
若析构函数是私有的，那么类就不会在栈上分配内存
(2)只能建立在栈上
   只有使用new运算符，对象才会建立在堆上，因此，只要禁用new运算符就可以实现
   类对象只能建立在栈上。将operator new()设为私有即可。
*/

#define N 5
int x[N];
 int count = 0;
void dump()
{
	for (int i = 0 ; i < N ; i++)
	{
		printf("%d",x[i]);
	}
	printf("\n");
}
void swap(int a, int b)
{
	int t = x[a];
	x[a] = x[b];
	x[b] = t;
}

void run(int n)
{
	int i;
	if (n == N-1)
	{
		dump();
	//	count++;
		return;
	}
	for (i =n;i<n;i++)
	{
		swap(n,i);
		run(n + 1);
		swap(n,i);
	}
}
#define F 2
#define M F+1
#define NUM (M+1)*M/2
//(F+1+1)*F+1/2 =8
int main()
{
	
	/*A a1(1), a2(2);
	A a3 = a1;//122444
	*/

	/**son s;
	memset(&s, 0, sizeof(s));//虚函数表被破坏
	parent& p = s;
	p.output();//error
	*/

	/*char a[80] = "AB",b[80]="LMNP",i=0;
	strcat_s(a,b);
	while (a[i] != '\0')
	{
		i++;//先++，跳过a[0]和a[1]
		b[i] = a[i];
	}
	puts(b);//LBLMNP
	*/

	/*int i = 0;
	for (i = 0 ; i < N ; i++)
	{
		x[i] = i + 1;		
	}
	run(0);
	//printf("total count:%d\n",count);
	*/
	
	/**int a, x;
	for (a = 0 ,x = 0 ; a <= 1 && !x++ ; a++)
	{
		a++;
	}
	//!x++ 等价于！(x++)
	cout<< a<< x <<endl;//2 1
	*/
	printf("%d\n",NUM);//8
	getchar();
	system("pause");
	return 0;
}


//055  单例类和线程安全

#include "pch.h"
#include<stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
using namespace std;


class  A
{
public:
	A(int i=0)
	{
		cout << i;
	}
	A(const A& x)
	{
		cout << 2;
	}
    
	A& operator=(const A& x)
	{
		cout << 3;
		return *this;
	}
	~A()
	{
	  cout << 4;
	}
};

class parent
{
public:
	virtual void output();
};
void parent::output()
{
	printf("parent");
}

class son :public parent
{
public:
	virtual void output();
};

void son::output()
{
	printf("son");
}

/**
  (1)为了让某个类只能在堆上分配内存，只需将析构函数设置为私有即可。
  解析：
   编译器在为类对象分配栈空间时，会先检查类的析构函数和非静态函数的访问性，
若析构函数是私有的，那么类就不会在栈上分配内存
(2)只能建立在栈上
   只有使用new运算符，对象才会建立在堆上，因此，只要禁用new运算符就可以实现
   类对象只能建立在栈上。将operator new()设为私有即可。
*/

#define N 5
int x[N];
 int count = 0;
void dump()
{
	for (int i = 0 ; i < N ; i++)
	{
		printf("%d",x[i]);
	}
	printf("\n");
}
void swap(int a, int b)
{
	int t = x[a];
	x[a] = x[b];
	x[b] = t;
}

void run(int n)
{
	int i;
	if (n == N-1)
	{
		dump();
	//	count++;
		return;
	}
	for (i =n;i<n;i++)
	{
		swap(n,i);
		run(n + 1);
		swap(n,i);
	}
}
#define F 2
#define M F+1
#define NUM (M+1)*M/2
//(F+1+1)*F+1/2 =8
int main()
{
	
	/*A a1(1), a2(2);
	A a3 = a1;//122444
	*/

	/**son s;
	memset(&s, 0, sizeof(s));//虚函数表被破坏
	parent& p = s;
	p.output();//error
	*/

	/*char a[80] = "AB",b[80]="LMNP",i=0;
	strcat_s(a,b);
	while (a[i] != '\0')
	{
		i++;//先++，跳过a[0]和a[1]
		b[i] = a[i];
	}
	puts(b);//LBLMNP
	*/

	/*int i = 0;
	for (i = 0 ; i < N ; i++)
	{
		x[i] = i + 1;		
	}
	run(0);
	//printf("total count:%d\n",count);
	*/
	
	/**int a, x;
	for (a = 0 ,x = 0 ; a <= 1 && !x++ ; a++)
	{
		a++;
	}
	//!x++ 等价于！(x++)
	cout<< a<< x <<endl;//2 1
	*/
	printf("%d\n",NUM);//8
	getchar();
	system("pause");
	return 0;
}

//056 类设计
#include "pch.h"
#include<stdio.h>
#include <string.h>
#include <iostream>
#include "afxmt.h"
using namespace std;

/**
设计一个抽象类，使得它可以完成有序数组归并的任务
*/

class Sequence 
{
public:
	virtual Object* next()=0;
	virtual bool hasNext()=0;
	virtual Object* top() = 0;
	virtual bool add(Object* obj) = 0;
};

class Object
{
public:
	virtual bool lessthan(Object* another);
};

/**
  设计一个多终端日志打印的接口，使得它可以动态支持不同终端
的日志打印

*/

//抽象类
class BaseTerminal
{
public:
	virtual void PrintLog(const string& message) = 0;
};

class Scene :public BaseTerminal
{
public:
	void PrintLog(const string& message) = 0;
};
class NetWork :public BaseTerminal
{
public:
	void PrintLog(const string& messsage) = 0;
};
//全局的打印日志函数
void  LogPrint(const BaseTerminal& terminal);


/**
  给N张牌和一个随机函数，实现一个洗牌算法
*/

void shuffle(int cards[], int n)
{
	if (NULL == cards)
	{
		return;
	}
	//随机种子
	srand(time(0));
	for(int i = 0 ; i < n - 1 ; i++)
	{
		int index = i + rand() % (n - i);
		int temp = cards[i];
		cards[i] = cards[index];
		cards[index] = temp;
	}
}

/**
  智能指针是一种资源管理类，通过对原始指针进行封装，
在资源管理对象进行析构时对指针指向的内存进行释放
实现一个智能指针
*/

class Object2;
class SmartPointer;

class Counter
{
	friend class SmartPointer;
public:
	Counter():ptr(NULL),cnt(0){}
	Counter(Object2* p):ptr(p),cnt(1){}
	~Counter() { delete ptr; }
private:
	Object2* ptr;
	int cnt;
};
class SmartPointer
{
public:
	SmartPointer(Object2* p)
	{
		ptr_counter = new Counter(p);
	}

	//拷贝构造函数
	SmartPointer(const SmartPointer& sp)
	{
		ptr_counter = sp.ptr_counter;
		++ptr_counter->cnt;//引用计数增加
	}

	//拷贝赋值函数
	SmartPointer& operator=(const SmartPointer& sp)
	{
		++sp.ptr_counter->cnt;
		--ptr_counter->cnt;
		if (0 == ptr_counter->cnt)
		{
			delete ptr_counter;
		}
		//重新赋值
		ptr_counter = sp.ptr_counter;
	}
	~SmartPointer()
	{
		--ptr_counter->cnt;//引用计数减1
		//引用计数为0时，释放指针
		if (0 == ptr_counter->cnt  )
		{
			delete ptr_counter;
		}
	}
private:
	Counter* ptr_counter;

};

/**

用普通的互斥锁编程实现一个读写锁*/

int  mutex_init() {}
void lock(int ) {}
void unlock(int) {}
int count_mutex = mutex_init();
int write_mutex = mutex_init();
int read_cout = 0;


void read_lock()
{
	lock(count_mutex);
	read_cout++;
	//当前读的数量为1
	if (1 == read_cout)
	{
		//锁住写互斥量
		lock(write_mutex);
	}
	//解锁计数互斥量
	unlock(count_mutex);
}

void read_unlock()
{
	lock(count_mutex);
	read_cout--;
	//当前读的计数为0
	if (0 == read_cout)
	{
		//释放写互斥量
		unlock(write_mutex);
	}
	unlock(count_mutex);
}

void write_lock()
{
	lock(write_mutex);
}

void write_unlock()
{
	unlock(write_mutex);
}
int main()
{
	getchar();
	system("pause");
	return 0;

}


//057 ArrayClass
#include "pch.h"
#include<stdio.h>
#include <string>
#include <algorithm>
#include <iostream>
#include<time.h>
#include <stdlib.h>
#include <assert.h>
using namespace std;

template <class T>
class Array
{
public:
	//ctor
	Array(int sz =50)
	{
		assert(sz > 0);
		size = sz;
		list = new T[size];
	}
	//dtor
	~Array()
	{
		delete[] list;
	}

	//复制构造函数
	Array(const Array<T>& a)
	{
		size = a.size;
		//新建对象
		list = new T[size];
		//赋值
		for (int i = 0 ;i < size; i++)
		{
			list[i] = a[i];
		}
	}

	//=号重载
	Array<T>& operator= (const Array<T>& rhs)
	{
		if (this != rhs)
		{
			if (size != rhs.size)
			{
				delete[] list;
				size = rhs.size;
				list = new T[size];
			}
			for (int i = 0 ; i < size ; i++)
			{
				list[i] = rhs.list[i];
			}
		}
		return *this;
	}

	//下标[]重载
	T& operator[](int i)
	{
		assert(i >= 0 && i<size);
		return list[i];
	}
	//下标[]重载2
	const T& operator[]( int i)const
	{
		assert(i >= 0 && i < size);
		return list[i];
	}
	//重载指针转换运算符，将Array类的对象名转换为T类型的指针  
	//指向当前对象中的私有数组，  
	//因而可以像使用普通数组首地址一样使用Array类的对象名  
	/*operator T* ()
	{
		return list;
	}

	const operator T*() const 
	{
		return list;
	}*/
	//数组的大小
	int getsize() const 
	{
		return size;
	}

	//重新设置数组的大小
	void resize(int sz)
	{
		if (sz == size)
		{
			return;
		}
		//new出新的空间
		T* newlist = new T[sz];
		//确定循环的大小
		int n = sz < size ? sz : size;
		//复制新的值
		for (int i = 0 ; i < n ; i++)
		{
			newlist[i] = list[i];
		}
		delete[] list;
		list = newlist;
		size = sz;
	}
	//添加数据
	void add(const T x)
	{
		assert(size >= 0);
		size++;
		T* newlist = new T[size];
		for (int i =0 ;i <size - 1 ; i++)
		{
			newlist[i] = list[i];
		}
		newlist[size - 1] = x;
		delete[] list;
		list = newlist;
	}
	//在指定位置插入数据
	void insert(const int n ,const int x)
	{
		assert(size >= 0 && n < size );
		T* newlist = new T[n];
		for (int i = 0; i < n; i++)
		{
			newlist[i] = list[i];
		}
		newlist[n] = x;
		//后半段的赋值
		for (int i = n + 1 ; i < size ; i++)
		{
			newlist[i] = list[i - 1];
		}
		delete[] list;
		list = newlist;
	}
	//在指定位置删除数据
	void remove(const int n)
	{
		assert(size >= 0 && n < size);
		T* newlist = new T[--size];
		//复制[0,n-1]的数据
		for (int i = 0 ; i < n ; i++)
		{
			newlist[i] = list[i];
		}
		//数据移动,将n之后的元素往前移动一位
		for (int i = n + 1 ; i < size ; i++)
		{
			newlist[i - 1] = list[i];
		}
		delete[] list;
		list = newlist;
	}
private:
	T* list;
	int size;
};

int main()
{
	Array<int> a(4);
	cout << "size=" << a.getsize() << endl;
	for (int i = 0; i < a.getsize(); i++) {
		a[i] = i;
	}
	for (int i = 0; i < a.getsize(); i++) {
		cout << a[i] << endl;
	}
	cout << "***********test***************" << endl;
	cout << "add " << endl;
	a.add(53);
	for (int i = 0; i < a.getsize(); i++) {
		cout << a[i] << endl;
	}
	cout << "size=" << a.getsize() << endl;
	cout << "***********test***************" << endl;
	cout << "remove" << endl;
	a.remove(2);
	for (int i = 0; i < a.getsize(); i++) {
		cout << a[i] << endl;
	}
	cout << "size=" << a.getsize() << endl;
	cout << "***********test***************" << endl;
	cout << "insert" << endl;
	a.insert(2, 100);
	for (int i = 0; i < a.getsize(); i++) {
		cout << a[i] << endl;
	}
	cout << "size=" << a.getsize() << endl;
	cout << "***********test***************" << endl;
	getchar();
	system("pause");
	return 0;

}


//058 string

#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;
int main()
{
	/*strcpy()字符拷贝
	char str1[11] = { "TsinghuaOK" };
	char str2[10] = { "Computer" };
	strcpy_s(str1, str2);
	cout << str1 << endl;*/

	
	/*strncpy()拷贝前n个字符
	char str1[50] = { "TsinghuaOK" };
	char str2[10] = { "Computer" };
	strncpy_s(str1, str2, 4);
	cout<< str1<<endl;
	*/

  /* atoi()将字符串转换成整数
	char a[] = "-100";
	char b[] = "456";
	int c = 0;
	c = atoi(a) + atoi(b);
	printf("c = %d\n", c);
	*/

	/*strcat_s()连接两个字符
	char str1[50] = { "TsinghuaOK" };
	char str2[10] = { "Computer" };
	strcat_s(str1, str2);
	cout << str1 <<endl;
	*/
	
	/*strncat_s()将n个字符连接在第一参数的后面
	char str1[50] = { "TsinghuaOK" };
	char str2[10] = { "Computer" };
	strncat_s(str1, str2,4);
	cout << str1 << endl;*/

	/*strlen()遇到'\0'结束，sizeof()是实际的大小
	char str2[10] = { "Computer" };
	int len = strlen(str2);
	cout<< len<<endl;
	int size = sizeof(str2);
	cout << size << endl;*/

	/*strcmp()比较两个字符串的大小(-1,0,1)
	char buf1[] = "aa";
	char buf2[] = "ab";
	//cout<< strcmp(buf1,buf2)<<endl; //-1
	cout << strcmp(buf2, buf1) << endl; //1
	*/


	/*
	C++中字符串的API操作
	*/
	string s="abc";
    //cout<< s.size()<<endl;
	s.append("d");
	//cout<< s.size()<<endl;
	s.insert(1,"f");//在特定位置插入字符
	s.replace(2, 1, "g");
	//s.substr(2);
	cout << s.find("g")<<endl;
	for(int i = 0 ; i < s.size() - 1 ; i++)
	{
		cout<<s[i]<<endl;
	}
	getchar();
	system("pause");
	return 0;
}


//059 pointer

#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;
void fun(int* a,int b[])
{
	b[0] = *a + 6;
}
struct st_task
{
	uint16_t id;
	uint32_t value;
	uint64_t timestamp;
};
void f(int a[],int i,int j)
{
	int t;
	if (i < j)
	{
		t = a[i];
		a[i] = a[j];
		a[j] = t;
		f(a,i+1,j-1);
	}
}
void f2(char* c,char d)
{
	//char a = 'A', b = 'a';
	//f2(&b, a);
	*c = *c + 1;
	d = d + 1;
	cout << *c << d<<endl; //bB
}
int main()
{
	/**
	c语言中分动态存储(使用时才分配内存)和静态存储(运行时就分配好了)
	auto,register 动态
	static,extern 静态
	*/
	/*int a, b[5];
	a = 0;
	b[0] = 3;
	fun(&a,b);//数组名作为函数的参数传递，此时会退化为指针，
	调用函数时，相当于传地址，会改变数组元素的值
	printf("%d\n",b[0]); //6
	*/

	//union中所有成员相对于基地址的偏移量都为0
	/*
	union Data
	{
		struct { int x; int y; }s;
		int x, y;
	}d;
	d.x = 1;
	d.y = 2;
	//d.x,d.y起始地址相同，共享内存空间，给任意一个变量赋值，其他变量也赋值相同
	d.s.x = d.x*d.x;
	d.s.y = d.y + d.y;
	printf("%d\n%d\n", d.s.x,d.s.y); //4,8
	*/
	/**d是局部变量，起始值是0
	                    x,y,s.x    s.y
	d.x=1                1          0
	d.y=2                2          0
	d.s.x = d.x*d.x;     4          0
	d.s.y = d.y + d.y;   4          8
	*/

	/*st_task task = {};
	uint64_t a = 0x00010001;
	memcpy(&task,&a,sizeof(uint64_t));
	printf("%11u,%11u,%11u",task.id,task.value,task.timestamp);//1,1,0
	*/

	/*int a = 10, b;
	int* pa = a;
	int* pb = b;
	//&pb = a;error
	pb = &a;
   //pb = &pa;error
	pb = pa;
	*/

	/*int i, a[5] = {1,2,3,4,5};
	f(a,0,4);
	for ( i=0;i<5;i++)
	{
		printf("%d\n",a[i]);//5,4,3,2,1
	}*/
	
	/*int i, j = 0;
	char a[] = " How are you!";
	for (i=0;a[i];i++)
	{
		if (a[i] != ' ')
		{
			a[j++] = a[i];           
		}
		a[j] = '\0';		
	}
	printf("%s\n",a);//Howareyou!
	*/
	/*char a = 'A', b = 'a';
	f2(&b,a);//传指针和传值
	cout<< a<<endl<<b<<endl;//Ab
	*/

	/*char str[] = "ABCD";
	char* p = str;
	printf("%d\n",*(p+4));//'\0'的值是0
	*/
	getchar();
	system("pause");
	return 0;
}

//060 结构体的深拷贝和浅拷贝

浅拷贝：

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct student
{
    char name[32];
    char *_name;
    int age;
};

struct student *Creat_student(int count)
{
    int i=0;
    struct student *stutemp=(struct student *)malloc(count*sizeof(struct student ));
    if(stutemp == NULL)
    {
        printf("分配内存失败！！！\n");
        return NULL;
    }
    for(i = 0; i < count; i++)
    {
        memset(&stutemp[i], 0, sizeof(struct student));
        stutemp[i]._name=(char *)malloc(32);
        memset(stutemp[i]._name, 0, 32);
    }
    return stutemp;
}

void free_student(struct student *stu)
{
    if(stu==NULL)
    {
        printf("stu==NULL");
        return ;
    }
    if(stu->_name != NULL)
    {
        free(stu->_name);
    }
    free(stu);
}

int main()
{
    int num=1;
    struct student *Stu1=NULL;
    struct student *Stu2=NULL;
    Stu1=Creat_student(num);
    Stu2=Creat_student(num);
    Stu1->age=20;
    strcpy(Stu1->name,"321");
    strcpy(Stu1->_name,"123");
    Stu2 = Stu1;//等号操作编译器会执行浅拷贝机械的拷贝变量Stu1中的内容到变量Stu2中
    free_student(Stu1);
    free_student(Stu2);
    //因为是浅拷贝 这样干相当于把同一块内存释放两遍程序会崩溃
    return 0;
}

//在执行Stu2=Stu1后编译器并没有将Stu1所指向的内存空间数据拷贝给Stu2，而是直接将Stu1中所
//存的地址拷贝给Stu2,导致Stu2指向Su1所指的内存空间释放两次自然会导致程序崩溃。
//如果将Stu2=Stu1改为*Stu2=*Stu1，程序同样会崩溃，这是因为Stu1->_name直接指向
//了Stu1->_name 所指向的内存


//深拷贝
void deep_copy(struct student *stu1,struct student *stu2)//深拷贝
{
    memcpy(stu1, stu2, sizeof(struct student ));//先执行浅拷贝拷贝数据
    stu1->_name = (char *)malloc(32);
    memcpy(stu1->_name, stu2->_name, 32);
}
//深拷贝只需将Stu1=Stu2;改为deep_copy(Stu2,Stu1);这样相当于手工给他拷贝了指针所指
//向内存空间的值，程序可以正常运行。






