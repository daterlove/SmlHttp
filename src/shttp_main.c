/*************************************************************************
    > File Name: main.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时58分54秒 CST
 ************************************************************************/

#include "common.h"
int main(int arg,char **argv)
{
    int listenfd=connect_socket_start();
    if (listenfd<0)
    {
        return -1;
    }
	printf("listenfd:%d\n",listenfd);
	return 0;
}
