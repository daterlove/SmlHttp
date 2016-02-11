/*************************************************************************
    > File Name: shttp_request.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月09日 19时28分00秒 CST
 ************************************************************************/

#include "common.h"
void request_parse_method(char *buf,int bufsize,char *method,char *url)
{
    int i = 0,j = 0;
    while (!isspace(buf[j]) && (i < sizeof(MAX_BUFFER_SIZE) - 1))
    {
        method[i] = buf[j];
        i++; j++;
    }
    method[i] = '\0';  
   
    //获取第一个空格位置，保存在j里
    while (isspace(buf[j]) && (j < sizeof(buf)))
        j++;
    
    i=0;
    //保存第一空格到第二空格之间的 url地址
    while (!isspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
    {
        url[i] = buf[j];
        i++; j++;
    }
    url[i] = '\0';
}
int request_handle(int listenfd,int client,int epollfd)
{
    int ret;
    int bytes;
	char buf[MAX_BUFFER_SIZE];
    char method[MAX_BUFFER_SIZE]; 
    char url[MAX_BUFFER_SIZE];
    
    if(listenfd == client)//如果是监听连接
	{
		ret=socket_ET_accept(listenfd,epollfd);
		if(ret<0)
		{
			perror("socket_ET_accept");
		}
	}
	else
	{
        //bytes=recv(client,buf,MAX_BUFFER_SIZE,0);
        bytes=socket_read(client,buf,MAX_BUFFER_SIZE);
        if(bytes<0)
        {
            return -1;
        }
        else if(bytes>0)
        {
            request_parse_method(buf,bytes,method,url);
            //printf("bytes:%d,收到信息buf:\n%s",bytes,buf);
            printf("method:%s,url:%s\n\n",method,url);
            if(!strncmp ( buf, "GET", 3 ) == 0)
            {
                response_unimplement_501(client);//发送给客户端501错误
            }
            else
            {
                response_notfound_404(client);
            }
            
        }
        
	}
    
    
    return 0;
}

