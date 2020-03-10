#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define TIMESTRSIZE 1024
#define FMTSIZE 1024
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
   char fmtstr[FMTSIZE];
   FILE* fp = stdout;

   fmtstr[0]='\0';

   stamp = time(NULL);
   tm = localtime(&stamp);
   while(1){
      c = getopt(argc, argv, "-H:MSy:md");
      if(c < 0){
         break;
      }
      switch(c){
         //not option, why 1? check man 
         case 1:
         if(fp == stdout){
            fp = fopen(argv[optind - 1],"w");
            if(fp == NULL){
               perror("fopen()");
               fp = stdout;
            }
         }
            break;
         case 'H':
         if(strcmp(optarg, "12") == 0){
             strncat(fmtstr, "%I(%P)", FMTSIZE);
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
   strftime(timestr, TIMESTRSIZE, fmtstr, tm);
   fputs(timestr, fp);

   //not in stdout then fclose(fp)
   if(fp != stdout){
      fclose(fp);
   }
   
   exit(0);
}