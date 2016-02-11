/*************************************************************************
    > File Name: shttp_response.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月09日 21时14分38秒 CST
 ************************************************************************/

#include "common.h"
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
 
 printf("关闭client:%d\n",client);
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
 
 printf("关闭client:%d\n",client);
 close(client);
}
