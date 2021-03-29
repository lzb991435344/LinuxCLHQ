#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define TIMESTRSIZE 1024
#define FMTSIZE 1024

//命令行分析 getopt() getopt_long()


/*
* -y:year
* -m:month
* -d:day
* -H:hour
* -M:minute
* -S:second
**/
//0x00  ./mydate -MSmd 
//0x01  ./mydate -M -S -m -d  -y 4or2  -H 12or24
//0x02 ./mydate   -M -S -m -d  -y 4  -H 24  /tmp/out

//0x03  ./mydate  /tmp/out -M -S -m -d /tmp/out1 -y 4  -H 24  /tmp/out2
// /tmp/out has data
int main(int argc, char* argv[]){

   char timestr[TIMESTRSIZE];
   time_t stamp;
   struct tm* tm;
   char c;
   char fmtstr[FMTSIZE];//字符数组存放
   FILE* fp = stdout;

   fmtstr[0]='\0';

   stamp = time(NULL);
   tm = localtime(&stamp);

   while(1){
      // void exit(int status);
     
     //带参数的选项，在选项之后加上冒号
     // H 和 y 能够使用，全局指针指向该处，添加了冒号之后

      //加-号：遇到非选项的参数时都返回1值
      c = getopt(argc, argv, "-H:MSy:md");

      if(c < 0){
         break;
      }
      switch(c){
         //not option, why 1? check man 
         //重定向到文件中，而不是终端，这里是stdout
         case 1:

         //当有多个文件的时候，有几种策略，先入为主，后入为主，都写入
         //下面实现是先入为主的时候，测试命令如上，含有多个文件，以第一个
         //文件为输入点
         if(fp == stdout){
            //找到第一个非选项传参之后，指针指向了下一个位置，即optind - 1 位置
            fp = fopen(argv[optind - 1],"w");
            if(fp == NULL){
               perror("fopen()");
               fp = stdout;
            }
         }
         break;


         case 'H':
         if(strcmp(optarg, "12") == 0){
             strncat(fmtstr, "%I(%P)", FMTSIZE);//P上午还是下午
         }else if(strcmp(optarg, "24") == 0){
             strncat(fmtstr, "%H ", FMTSIZE);
         }else{
             fprintf(stderr, "Inavalid argument\n");
         }
         break;


         case 'M':
         strncat(fmtstr,"%M ", FMTSIZE);
         break;


         case 'S':
         strncat(fmtstr, "%S ", FMTSIZE);
         break;


         case 'y':
         if(strcmp(optarg, "2") == 0){
            strncat(fmtstr,"%y ", FMTSIZE);
         }else if(strcmp(optarg, "4") == 0){
            strncat(fmtstr,"%Y ", FMTSIZE);
         }else{
            fprintf(stderr, "Invalid argument of -y\n");
         }
         break;


         case 'm':
         strncat(fmtstr, "%m ", FMTSIZE);
         break;


         case 'd':
         strncat(fmtstr, "%d ", FMTSIZE);
         break;


         default:
            break;
      }
   }
   strncat(fmtstr,"\n", FMTSIZE);
   // strftime - format date and time
   //size_t strftime(char *s, size_t max, const char *format,
                    //   const struct tm *tm);

   strftime(timestr, TIMESTRSIZE, fmtstr, tm);
   fputs(timestr, fp);//往流输出字符或字符串

   //not in stdout then fclose(fp)
   //在写完成文件之后，关闭fp
   if(fp != stdout){
      fclose(fp);
   }
   
   exit(0);
}