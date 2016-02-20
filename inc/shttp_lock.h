/*************************************************************************
    > File Name: shttp_lock.h
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月20日 20时19分09秒 CST
 ************************************************************************/

#ifndef _SHTTP_LOCK_H_
#define _SHTTP_LOCK_H_
	//自定义进程锁结构体
	struct shttp_spinlock_t
	{
   	  	pthread_spinlock_t spinlock;
     		int shm_size;
     		int pid_lock;
	};

	struct shttp_spinlock_t *shttp_spinlock_create(int size);
	int shttp_getlock(struct shttp_spinlock_t *shttp_lock,int pid);
	int shttp_unlock(struct shttp_spinlock_t *shttp_lock);
#endif

