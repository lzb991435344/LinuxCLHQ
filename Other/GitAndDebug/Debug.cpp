
linux基础，程序构建及基础工具
https://linuxtools-rst.readthedocs.io/zh_CN/latest/base/index.html


1.程序构建
1.1 配置
(1)查询可用配置选项
 	./configure --help
 (2)配置路径，–prefix是配置使用的最常用选项，设置程序安装的路径
 	./configure --prefix=/usr/local/snmp

1.2 Makefile 
1.2.1 编写要点
	必须满足第一条规则，满足后停止
	除第一条规则，其他无顺序
1.2.2 全局自变量
	$@目标文件名
	@^所有前提名，除副本
	@＋所有前提名，含副本
	@＜一个前提名
	@？所有新于目标文件的前提名
	@*目标文件的基名称



(1)生成core定位错误位置
//core find seg err
//1. Makefile + -g
//2.ulimit -c unlimited //解除生成core的限制
//3. ./servertest -F  //运行生成core文件
//4.gdb servertest core or gdb -q servertest core //gdb core文件 得到结果，参数-q
//6. bt

安装 makeinstall


1.3 Cmake 

博客位置：
https://blog.csdn.net/zhuiyunzhugang/article/details/88142908

1.3.1 单个源文件
(1)编写 CMakeLists.txt

	# CMake 最低版本号要求，指定运行此配置文件所需的 CMake 的最低版本;
	cmake_minimum_required (VERSION 2.8)

	# 项目信息,参数值是 Demo1，该命令表示项目的名称是 Demo1 
	project (Demo1)

	# 指定生成目标, 将名为 main.cc 的源文件编译成一个名称为 Demo 的可执行文件。
	add_executable(Demo main.cc)


(2)编译项目
	在当前目录执行 cmake . ，得到 Makefile 后再使用 make 命令编译得到 Demo1 可执行文件。
   cmake . + make 

1.3.2 同一目录，多个源文件
	V1:CMakeLists.txt
	# CMake 最低版本号要求
	cmake_minimum_required (VERSION 2.8)
	# 项目信息
	project (Demo2)
	# 指定生成目标
	add_executable(Demo main.cc MathFunctions.cc)



	V2:使用变量 CMakeLists.txt
	# CMake 最低版本号要求
	cmake_minimum_required (VERSION 2.8)
	# 项目信息
	project (Demo2)
	# 查找当前目录下的所有源文件
	# 并将名称保存到 DIR_SRCS 变量
	aux_source_directory(. DIR_SRCS)

	# 指定生成目标
	add_executable(Demo ${DIR_SRCS})


1.3.3 多个目录，多个源文件
目录结构如下：
	./Demo3
|
+--- main.cc
|
+--- math/
|
+--- MathFunctions.cc
|
+--- MathFunctions.h


CMakeLists.txt
	# CMake 最低版本号要求
	cmake_minimum_required (VERSION 2.8)
	# 项目信息
	project (Demo3)
	# 查找当前目录下的所有源文件
	# 并将名称保存到 DIR_SRCS 变量
	aux_source_directory(. DIR_SRCS)
	# 添加 math 子目录
	add_subdirectory(math)
	# 指定生成目标
	add_executable(Demo main.cc)
	# 添加链接库
	target_link_libraries(Demo MathFunctions)


子目录中的 CMakeLists.txt：
	# 查找当前目录下的所有源文件
	# 并将名称保存到 DIR_LIB_SRCS 变量
	aux_source_directory(. DIR_LIB_SRCS)

	# 生成链接库，静态库
	add_library (MathFunctions ${DIR_LIB_SRCS})





2.程序调试
2.1 进程调试
	(1)gdb调试
	$gdb programmer     # 启动gdb
	>break main         # 设置断点
	>run                # 运行调试程序
	>next               # 单步调试
	>print var1         # 在调试过程中，我们需要查看当前某个变量值的时候，使用print 命令打印该值
	>list               # 显示当前调试处的源代码
	>info b             # 显示当前断点设置情况

	(2)pstack 跟踪栈空间    18177384904
	$pstrack <program-pid>

	$ pstack 4551
	Thread 7 (Thread 1084229984 (LWP 4552)):
	#0  0x000000302afc63dc in epoll_wait () from /lib64/tls/libc.so.6
	#1  0x00000000006f0730 in ub::EPollEx::poll ()
	#2  0x00000000006f172a in ub::NetReactor::callback ()
	#3  0x00000000006fbbbb in ub::UBTask::CALLBACK ()
	#4  0x000000302b80610a in start_thread () from /lib64/tls/libpthread.so.0
	#5  0x000000302afc6003 in clone () from /lib64/tls/libc.so.6
	#6  0x0000000000000000 in ?? ()

	(3)strace 分析系统调用 
	strace可以跟踪到一个进程产生的系统调用,包括参数，返回值，执行消耗的时间。

	$strace -p <process-pid>
		strace -o output.txt -T -tt -e trace=all -p 28979
	跟踪28979进程的所有系统调用（-e trace=all），并统计系统调用的花费时间，以及开始
	时间（以可视化的时分秒格式显示），最后将记录结果存在output.txt文件里面。


2.2 目标文件分析
	(1)nm
	 用来列出目标文件的符号清单。

	 $nm 005main
0000000000201010 B __bss_start
0000000000201010 b completed.7697
                 w __cxa_finalize@@GLIBC_2.2.5
0000000000201000 D __data_start
0000000000201000 W data_start
00000000000005f0 t deregister_tm_clones
0000000000000680 t __do_global_dtors_aux
0000000000200db0 t __do_global_dtors_aux_fini_array_entry
0000000000201008 D __dso_handle
0000000000200db8 d _DYNAMIC
0000000000201010 D _edata
0000000000201018 B _end
                 U exit@@GLIBC_2.2.5
00000000000007c4 T _fini
00000000000006c0 t frame_dummy
0000000000200da8 t __frame_dummy_init_array_entry
0000000000000924 r __FRAME_END__
0000000000200fa8 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
00000000000007e0 r __GNU_EH_FRAME_HDR
0000000000000558 T _init
0000000000200db0 t __init_array_end
0000000000200da8 t __init_array_start
00000000000007d0 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
00000000000007c0 T __libc_csu_fini
0000000000000750 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
00000000000006ca T main
                 U printf@@GLIBC_2.2.5
                 U puts@@GLIBC_2.2.5
0000000000000630 t register_tm_clones
00000000000005c0 T _start
0000000000201010 D __TMC_END__

 (2)objdump
 	objdump工具用来显示二进制文件的信息，就是以一种可阅读的格式让你更多地了解
 	二进制文件可能带有的附加信息。
 
 	objdump -d 005main  用于研究编译器和汇编器的输出,细节信息

 (3)readelf
 	ELF Header 为该文件中所有段入口显示了详细的摘要
 	将调试信息放入到目标文件中，并且还可以显示这些信息
 	$readelf --debug-dump a.out | more

 (4)size 查看程序内存占用
   size 005main
   text	   data	    bss	    dec	    hex	filename
   1754	    616	      8	   2378	    94a	005main
 (5)file 查看文件类型
  file 005main
  005main: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), 
  dynamically linked, interpreter /lib64/l, for GNU/Linux 3.2.0,
   BuildID[sha1]=0faf5bc9dd187e0b860bb3226130b3ccfcdb87ab, not stripped

   查看core文件是由那个文件生成的
   $file core.22355


  (6)strings 查询数据中的文本信息
   $strings <objfile>

  (7)fuser 显示文件使用者
  示所有正在使用着指定的file, file system 或者 sockets的进程信息;
  $fuser -m -u redis-server
	redis-server: 11552rce(weber) 22912rce(weber) 25501rce(weber)
  使用了-m和-u选项，用来查找所有正在使用redis-server的所有进程的PID以及该进程的OWNER；

fuser通常被用在诊断系统的”resource busy”问题。如果你希望kill所有正在使用某一指定的file, file system or sockets的进程的时候，
你可以使用-k选项:

 $fuser –k /path/to/your/filename

 (8)xxd 以十六进制方式显示文件，只显示文本信息
   xxd 005main
 (9)od 
   通常使用od命令查看特殊格式的文件内容。通过指定该命令的不同选项可以以十进制、八进制、十六进制和ASCII码来显示文件。
 
 参数说明：

-A 指定地址基数，包括：

d 十进制
o 八进制（系统默认值）
x 十六进制
n 不打印位移值
-t 指定数据的显示格式，主要的参数有：

c ASCII字符或反斜杠序列
d 有符号十进制数
f 浮点数
o 八进制（系统默认值为02）
u 无符号十进制数
x 十六进制数
除了选项c以外的其他选项后面都可以跟一个十进制数n，指定每个显示值所包含的字节数。




2.3 性能优化
(1)分析系统瓶颈
top
进入交互模式后:
	输入M，进程列表按内存使用大小降序排序，便于我们观察最大内存使用者使用有问题（检测内存泄漏问题）;
	输入P，进程列表按CPU使用大小降序排序，便于我们观察最耗CPU资源的使用者是否有问题；
top第三行显示当前系统的，其中有两个值很关键:
	%id：空闲CPU时间百分比，如果这个值过低，表明系统CPU存在瓶颈；
	%wa：等待I/O的CPU时间百分比，如果这个值过高，表明IO存在瓶颈；

(2)分析内存瓶颈
	
	free
	系统实际可用的内存为free工具输出第二行的free+buffer+cached
(3)分析IO瓶颈
	top:IO存在性能瓶颈，top工具中的%wa会偏高
	iostat: /root$iostat -d -x -k 1 1

	如果%iowait的值过高，表示硬盘存在I/O瓶颈。
	如果 %util 接近 100%，说明产生的I/O请求太多，I/O系统已经满负荷，该磁盘可能存在瓶颈。
	如果 svctm 比较接近 await，说明 I/O 几乎没有等待时间；
	如果 await 远大于 svctm，说明I/O 队列太长，io响应太慢，则需要进行必要优化。
	如果avgqu-sz比较大，也表示有大量io在等待。
(4)分析进程调用
	pstack and pstrace
	pstack用来跟踪进程栈，这个命令在排查进程问题时非常有用，比如我们发现一个服务一直
	处于work状态（如假死状态，好似死循环），使用这个命令就能轻松定位问题所在；可以
	在一段时间内，多执行几次pstack，若发现代码栈总是停在同一个位置，那个位置就需要
	重点关注，很可能就是出问题的地方；

	查看bash程序进程栈:
	$ps -fe| grep bash
	$pstack + 进程号


	strace 跟踪系统调用
(5)优化代码程序

	gprof使用步骤
1.用gcc、g++、xlC编译程序时，使用-pg参数，如：g++ -pg -o test.exe test.cpp编译器会自
动在目标代码中插入用于性能测试的代码片断，这些代码在程序运行时采集并记录函数的调用关
系和调用次数，并记录函数自身执行时间和被调用函数的执行时间。
2.执行编译后的可执行程序，如：./test.exe。该步骤运行程序的时间会稍慢于正常编译的可执
行程序的运行时间。程序运行结束后，会在程序所在路径下生成一个缺省文件名为gmon.out的
文件，这个文件就是记录程序运行的性能、调用关系、调用次数等信息的数据文件。
3.使用gprof命令来分析记录程序运行信息的gmon.out文件，如：gprof test.exe gmon.out
则可以在显示器上看到函数调用相关的统计、分析信息。上述信息也可以采用
gprof test.exe gmon.out> gprofresult.txt重定向到文本文件以便于后续分析。



4.基本工具
(1)ldd 用来查看程式运行所需的共享库,常用来解决程式因缺少某个库文件而不能运行的一些问题。
$ldd 005main
	linux-vdso.so.1 (0x00007ffdba125000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f03ccf24000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f03cd517000)


	第一列：程序需要依赖什么库
	第二列: 系统提供的与程序需要的库所对应的库
	第三列：库加载的开始地址

(2)lsof 文件相关
	

(3)ipcs 查询进程间通信状态
 (i) 查看系统使用的IPC资源
  	$ipcs -m 查看系统使用的IPC共享内存资源
	$ipcs -q 查看系统使用的IPC队列资源
	$ipcs -s 查看系统使用的IPC信号量资源
 (ii)



 (4)vmstat 可实时动态监视操作系统的虚拟内存、进程、CPU活动。
	

 (5)sar 系统活动情况报告


 (6)readelf ELF
 (i)可重定位的对象文件(Relocatable file)
	由汇编器汇编生成的 .o 文件
(ii)可执行的对象文件(Executable file)
	可执行应用程序
(iii)可被共享的对象文件(Shared object file)
	动态库文件，也即 .so 文件

 (7)wget 文件下载

 (8)scp 跨机远程复制

 (9)crontab 定时任务


















