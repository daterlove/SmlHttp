/*************************************************************************
    > File Name: shttp_signal.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月02日 21时10分47秒 CST
 ************************************************************************/
extern int g_ProcessArr[];

#include "common.h"

void signal_exit()
{
    int i;
    //发送给子进程退出信号 
    for(i=0;i<PROCESS_NUM;i++)
    {
        printf("kill pid:%d\n",g_ProcessArr[i]);
      //  kill(g_ProcessArr[i], SIGTERM); 
    }
  //  exit(0);  
}
int signal_init()
{
    signal(SIGTERM, signal_exit);  
    return 0;  
}