/*************************************************************************
    > File Name: process.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月02日 14时53分00秒 CST
 ************************************************************************/

#include "common.h"

//管理子进程的数组 
int g_ProcessARrr[PROCESS_NUM];  

int CreateSubProcess()//创建进程
{
    int i;
    for(i=0;i<PROCESS_NUM;i++)
    {
        int pid=fork();
        if(pid==0)//子进程
        {
            return 1;
        }
        else if(pid>0)//父进程
        {
            g_ProcessARrr[i]=pid;
        }
        else//出错
        {
            perror("create subProcess error");
            return -1;
        }
    }
    return 0;
}