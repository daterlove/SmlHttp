/*************************************************************************
    > File Name: shttp_timer.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月23日 20时43分03秒 CST
 ************************************************************************/

#include "common.h"
void sleep_over(int signum)
{
    #ifdef DEBUG  
    printf("Sleep over\n");
    #endif 
    return;
}
//设置秒和微秒后，阻塞程序
void timer_sleep(int sec,int usec)
{
   struct itimerval value, ovalue;
   signal(SIGALRM, sleep_over);
   value.it_value.tv_sec = sec;
   value.it_value.tv_usec = usec;
   value.it_interval.tv_sec = 0;
   value.it_interval.tv_usec = 0;
   setitimer(ITIMER_REAL, &value, &ovalue); //(2)
   pause(); 
}

void timer_record_log(int sec)
{
    char time_buf[256];
    char file_buf[256];
    alarm(sec);
    format_time(time_buf);
    sprintf(file_buf,"\n[时间：%s]\n",time_buf);
    file_write(LOG_NAME,file_buf);
    pause();
    signal_record(); 
}