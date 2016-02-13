/*************************************************************************
    > File Name: shttp_response.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月09日 21时14分38秒 CST
 ************************************************************************/

#include "common.h"
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
    {"zip ","application/zip"},
    {"txt ","text/plain"},
    {"asc ","text/plain"},
    {"xml ","xsl"},
    {"dtd ","application/xml-dtd"},
    {"xslt","application/xslt+xml"},
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
    /*
    printf("#############################\n");
    for(i=0;i<4;i++)
    {
        printf("%x ",buf[i]);
    }
    printf("\n");
    */
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
    printf("response_head_200:1\n");
    int type_index=cmp_content_type(path);
    if(type_index < 0)
    {
        response_unimplement_501(client);
        return;
    }
    printf("response_head_200:2\n");
    struct stat st;
    if (stat(path, &st) == -1)//读取文件失败
    {
        printf("response_head_200:3\n");
        perror("stat");
        //printf("bytes:%d,收到信息buf:\n%s",bytes,buf);
        response_notfound_404(client);
        
    } 
    else
    {
        printf("response_head_200:4\n");
        off_t offset=0;
        int fd = open(path, O_RDONLY);
        if(fd<0)
         {
            perror("response-open");
            return;
         }
      printf("response_head_200:5\n");
        //发送协议头
        response_head_200(client,type_index);
      printf("response_head_200:6\n");  
        //发送文件
        int ret=sendfile(client,fd,&offset,st.st_size);;
        if(ret<0)
        {
            perror("sendfie");
        }
      printf("response_head_200:7\n");  
        close(fd);
      printf("response_head_200:8\n");  
        close(client);   
      printf("response_head_200:9\n");  
        printf("文件-正确发送,关闭client：%d\n",client);  
      printf("response_head_200:10\n");              
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
 sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
 send(client, buf, strlen(buf), 0);
 //指定网页编码
 sprintf(buf, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /> ");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<BODY><P>文件找不到The server could not fulfill\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "your request because the resource specified\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "is unavailable or nonexistent.\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "</BODY></HTML>\r\n");
 send(client, buf, strlen(buf), 0);
 
 printf("回复：404-关闭client:%d\n",client);
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
 
 printf("回复：501-关闭client:%d\n",client);
 close(client);
}
