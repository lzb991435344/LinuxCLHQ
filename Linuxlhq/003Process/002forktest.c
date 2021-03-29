#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
 
 
/** 0x00
//result printf 8 "-"
int main(void)
{
   int i;
   for(i=0; i<2; i++){
      fork();
      printf("-");
   }
 
   wait(NULL);
   wait(NULL);
 
   return 0;
}*/


/*0x01
//result printf 6 "-"
int main(void)
{
   int i;
   for(i=0; i<2; i++){
      fork();
      //printf("-\n");
      
      printf("-");
      fflush(stdout);   
   }
   //程序遇到“\n”，或是EOF，或是缓中区满，或是文件描述符关闭，
   //或是主动flush，或是程序退出，就会把数据刷出缓冲区。需要注意的是，
   //标准输出是行缓冲，所以遇到“\n”的时候会刷出缓冲区，但对于磁盘这个块设备来说，
   //“\n”并不会引起缓冲区刷出的动作，那是全缓冲，你可以使用setvbuf来设置缓冲区大小，
   //或是用fflush刷缓存。
 
   wait(NULL);
   wait(NULL);
 

   return 0;
}*/

/**
//result printf 8 "-"
int main(void)
{
   int i;
   for(i=0; i<2; i++){
      fork();
      //printf("-\n");
      
      printf("-");
      //fflush(stdout);   
   }
 	
   fflush(stdout);   
   wait(NULL);
   wait(NULL);
   //fflush(stdout); 

   return 0;
}*/

//for test

/**
int main(void)
{
   int i;
   for(i = 0; i < 2; i++){
      fork();
      //注意：下面的printf有“\n”
      printf("ppid=%d, pid=%d, i=%d \n", getppid(), getpid(), i);
   }
   sleep(20); //让进程停留十秒，这样我们可以用pstree查看一下进程树
   return 0;
}*/

/**
ppid=2536, pid=13386, i=0 
ppid=13386, pid=13387, i=0 
ppid=13386, pid=13387, i=1 
ppid=13387, pid=13388, i=1 
ppid=2536, pid=13386, i=1 
ppid=13386, pid=13389, i=1 

*/
//pstree -p | grep fork


int main(void)
{
   int i;
   //fflush(NULL); //8 "-"
   for(i=0; i<2; i++){
      //fflush(stdout); // 6 "-"
      fflush(NULL);  // 6 "-"
      fork();
      //printf("-\n");
      
      printf("-");
      //fflush(stdout);   
   }
   
   //fflush(stdout);   
   wait(NULL);
   wait(NULL);
   //fflush(stdout); 

   return 0;
}