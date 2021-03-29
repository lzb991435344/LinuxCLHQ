腾讯面经1
1.	TCP三次握手和TCP四次挥手

2.	TCP序列号的作用

3.	TCP滑动窗口，拥塞控制

4.	epoll实现机制，为什么返回是就绪的？与select的区别，为什么select不能做到？select轮训的默认时间是多久？

5.	高并发下有哪些模式？
  

6.	C++多态实现的机制，为什么能实现运行时的多态？
	通过父类指针调用子类的函数，可以让父类指针有多种形态。对于虚函数调用来说，每一个对象内部都
	有一个虚表指针，该虚表指针被初始化为本类的虚表。所以在程序中，不管你的对象类型如何转换，
	但该对象内部的虚表指针是固定的，所以，才能实现动态的对象函数调用。
	总结（基类有虚函数）：
     1、每一个类都有虚表。
     2、虚表可以继承，如果子类没有重写虚函数，那么子类虚表中仍然会有该函数的地址，只不过这个地址指向的是基类的虚函数实现。如果基类3个虚函数，那么基类的虚表中就有三项（虚函数地址），派生类也会有虚表，至少有三项，如果重写了相应的虚函数，那么虚表中的地址就会改变，指向自身的虚函数实现。如果派生类有自己的虚函数，那么虚表中就会添加该项。
     3、派生类的虚表中虚函数地址的排列顺序和基类的虚表中虚函数地址排列顺序相同。
	 
7.	C++存储数据区域，staic存在哪个区？
	1. 内存栈区： 存放局部变量名；
	2. 内存堆区： 存放new或者malloc出来的对象；
	3. 常数区： 存放局部变量或者全局变量的值；
	4. 静态区： 用于存放全局变量或者静态变量；
	5. 代码区：二进制代码。
	
8.	atoi函数使用
  --convert a string to an integer
  int atoi(const char *nptr);
  
  int my_atoi(char* pstr)
{
	int Ret_Integer = 0;
	int Integer_sign = 1;
	
	/*
	* 判断指针是否为空
	*/
	if(pstr == NULL)
	{
		printf("Pointer is NULL\n");
		return 0;
	}
	
	/*
	* 跳过前面的空格字符
	*/
	while(isspace(*pstr) == 0)
	{
		pstr++;
	}
	
	/*
	* 判断正负号
	* 如果是正号，指针指向下一个字符
	* 如果是符号，把符号标记为Integer_sign置-1，然后再把指针指向下一个字符
	*/
	if(*pstr == '-'){
		Integer_sign = -1;
	}
	if(*pstr == '-' || *pstr == '+'){
		pstr++;
	}
	
	/*
	* 把数字字符串逐个转换成整数，并把最后转换好的整数赋给Ret_Integer
	*/
	while(*pstr >= '0' && *pstr <= '9'){
		Ret_Integer = Ret_Integer * 10 + *pstr - '0';
		pstr++;
	}
	Ret_Integer = Integer_sign * Ret_Integer;
	
	return Ret_Integer;
}

10.	平常调试的方法？内存增长怎么判断？用什么调试方式？
10.1	内存错误类别
（1）	内存泄露
//简单的潜在堆内存丢失和缓冲区覆盖
void f1(char *explanation){
    char p1;
    p1 = malloc(100);
        (void) sprintf(p1,
                       "The f1 error occurred because of '%s'.",
                       explanation);
        local_log(p1);
}
//每次f1的调用都会泄露100字节



//来自资源错误管理的潜在堆内存丢失
int getkey(char *filename){
    FILE *fp;
    int key;
 
    fp = fopen(filename, "r");
    fscanf(fp, "%d", &key);
    return key;
}
//缺少fclose();其他资源如信号量，网络句柄，数据库连接等



（2）	错误分配（大量增加free()释放的内存和未初始化的引用）
//未初始化指针
	void f2(int datum){
		int *p2;
 
            /* Uh-oh!  No one has initialized p2. */
        *p2 = datum;
       ...
	}
//对未初始化指针的分配通常会立即导致 segmentation fault 错误。它的好处是任何此类
//错误都会被快速地检测到两个错误的内存释放

/* Allocate once, free twice. */
void f3(){
    char *p;
 
    p = malloc(10);
     ...
        free(p);
     ...
        free(p);
    }
 
    /* Allocate zero times, free once. */
	void f4(){
		char *p;
            /* Note that p remains uninitialized here. */
		free(p);
	}
	
（3）	悬空指针
   void f8() {
	struct x *xp;
 
	xp = (struct x *) malloc(sizeof (struct x));
	xp.q = 13;
	...
	free(xp);
	...
    /* Problem!  There's no guarantee that
   the memory block to which xp points
   hasn't been overwritten. */
	return xp.q;
}
传统的“调试”难以隔离悬空指针。
（1）即使影响提前释放内存范围的代码已本地化，内存的使用仍然可能取决于应用程序甚至
（在极端情况下）不同进程中的其他执行位置。
（2）悬空指针可能发生在以微妙方式使用内存的代码中。结果是，即使内存在释放后立即被覆盖，
  并且新指向的值不同于预期值，也很难识别出新值是错误值。
（4）	数组边界违规
	如果 explanation 的长度超过 80，则会发生什么情况？回答：难以预料，但是它可能与良好情形相差甚远。
	特别是，C 复制一个字符串，该字符串不适于为它分配的 100 个字符。在任何常规实现中，“超过的”字符
	会覆盖内存中的其他数据。内存中数据分配的布局非常复杂并且难以再现，所以任何症状都不可能追溯到
	源代码级别的具体错误
10.2 改正措施
  (1) 编码风格
   /********
 * ...
 *
 * Note that any function invoking protected_file_read()
 * assumes responsibility eventually to fclose() its
 * return value, UNLESS that value is NULL.
 *
 ********/
FILE *protected_file_read(char *filename)
{
    FILE *fp;
 
    fp = fopen(filename, "r");
    if (fp) {
    ...
    } else {
    ...//为空
    }
    return fp;//返回到堆内存中
}
 
    /*******
 * ...
 *
 * Note that the return value of get_message points to a
 * fixed memory location.  Do NOT free() it; remember to
 * make a copy if it must be retained ...
 *
 ********/
char *get_message()
{
    static char this_buffer[400];
 
        ...
    (void) sprintf(this_buffer, ...);
    return this_buffer;//保留，不能free
    }
 
 
    /********
 * ...
 * While this function uses heap memory, and so 
 * temporarily might expand the over-all memory
 * footprint, it properly cleans up after itself.
 * 
 ********/
    int f6(char *item1){
    my_class c1;
    int result;
        ...
    c1 = new my_class(item1);
    ...
        result = c1.x;
    delete c1; //记得delete
    return result;
}
/********
 * ...
 * Note that f8() is documented to return a value
 * which needs to be returned to heap; as f7 thinly
 * wraps f8, any code which invokes f7() must be
 * careful to free() the return value.
 *
 ********/
int *f7()
{
    int *p;
 
    p = f8(...);
    ...
    return p; //返回到堆内存中，记得调用free释放
}
  
  //专用库
  //语言
  //软件工具
  //硬件检查器
  
  (2)检测
    通过少量的实践和适当的文本搜索，您能够快速验证平衡的 *alloc() 和 free() 
	或者 new 和 delete 的源主体
     static char *important_pointer = NULL;
	void f9()
	{
		if (!important_pointer) 
		important_pointer = malloc(IMPORTANT_SIZE);
			...
		if (condition)
			/* Ooops!  We just lost the reference 
			   important_pointer already held. */
		important_pointer = malloc(DIFFERENT_SIZE);
			...
    }
	//如果 condition 为真，简单使用自动运行时工具不能检测发生的内存泄漏
	
   (3)静态的自动语法分析
    静态语法分析 成为开发流程的一部分。静态语法分析是 lint、严格编译 和几种商业产品执
	行的内容：扫描编译器接受的源文本和目标项，但这可能是错误的症状。
	希望让您的代码无 lint。尽管 lint 已过时，并有一定的局限性，但是，没有使用它（或其
	较高级的后代）的许多程序员犯了很大的错误
   (4) 内存库
     库可以解决多种不同的内存问题，以致于直接对它们进行比较是非常困难的；这方面的常见主题
	 包括垃圾收集、智能指针 和 智能容器。大体上说，库可以自动进行较多的内存管理 
   (5) 内存工具
     基于软件的内存工具。还有硬件内存调试器；在非常特殊的情况下（主要是在使用不支持其他
	 工具的专用主机时）才考虑它们。
	市场上的软件内存工具包括专有工具（如 IBM Rational® Purify 和 Electric Fence）和其他
	开放源代码工具。其中有许多可以很好地与 AIX 和其他操作系统一起使用。
     所有内存工具的功能基本相同：构建可执行文件的特定版本（很像在编译时通过使用 -g 标记生成
     的调试版本）、练习相关应用程序和研究由工具自动生成的报告
	 
11.	虚拟内存寻址空间大小
    32位下为4G,Linux下0-3G是应用层，3G-4G是内核层
	
12.	浏览器输入url发生什么？
	
	
	
13.	Cpu调度的基本单位，进程和线程的区别
线程。
执行一段程序代码，实现一个功能的过程介绍 ，当得到CPU的时候，相关的资源必须也已经就位，就是显卡啊，
GPS啊什么的必须就位，然后CPU开始执行。这里除了CPU以外所有的就构成了这个程序的执行环境，也就是我们
所定义的程序上下文。当这个程序执行完了，或者分配给他的CPU执行时间用完了，那它就要被切换出去，等待
下一次CPU的临幸。在被切换出去的最后一步工作就是保存程序上下文，因为这个是下次他被CPU临幸的运行环境。
	先加载程序A的上下文，然后开始执行A，保存程序A的上下文，调入下一个要执行的程序B的程序上下文，
	然后开始执行B,保存程序B的上下文。
进程就是包换上下文切换的程序执行时间总和 = CPU加载上下文+CPU执行+CPU保存上下文
14.	40亿个QQ号码如何存储？
	
15.	平时关注的库


美团面经
16.	Tcp为什么是3次，而不是4次？

17.	Tcp和udp的区别，攻击

18.	Tcp的粘包和分包

19.	IO的模型有哪些？

用户态和内核态的交互



4.15 面试
1.	可变长的结构体怎么定义？
（1）	使用指针
typedef struct _S_HB_TIME_REPORT_INFO{
     uint16_t msg_id;
     uint16_t msg_buf_len;
     char *p_msg_buf;
}__attribute__((packed)) S_HB_TIME_REPORT_INFO;
成员变量p_msg_buf为一个指针，指向一个不确定长度的字符串，长度由msg_buf_len决定。在使用时可以动态给p_msg_buf分配msg_buf_len的内存，也可以p_msg_buf指向一个已知地址的字符串。
缺点：
1）动态分配内存使用malloc，在使用之后容易忘记free，此时会产生内存泄漏。 
2）如果你使用S_HB_TIME_REPORT_INFO结构体的次数比较多，那么就要malloc很多次，此时很容易产生内存碎片。我的建议是，能不使用malloc就尽量不要用malloc。我的通常做法是让p_msg_buf指向一个已知地址的字符串。 
3）不管是malloc分配内存还是静态指向一个已知地址的字符串，结构体里面的成员变量char *p_msg_buf是与结构体分离的，不利于操作。

（2）	柔性数组
柔性数组既数组大小待定的数组， C语言中结构体的最后一个元素可以是大小未知的数组，也就是所谓的0长度，所以我们可以用结构体来创建柔性数组。在一个结构体的最后 ，申明一个长度为空的数组，就可以使得这个结构体是可变长的。对于编译器来说，此时长度为0的数组并不占用空间，因为数组名本身不占空间，它只是一个偏移量， 数组名这个符号本身代 表了一个不可修改的地址常量 （注意：数组名永远都不会是指针！ ），但对于这个数组的大小，我们可以进行动态分配,对于编译器而言，数组名仅仅是一个符号，它不会占用任何空间，它在结构体中，只是代表了一个偏移量，代表一个不可修改的地址常量！对于柔性数组的这个特点，很容易构造出变成结构体，如缓冲区，数据包等等。



typedef struct _S_HB_TIME_REPORT_INFO
{
     uint16_t msg_id;
     uint16_t msg_buf_len;
     char msg_buf[0];
 }__attribute__((packed)) S_HB_TIME_REPORT_INFO;
1.	strcpy和memcpy的底层实现
strcpy提供了字符串的复制。即strcpy只用于字符串复制，并且它不仅复制字符串内容之外，还会复制字符串的结束符。memcpy提供了一般内存的复制。即memcpy对于需要复制的内容没有限制，因此用途更广。
// 实现src到dest的复制
char * strcpy(char * dest, const char * src) {
//判断参数src和dest的有效性
　　if ((src == NULL) || (dest == NULL)) {
　　　　　　return NULL;
　　}
　　char *strdest = dest;        //保存目标字符串的首地址
　　while ((*strDest++ = *strSrc++)!='\0'); //把src字符串的内容复制到dest下
　　return strdest;
}
void *memcpy(void *memTo, const void *memFrom, size_t size){
//memTo和memFrom必须有效
　　if((memTo == NULL) || (memFrom == NULL)) 
         return NULL;

		//保存memFrom首地址
　　char *tempFrom = (char *)memFrom;  

    //保存memTo首地址
　　char *tempTo = (char *)memTo;  

   //循环size次，复制memFrom的值到memTo中                    
　　while(size -- > 0)                
       　　*tempTo++ = *tempFrom++ ;  
　　return memTo;
}
strcpy和memcpy主要有以下3方面的区别。
1、复制的内容不同。strcpy只能复制字符串，而memcpy可以复制任意内容，例如字符数组、整型、结构体、类等。
2、复制的方法不同。strcpy不需要指定长度，它遇到被复制字符的串结束符"\0"才结束，所以容易溢出。memcpy则是根据其第3个参数决定复制的长度。
3、用途不同。通常在复制字符串时用strcpy，而需要复制其他类型数据时则一般用memcpy

2.	汇编指令
CPU内部又包括一种叫做内部寄存器的东西，包括
数据寄存器： AX,BX,CX,DX；
段寄存器： CS,DS,ES,SS；
指针与变址寄存器SP,BP,SI,DI；----堆栈指针寄存器sp
指令指针与标志寄存器IP,FLAGS。
3.	Memcpy拷贝结构体数组
    
4.	gets()函数使用问题
//本函数可以无限读取，易发生溢出。如果溢出，多出来的字符将被写入到堆栈中，
//这就覆盖了堆栈原先的内容，破坏一个或多个不相关变量的值
5.	结构体大小计数
//偏移量是指结构体变量中成员的地址和结构体变量的地址的差。第一个成员的偏移量为0，
//第二个成员的偏移量为第一个成员的偏移量加上第一个成员的大小，依次类推。。。。
（1）无结构体嵌套的情况
        结构体的大小为最后一个成员的偏移量+其所占的字节数，除了这个准则外，还需要满足以下的两个原则：
       1：结构体中成员的偏移量必须是该成员所在字节数的整数倍。
       2：结构体的大小必须是所有成员的整数倍，即最小公倍数。。
（2）有结构体嵌套的情况
       需要将嵌套的结构体展开，并且被展开结构体的第一个成员变量的偏移量必须为这个被展开结构体中所占最大字节成员的整数倍。并符合上述1：2：的原则。
（3）含有union
union   mm{   
  char   a;//元长度1   
  int   b[5];//元长度4   
  double   c;//元长度8   
  int   d[3]; //元长度4
  };   
//本来mm的空间应该是sizeof(int)*5=20;但是如果只是20个单元的话,那可以存几个double型(8位)呢?
//两个半?当然不可以,所以mm的空间延伸为既要大于20,又要满足其他成员所需空间的整数倍,,
//因为含有double元长度8，故大小为24。
struct stu5 { 
     char i; 
     struct  { 
          char c; 
          int j; 
     } ss;  
     char a; 
     char b; 
     char d; 
     char e; 
     char f; 
};

