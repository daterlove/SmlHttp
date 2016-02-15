/*************************************************************************
    > File Name: shttp_connect.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时53分24秒 CST
 ************************************************************************/

#include "common.h"

//创建绑定套接字
//失败返回-1，成功返回监听套接字
int maxfd;
int socket_listen()
{
    struct sockaddr_in server_addr;
    int listenfd;
    int ret,opt=1;
    
    //初始化服务器地址
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(LISTEN_PORT);//config.h中指定
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    
    //创建监听套接字
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0)
    {
        perror("Create socket error");
        return -1;
    }
    //设置地址可重用
    ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if(ret<0)
    {
        perror("setsockopt error");
        return -1;
    }
    //绑定地址
    int len=sizeof(struct sockaddr);
    ret=bind(listenfd,(struct sockaddr*)&server_addr,len);
    if(ret<0)
    {
        perror("bind error");
        return -1;
    }
    //开始监听,设置最大连接请求
    ret=listen(listenfd,MAX_LISTEN);//config.h中指定
    if(ret<0)
    {
        perror("listen error");
        return -1;
    }
    
    //设置 监听文件描述符 为非阻塞
	fcntl(listenfd,F_SETFL,O_NONBLOCK);
    return listenfd;
}
int socket_setarg(int socket)
{
    //TCP_CORK：采用Nagle算法把较小的包组装为更大的帧
     int on = 1;
    setsockopt (socket, SOL_TCP, TCP_CORK, &on, sizeof ( on ) );
    //设置非阻塞
    int ret=fcntl(socket,F_SETFL,O_NONBLOCK);
    
    return ret;
}
// 在ET模式下必须循环accept到返回-1为止,并加入epoll事件
int socket_ET_accept(int listenfd,int epollfd)
{
    struct sockaddr in_addr;
    socklen_t in_len;
    int infd;
    int ret;
    
    while(1)
    {
        in_len = sizeof(in_addr);
        infd = accept ( listenfd, &in_addr, &in_len );
        
        if ( infd == -1 )
        {
            if ( ( errno == EAGAIN ) ||( errno == EWOULDBLOCK ) )
            {
                //处理完所有监听套接字
                break;
            }
            else
            {
                perror ( "accept" );
                return -1;
               // break;
            }
        }
        //设置非阻塞 及 TCP_CORK
        ret=socket_setarg(infd);
        if(ret<0)
        {
            perror("socket_setarg error");
            return -1;
        }
        //加入epoll事件
        ret=epoll_add_sockfd(epollfd,infd);
        if(ret<0)
        {
            perror("epoll_add_sockfd error");
            return -1;
        }
        if(infd>maxfd) maxfd=infd;
        //printf("接受请求：infd=%d,加入epoll事件\n",infd);
    }
    return 0;
}
int socket_read(int sock,char *buf,int bufsize)
{
    int pos=0,count;
    while(1)
    {
        count = read ( sock, buf+pos, bufsize);
        if ( count == -1 )
        {
            if ( errno == EAGAIN )
            {
                //读取结束
                buf[pos]=0;
                return pos;
            }
            else
            {
                perror("read error");
                return -1;
            }
        }
        else if(count == 0)// 被客户端关闭连接
        {
           // printf("客户端连接,sock=%d\n",sock);
            log_success(sock,"客户端关闭连接");
            close(sock);
            return 0;
        }
        else if(count >0 )
        {
            pos+=count;
        }
    }
    return pos;
}
