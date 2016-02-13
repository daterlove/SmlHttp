/*************************************************************************
    > File Name: shttp_response.h
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月09日 21时15分08秒 CST
 ************************************************************************/

#ifndef _SHTTP_RESPONSE_H_
#define _SHTTP_RESPONSE_H_
    void response_sendfile(int client,char *path);
    void response_head_200(int client,int type_index);
    void response_unimplement_501(int client);
    void response_notfound_404(int client);
#endif
