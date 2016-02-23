/*************************************************************************
    > File Name: sttp_epoll.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月05日 17时02分24秒 CST
 ************************************************************************/

#include"common.h"

int epoll_init(int listenfd)
{
    int epollfd;
    int rv;
    struct epoll_event ev;
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
    return epollfd;
}

int epoll_add_sockfd(int epollfd,int sockfd)
{
    struct epoll_event ev;
    ev.data.fd=sockfd;
	ev.events=EPOLLIN | EPOLLET;
	int ret=epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
    
    return ret;
}