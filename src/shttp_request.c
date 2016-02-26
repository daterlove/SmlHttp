/*************************************************************************
    > File Name: shttp_request.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月09日 19时28分00秒 CST
 ************************************************************************/

#include "common.h"
extern const char *g_content_type[][2];
extern struct shttp_spinlock_t *shttp_lock;
extern int g_connect_count;
char g_cur_exe_dir[MAX_LINE_SIZE];//当前程序目录

int judge_url_type(char *url)
{
    int i;
    int len=strlen(url) - 1;
    for(i=len;i>=0;i--)
    {
        if(url[i] == '/') 
        {
            if(i == len)//顶层目录
                return 0;
            else        //二级目录
                return 1;
        }
        if(url[i] == '.')//文件
            return 2;
    }
    return -1;
}

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
void init_exe_dir()
{
   //获取当前目录
   int ret=get_exe_dir(g_cur_exe_dir,MAX_LINE_SIZE);
   if(ret < 0) 
   {
       printf("获取程序目录出错，程序退出\n");
       exit(0);
   }
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
        //---进程负载语句块
        #ifdef BLANCE_DEBUG
        printf("g_connect_count:%d\n",g_connect_count);
        #endif
        if(g_connect_count >MAX_CONNECTIONS)
        {
            #ifdef BLANCE_DEBUG
            printf("超过最大连接数，放弃连接\n");
            #endif
            return 1;
        }
        else if(g_connect_count > (MAX_CONNECTIONS* 7/8))
        {
            #ifdef BLANCE_DEBUG
            printf("超过连接总数7/8\n");
            #endif
            volatile int i=1;//多做一点操作方便其他进程有更多机会
            i--;
        }
        //获取进程锁
        int lock_ret=shttp_getlock(shttp_lock,getpid());
        if( lock_ret== 0)
        {
		    ret=socket_ET_accept(listenfd,epollfd);
            shttp_unlock(shttp_lock);//释放锁
            #ifdef DEBUG
            printf("监听请求，成功获取锁,PID:%d\n",getpid());
            #endif
        }
        else
        {
            /*
            #ifdef DEBUG
            printf("----没有获取到锁，PID：%d\n",getpid());
            #endif*/
            return 0;
        }
        
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
           // printf("\e[32m\e[1m----收到消息----------------------------------\e[0m\n");
           
            if(strncmp ( buf, "GET", 3 ) == 0)
            {
                //得出网页的本地路径
                sprintf(path, "%s/%s%s", g_cur_exe_dir,DIR_HTDOCS,url);
                ret=judge_url_type(url);
                if (ret == 0)//是目录
                {
                    strcat(path, "index.html");//首页路径
                }
                else if (ret == 1)//是目录
                {
                    strcat(path, "/index.html");//二级目录路径
                }       
                
               #ifdef DEBUG    
               log_output_client(client,"GET",url);
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

