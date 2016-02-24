/*************************************************************************
    > File Name: shttp_main.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年01月30日 14时58分54秒 CST
 ************************************************************************/

#include "common.h"

struct shttp_spinlock_t *shttp_lock;//进程锁
int g_connect_count;//进程连接数
int g_maxfd;        //监听的最大套接字fd[记录用]
int g_accept_disable;//进程负载判断标志

int main(int argc,char **argv)
{
    int recordnum=-1;
    string_get_argments(argc,argv,&recordnum);
    printf("recordnum:%d\n",recordnum);
    int ret;
    
    
    //开始监听
    int listenfd=socket_listen();
    if (listenfd<0)
    {
        return -1;
    }
    log_start(listenfd);
    
    //创建进程锁
    shttp_lock=shttp_spinlock_create(sizeof(struct shttp_spinlock_t));
    if(shttp_lock == (struct shttp_spinlock_t *)-1)
    {
        perror("进程锁分配共享内存错误，程序退出");
        return -1;
    }
    
    //创建子进程
    ret=process_CreateSub();
    if(0==ret)//父进程
    {
        signal(SIGALRM,&signal_alarm_handlde);
        if(recordnum < 0)
        {
            pause();
        }
        else if (recordnum > 0)
        {
            int i;

            for(i=0;i<recordnum;i++)
            {
                timer_record_log(3);
            }
        }
        else
        {
            for(;;)
            {
                timer_record_log(3); 
            } 
        }
        pause();
    }
    else if(1==ret)//子进程
    {
        //初始化信号处理
        signal_init();
        printf("\e[34m\e[1mworker子进程启动-pid:%d \e[0m\n",getpid());
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
            if(errno == EINTR)//高级别中断
                continue;
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
