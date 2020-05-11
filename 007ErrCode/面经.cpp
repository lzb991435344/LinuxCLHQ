腾讯面经1
1.	TCP三次握手和TCP四次挥手

2.	TCP序列号的作用

3.	TCP滑动窗口，拥塞控制

4.	epoll实现机制，为什么返回是就绪的？与select的区别，为什么select不能做到？select轮训的默认时间是多久？

5.	高并发下有哪些模式？

6.	C++多态实现的机制，为什么能实现运行时的多态？
通过父类指针调用子类的函数，可以让父类指针有多种形态。对于虚函数调用来说，每一个对象内部都有一个虚表指针，该虚表指针被初始化为本类的虚表。所以在程序中，不管你的对象类型如何转换，但该对象内部的虚表指针是固定的，所以，才能实现动态的对象函数调用。
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

9.	常用的stl函数的实现


10.	平常调试的方法？内存增长怎么判断？用什么调试方式？
10.1	内存错误类别
（1）	内存泄露
简单的潜在堆内存丢失和缓冲区覆盖
void f1(char *explanation){
    char p1;
    p1 = malloc(100);
        (void) sprintf(p1,
                       "The f1 error occurred because of '%s'.",
                       explanation);
        local_log(p1);
}
每次f1的调用都会泄露100字节

来自资源错误管理的潜在堆内存丢失
int getkey(char *filename){
    FILE *fp;
    int key;
 
    fp = fopen(filename, "r");
    fscanf(fp, "%d", &key);
    return key;
}
缺少fclose();其他资源如信号量，网络句柄，数据库连接等

（2）	错误分配（大量增加free()释放的内存和未初始化的引用）
未初始化指针
void f2(int datum){
    int *p2;
 
            /* Uh-oh!  No one has initialized p2. */
        *p2 = datum;
       ...
}
对未初始化指针的分配通常会立即导致 segmentation fault 错误。它的好处是任何此类错误都会被快速地检测到

两个错误的内存释放

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
（1）即使影响提前释放内存范围的代码已本地化，内存的使用仍然可能取决于应用程序甚至（在极端情况下）不同进程中的其他执行位置。
（2）悬空指针可能发生在以微妙方式使用内存的代码中。结果是，即使内存在释放后立即被覆盖，并且新指向的值不同于预期值，也很难识别出新值是错误值。
（4）	数组边界违规
如果 explanation 的长度超过 80，则会发生什么情况？回答：难以预料，但是它可能与良好情形相差甚远。特别是，C 复制一个字符串，该字符串不适于为它分配的 100 个字符。在任何常规实现中，“超过的”字符会覆盖内存中的其他数据。内存中数据分配的布局非常复杂并且难以再现，所以任何症状都不可能追溯到源代码级别的具体错误
10.2	
11.	虚拟内存寻址空间大小

12.	浏览器输入url发生什么？

13.	Cpu调度的基本单位，进程和线程的区别
线程。
执行一段程序代码，实现一个功能的过程介绍 ，当得到CPU的时候，相关的资源必须也已经就位，就是显卡啊，GPS啊什么的必须就位，然后CPU开始执行。这里除了CPU以外所有的就构成了这个程序的执行环境，也就是我们所定义的程序上下文。当这个程序执行完了，或者分配给他的CPU执行时间用完了，那它就要被切换出去，等待下一次CPU的临幸。在被切换出去的最后一步工作就是保存程序上下文，因为这个是下次他被CPU临幸的运行环境。
	先加载程序A的上下文，然后开始执行A，保存程序A的上下文，调入下一个要执行的程序B的程序上下文，然后开始执行B,保存程序B的上下文。
进程就是包换上下文切换的程序执行时间总和 = CPU加载上下文+CPU执行+CPU保存上下文
14.	40亿个QQ号码如何存储？

15.	平时关注的库

 

美团面经
16.	Tcp为什么是3次，而不是4次？

17.	Tcp和udp的区别，攻击

18.	Tcp的粘包和分包

19.	IO的模型有哪些？


用户态和内核态的交互
