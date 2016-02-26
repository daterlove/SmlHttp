/*************************************************************************
    > File Name: shttp_request.h
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月09日 19时28分31秒 CST
 ************************************************************************/

#ifndef _SHTTP_REQUEST_H_
#define _SHTTP_REQUEST_H_

    void request_parse_url(char *buf,int bufsize,char *url,int linesize);
    int request_handle(int listenfd,int client,int epollfd);
    void init_exe_dir();
#endif
