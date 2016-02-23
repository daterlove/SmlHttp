/*************************************************************************
    > File Name: config.h
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月02日 14时04分51秒 CST
 ************************************************************************/


#ifndef _CONFIG_H_
#define _CONFIG_H_
   #define DEBUG
    
	#define LISTEN_PORT 8888       //监听端口
	#define MAX_LISTEN 100         //最大监听数
	#define PROCESS_NUM 3          //子进程数量
    #define MAX_BUFFER_SIZE 4068   //缓冲区大小
    #define MAX_LINE_SIZE 1024     //行字符串大小
    #define MAX_EVENTS 500         //epoll监听数大小
    //服务器信息字符串
    #define SERVER_STRING "Server:Sml-Http\r\n"
    #define DIR_HTDOCS "htdocs"    //网站目录
    #define LOG_NAME "record_log"   //日志文件名   
    #define MAX_CONNECTIONS 1024   //单进程最大连接数
#endif

