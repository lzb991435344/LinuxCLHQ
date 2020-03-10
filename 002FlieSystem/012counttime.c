#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define TIMESTRSIZE 1024


int main(){

   char timestr[TIMESTRSIZE];
   time_t stamp;
   struct tm* tm;


   stamp = time(NULL);
   tm = localtime(&stamp);
   strftime(timestr, TIMESTRSIZE, "Now:%Y-%m-%d", tm);
   puts(timestr);

   tm->tm_mday += 100;
   (void)mktime(tm);

   strftime(timestr, TIMESTRSIZE, "100days later:%Y-%m-%d", tm);
   puts(timestr);
   exit(0);
}