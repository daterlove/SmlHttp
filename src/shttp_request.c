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
int request_handle(int client)
{
    int bytes;
	char buf[MAX_BUFFER_SIZE];
    char method[MAX_BUFFER_SIZE];
    char url[MAX_BUFFER_SIZE];
    
    //从缓冲区读取一行
    bytes=socket_getline(client,buf,sizeof(buf));
    //解析命令头
    request_parse_method(buf,bytes,method,url);
    
    /*
	bytes=recv(client,buf,MAX_BUFFER_SIZE,0);
	if(bytes<0)
	{
		perror("recv err");
		return -1;
	}
	if(bytes == 0)
	{
		printf("客户端关闭连接\n---------------\n");
		return -2;
	}
    */
	printf("收到信息buf:%s",buf);
    printf("method:%s,url:%s\n\n",method,url);
    close(client);
    
    return 0;
}

