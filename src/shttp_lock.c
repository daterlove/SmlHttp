/*************************************************************************
    > File Name: shttp_lock.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月20日 20时18分22秒 CST
 ************************************************************************/

#include "common.h"
//调试程序
void print_lock(struct shttp_spinlock_t *shttp_lock)
{
    printf("[pid_lock:%d,shttp_lock->spinlock:%d]\n",shttp_lock->pid_lock,shttp_lock->spinlock);
}

struct shttp_spinlock_t *shttp_spinlock_create(int size)
{
    //申请共享内存
    struct shttp_spinlock_t *shttp_lock;
    shttp_lock=(struct shttp_spinlock_t *)mmap(NULL,size, PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    shttp_lock->shm_size=size;
    shttp_lock->pid_lock=0;
    //初始化自旋转锁（spinlock=1）        
    pthread_spin_init(&shttp_lock->spinlock, 0);
    
    return shttp_lock;
}
int shttp_getlock(struct shttp_spinlock_t *shttp_lock,int pid)
{
    int ret = 0;
    pthread_spin_lock(&shttp_lock->spinlock);//自旋锁 加锁【为了保证下面操作原子性】

    if(shttp_lock->pid_lock == 0 )//未上锁
    {
        shttp_lock->pid_lock=pid;
       //printf("上锁：pid_lock:%d,pid:%d\n",shttp_lock->pid_lock,pid);
    }
    else
    {
        ret = -1;
    }
    pthread_spin_unlock(&shttp_lock->spinlock);//自旋锁 解锁
    
    return ret;
}
int shttp_unlock(struct shttp_spinlock_t *shttp_lock)
{
    shttp_lock->pid_lock=0;
    return 0;
}
