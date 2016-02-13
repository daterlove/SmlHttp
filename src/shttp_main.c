/*************************************************************************
    > File Name: shttp_main.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时58分54秒 CST
 ************************************************************************/

#include "common.h"


int main(int arg,char **argv)
{
    int ret;
    
    //初始化信号处理
    signal_init();
    //开始监听
    int listenfd=socket_listen();
    if (listenfd<0)
    {
        return -1;
    }
	printf("listenfd:%d\n",listenfd);
    
    /*
    //创建子进程
    ret=process_CreateSub();
    if(0==ret)//父进程
    {
        pause();
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
    */
    //int sockfd;
    int epollfd,fds;
    struct epoll_event events[MAX_EVENTS];//epoll触发事件
  //  int rv;
	//struct sockaddr_in client;//客户端地址
	//int len=sizeof(struct sockaddr_in);
    
    //创建epoll句柄,并加入监听套接字
    epollfd=epoll_init(listenfd);
  
    int i;
	while(1)
	{
		//通常需要超时处理
		fds=epoll_wait(epollfd,events,MAX_EVENTS,-1);
		if(fds<0)
		{
			perror("epoll_wait err");
			//return -1;
            continue;
		}
		for(i=0;i<fds;i++)
		{
            request_handle(listenfd,events[i].data.fd,epollfd);
		}
	}
        
    close(listenfd);
	return 0;
}
