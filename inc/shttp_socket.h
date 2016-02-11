/*************************************************************************
    > File Name: shttp_connect.h
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时54分54秒 CST
 ************************************************************************/

#ifndef _SHTTP_CONNECT_H_
#define _SHTTP_CONNECT_H_
	int socket_listen();
    int socket_getline(int sock,char *buf,int size);
    // 在ET模式下必须循环accept到返回-1为止,并加入epoll事件
    int socket_ET_accept(int listenfd,int epollfd);
    int socket_read(int sock,char *buf,int bufsize);
#endif
