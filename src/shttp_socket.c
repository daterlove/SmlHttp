/*************************************************************************
    > File Name: shttp_connect.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时53分24秒 CST
 ************************************************************************/

#include "common.h"

//创建绑定套接字
//失败返回-1，成功返回监听套接字
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

//从套接字缓冲区读取一行
int socket_getline(int sock,char *buf,int size)
{
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);//读取一个字符
        if (n > 0)//读到了字符
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);//查看数据，不从缓冲区删除
                if ((n > 0) && (c == '\n'))
                {
                    recv(sock, &c, 1, 0);
                }
                else
                {
                    c = '\n';
                }
     
            }
            buf[i] = c;
            i++;
        }   
        else
        {
            c = '\n';
        }
    }
    buf[i] = '\0';
    return i;
}