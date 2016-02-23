/*************************************************************************
    > File Name: shttp_signal.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月02日 21时10分47秒 CST
 ************************************************************************/
extern int g_ProcessArr[];
extern int g_connect_count;
extern int g_maxfd;

#include "common.h"

void signal_record_handlde(int signum)
{
    char buf[256];
    sprintf(buf,"[PID:%d,并发数:%d,最大监听fd:%d]\n",getpid(),g_connect_count,g_maxfd);
    file_write(LOG_NAME,buf);
}
void signal_alarm_handlde(int signum)
{
    return;
}

//----------------------------------------------
void signal_record()
{
    #ifdef DEBUG
    printf("发送记录日志信号to子进程\n");
    #endif
    int i;
    //发送给子进程记录日志信号
    for(i=0;i<PROCESS_NUM;i++)
    {
        kill(g_ProcessArr[i], SIGUSR1); 
    }
}
void signal_exit(int signum)
{     
    int i;
    //发送给子进程退出信号 
    for(i=0;i<PROCESS_NUM;i++)
    {
        printf("kill pid:%d\n",g_ProcessArr[i]);
      //  kill(g_ProcessArr[i], SIGTERM); 
    }
    pause();
  //  exit(0);  
}
int signal_init()
{
    //忽略该信号，对已关闭的socket两次write会产生该信号
    //如果不加此句，程序遇到此信号 会奔溃
    signal(SIGPIPE,SIG_IGN);
    signal(SIGUSR1,&signal_record_handlde); 
    
    return 0;  
}