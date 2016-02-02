/*************************************************************************
    > File Name: main.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时58分54秒 CST
 ************************************************************************/

#include "common.h"
int main(int arg,char **argv)
{
    int ret;
    
    int listenfd=connect_socket_start();
    if (listenfd<0)
    {
        return -1;
    }
	printf("listenfd:%d\n",listenfd);
    
    ret=CreateSubProcess();
    if(0==ret)//父进程
    {
        printf("parent process\n");
    }
    else if(1==ret)//子进程
    {
        printf("sub process\n");
    }
    else//发生错误
    {
        return -1;
    }
    
    printf("Server Close\n");
	return 0;
}
