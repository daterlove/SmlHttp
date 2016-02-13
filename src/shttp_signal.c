/*************************************************************************
    > File Name: shttp_signal.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月02日 21时10分47秒 CST
 ************************************************************************/
extern int g_ProcessArr[];

#include "common.h"

void signal_exit(int signum)
{
    printf("func:signum=%d\n",signum);
		sleep(2);
        
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
    //signal(SIGTERM, signal_exit);  
    
    return 0;  
}