/*************************************************************************
    > File Name: shttp_epoll.h
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月05日 17时02分56秒 CST
 ************************************************************************/


#ifndef _SHTTP_EPOLL_H_
#define _SHTTP_EPOLL_H_
    int epoll_init(int listendfd);
    int epoll_add_sockfd(int epollfd,int sockfd);
#endif
