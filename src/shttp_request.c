/*************************************************************************
    > File Name: shttp_request.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月09日 19时28分00秒 CST
 ************************************************************************/

#include "common.h"
extern const char *g_content_type[][2];
int get_exe_dir(char *path,int size)
{
    int count; 
    count = readlink( "/proc/self/exe", path, size ); 

    if ( count < 0 || count >= size ) 
    { 
        return -1; 
    } 

    while(count>0 && path[count]!='/')//最后一个斜杠处
        count--;
    path[count]=0;

    return 0;
}
void request_parse_url(char *buf,int bufsize,char *url,int linesize)
{
    int i = 0,j = 0;
  
    //获取第一个空格位置，保存在j里
    while (!isspace(buf[j]) && (j < bufsize))
    {
        j++;
    }
    j++;//跳过空格
 
    //保存第一空格到第二空格之间的 url地址
    while (!isspace(buf[j]) && (i < linesize-1) && (j < bufsize-1))
    {
        url[i] = buf[j];
        i++; j++;
    }
    url[i] = '\0';
}
int request_handle(int listenfd,int client,int epollfd)
{
    int ret,bytes;
	char buf[MAX_BUFFER_SIZE];
    char url[MAX_LINE_SIZE];
    char path[MAX_LINE_SIZE];
    
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
        //循环读取
        bytes=socket_read(client,buf,MAX_BUFFER_SIZE);
        if(bytes<0)
        {
            return -1;
        }
        else if(bytes>0)
        {
            //解析url地址
            request_parse_url(buf,bytes,url,MAX_LINE_SIZE);
            
            printf("\e[32m\e[1m----收到消息----------------------------------\e[0m\n");
           
            if(strncmp ( buf, "GET", 3 ) == 0)
            {
                //获取当前目录
                char cur_dir[MAX_LINE_SIZE];
                ret=get_exe_dir(cur_dir,MAX_LINE_SIZE);
                if(ret < 0) return -1; 
                //得出网页的本地路径
                sprintf(path, "%s/%s%s", cur_dir,DIR_HTDOCS,url);
                if (path[strlen(path) - 1] == '/')
                {
                    strcat(path, "index.html");//首页路径
                }       
                
                #ifdef DEBUG    
                printf("GET请求：path:%s\n",path);
                #endif    
                
                response_sendfile(client,path);  
            }
            else
            {
                response_unimplement_501(client);//发送给客户端501错误
            }
            
        }
        
	}
    
    
    return 0;
}

