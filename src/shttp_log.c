/*************************************************************************
    > File Name: shttp_log.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月13日 21时24分58秒 CST
 ************************************************************************/

#include "common.h"
extern int maxfd;
//格式化时间 输出到buf
void format_time(char *time_buf)
{
    time_t timep; 
    struct tm *p; 
    time(&timep); 
    p=gmtime(&timep); 
    sprintf(time_buf,"%04d-%02d-%02d %02d:%02d:%02d",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday, 
        p->tm_hour, p->tm_min, p->tm_sec);
}
//格式化 客户端 ip，端口
void format_client(int client,char *client_buf)
{
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);
    if(!getpeername(client, (struct sockaddr *)&sa, &len))
    {
        sprintf(client_buf,"IP:%s 端口:%d",inet_ntoa(sa.sin_addr),ntohs(sa.sin_port));
    }
    else
    {
       client_buf[0]='\0';
    }
}

void format_color_out(int text_color,int bracket_color,char *str)
{
    printf("\e[%dm\e[1m%s\e[0m ",bracket_color,"[");
    printf("\e[%dm\e[1m%s\e[0m ",text_color,str);
    printf("\e[%dm\e[1m%s\e[0m\n\n",bracket_color,"]");
}
void log_output_client(int client,char *method,char *url)
{
    char time_buf[256];
    char client_buf[256];

    format_time(time_buf);
    format_client(client,client_buf);
    printf("\e[35m\e[1m%s\e[0m ","[收到请求]:");
    printf("\e[34m\e[1m[%s]\e[0m ",time_buf);
    printf("\e[34m\e[1m[%s client:%d]\e[0m\n",client_buf,client);
    printf("\e[36m\e[1m[请求方法:%s  URL:%s]\e[0m\n",method,url);
 
}

void log_error(int client,char *msg)
{
    //char buf[100];
    printf("\e[31m\e[1m[请求失败:%s client:%d]\e[0m\n\n",msg,client);
    //sprintf(buf,"请求失败:%s client:%d",msg,client);
    //format_color_out(31,33,buf);
}

void log_success(int client,char *msg)
{
    printf("\e[33m\e[1m[操作成功:%s client:%d]\e[0m\n\n",msg,client);
    //printf("\e[33m\e[1m[成功操作:%s client:%d]\e[0m\nmaxfd:%d\n",msg,client,maxfd);
}

void log_start(int listenfd)
{
    int ret;
    struct sockaddr_in sa; 
    
    socklen_t len=sizeof(struct   sockaddr_in); 
    ret=getsockname(listenfd,(struct sockaddr*)&sa,&len);
    if(ret!=0) 
    { 
        printf( "服务器地址解析失败，程序退出！\n"); 
        exit(0);
    } 
    
    char server_buf[344];
    sprintf(server_buf,"http//:%s:%d/",inet_ntoa(sa.sin_addr),ntohs(sa.sin_port));
    printf("\e[31m\e[1m%s\e[0m\n","Sml-Http 服务器端启动");
    printf("\e[33m\e[1m[访问地址:%s]\e[0m\n\n",server_buf);
}