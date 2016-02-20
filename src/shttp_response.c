/*************************************************************************
    > File Name: shttp_response.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月09日 21时14分38秒 CST
 ************************************************************************/

#include "common.h"
int g_count;
const char *g_content_type[][2]=
{
    {"html","text/html"},
    {"htm ","text/html"},
    {"jpg ","image/jpeg"},
    {"jpe ","image/jpeg"},
    {"jpeg","image/jpeg"},
    {"png ","image/png"},
    {"js  ","text/javascript"},
    {"css ","text/css"},
    {"gif ","image/gif"},
    {"ico ","image/x-icon"},
    {"zip ","application/zip"},
    {"txt ","text/plain"},
    {"asc ","text/plain"},
    {"xml ","xsl"},
    {"dtd ","application/xml-dtd"},
    {"xslt","application/xslt+xml"},
    {"bmp ","application/x-bmp"},
    {"doc ","application/msword"},
    {NULL,NULL}
};
//将四个字节 字符串 转成 int型进行比较
static int cmp_content_type(char *path)
{
    int len=strlen(path);
    int i,j;
    for(i=len;i>=0;i--)
    {
        if(path[i]  == '.') break;
    }
    if(i == 0) return -1;//没有找到
    char buf[4];
    memset(buf,0x20,4);//0x20是空格
    //复制到缓冲区
    for(j=0;j<len-i+1;j++)
    {
        buf[j]=path[++i];
    }

    int *nType=(int *)buf;
    i=0;
    while(g_content_type[i][0] != NULL)
    {
        //printf("nType=%d,g=%d\n",*nType,*((int *)&g_content_type[i][0]) );
        if(*((int *)g_content_type[i][0]) == *nType)//转成int型再比较
            return i;
        i++;
    }
    return -1;
}
void response_sendfile(int client,char *path)
{

    int type_index=cmp_content_type(path);
    if(type_index < 0)
    {
        response_unimplement_501(client);
        return;
    }

    struct stat st;
    if (stat(path, &st) == -1)//读取文件失败
    {
        response_notfound_404(client);
    } 
    else
    {
        off_t offset=0;
        int fd = open(path, O_RDONLY);
        if(fd<0)
        {
           perror("response-open");
           return;
        }
        //发送协议头
        response_head_200(client,type_index);
        sendfile(client,fd,&offset,st.st_size);
        close(fd);
        close(client);   
        
        #ifdef DEBUG  
        log_success(client,"文件成功发送");
        #endif         
    }    
     
}
void response_head_200(int client,int type_index)
{
 char buf[1024];

 strcpy(buf, "HTTP/1.0 200 OK\r\n");
 send(client, buf, strlen(buf), 0);
 strcpy(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Type: %s\r\n",g_content_type[type_index][1]);
 send(client, buf, strlen(buf), 0);
 //printf("200:buf:%s",buf);
 strcpy(buf, "\r\n");
 send(client, buf, strlen(buf), 0);
}
void response_notfound_404(int client)
{
 char buf[1024];

 sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Type: text/html\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "\r\n");
 send(client, buf, strlen(buf), 0);
 
 sprintf(buf, "<html><head><title>Not Found</title></head>");
 send(client, buf, strlen(buf), 0);
 //指定网页编码
 sprintf(buf, "<meta charset=utf-8 />");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<style>body{text-align:center}</style>");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<body text-align:center><h1>404 Not Found</h1>唔……你犯了一个404错误！");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<p /><hr /><small>");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Server:Sml-Http Author:DaterLove</small>");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "</body></html>");
 send(client, buf, strlen(buf), 0);

 #ifdef DEBUG         
 log_error(client,"404错误-文件不存在");
 #endif 
 close(client);
}
void response_unimplement_501(int client)
{
 char buf[1024];

 sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Type: text/html\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "</TITLE></HEAD>\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "</BODY></HTML>\r\n");
 send(client, buf, strlen(buf), 0);
 
 log_error(client,"501错误-尚未支持该请求");
 close(client);
}
