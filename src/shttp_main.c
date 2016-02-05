/*************************************************************************
    > File Name: shttp_main.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时58分54秒 CST
 ************************************************************************/

#include "common.h"
int Process_data(int sockfd)
{
	int bytes;
	char buf[MAX_BUFFER_SIZE];
	bytes=recv(sockfd,buf,MAX_BUFFER_SIZE,0);
	if(bytes<0)
	{
		perror("recv err");
		return -1;
	}
	if(bytes == 0)
	{
		printf("客户端关闭连接\n---------------\n");
		return -2;
	}
	printf("收到信息buf:\n%s\n",buf);
	send(sockfd,buf,strlen(buf),0);
	return 0;
}

int main(int arg,char **argv)
{
    int ret;
    
    //初始化信号处理
    signal_init();
    //开始监听
    int listenfd=connect_socket_start();
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
    int sockfd;
    int epollfd,fds;
    struct epoll_event ev,events[MAX_EVENTS];//epoll触发事件
    int rv;
	struct sockaddr_in client;//客户端地址
	int len=sizeof(struct sockaddr_in);
    
    //创建epoll句柄
	epollfd=epoll_create(MAX_EVENTS);
    
    ev.data.fd=listenfd;
	ev.events=EPOLLIN;
	rv=	epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);//将监听fd加入触发事件
	if(rv<0)
	{
		perror("epoll_ctrl err");
		return -1;
	}
    int i;
	while(1)
	{
		//通常需要超时处理
		fds=epoll_wait(epollfd,events,MAX_EVENTS,-1);
		if(fds<0)
		{
			perror("epoll_wait err");
			return -1;
		}
		for(i=0;i<fds;i++)
		{
			if(events[i].data.fd == listenfd)
			{
				sockfd=accept(listenfd,(struct sockaddr *)&client,&len);
				if(sockfd<0)
				{
					perror("accept error");
					continue;
				}
				ev.data.fd=sockfd;
				ev.events=EPOLLIN | EPOLLET;
				epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
				continue;
			}
			else
			{
				rv=Process_data(events[i].data.fd);
				if(rv == -2)
				{
					epoll_ctl(epollfd,EPOLL_CTL_DEL,events[i].data.fd,&ev);
				}
			}
		}
	}
        
    close(listenfd);
	return 0;
}
