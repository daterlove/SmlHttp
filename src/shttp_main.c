/*************************************************************************
    > File Name: shttp_main.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时58分54秒 CST
 ************************************************************************/

#include "common.h"

//进程锁
struct shttp_spinlock_t *shttp_lock;

int main(int arg,char **argv)
{
    int ret;
    
    //初始化信号处理
    signal_init();
    //开始监听
    int listenfd=socket_listen();
    if (listenfd<0)
    {
        return -1;
    }
    log_start(listenfd);
    
    //创建进程锁
    shttp_lock=shttp_spinlock_create(sizeof(struct shttp_spinlock_t));
    if(shttp_lock == -1)
    {
        perror("分配共享内存错误，程序退出");
        return -1;
    }
    
    //创建子进程
    ret=process_CreateSub();
    if(0==ret)
    {
        pause();
        printf("master进程-pid:%d\n",getpid());
    }
    else if(1==ret)
    {
        printf("worker子进程启动-pid:%d\n",getpid());
    }
    else//发生错误
    {
        return -1;
    }
    
 //--------子进程执行内容------------------------------- 
    int epollfd,fds;
    struct epoll_event events[MAX_EVENTS];//epoll触发事件
    //创建epoll句柄,并加入监听套接字
    epollfd=epoll_init(listenfd);
  
    int i;
	while(1)
	{
		//通常需要超时处理
		fds=epoll_wait(epollfd,events,MAX_EVENTS,-1);
       // printf("收到数据-pid:%d\n",getpid());
		if(fds<0)
		{
			perror("epoll_wait err");
            continue;
		}
		for(i=0;i<fds;i++)
		{
            request_handle(listenfd,events[i].data.fd,epollfd);
		}
	}
        
    close(listenfd);
	return 0;
}
