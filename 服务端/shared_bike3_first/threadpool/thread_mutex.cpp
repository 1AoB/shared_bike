
#include "thread.h"

// 创建互斥锁
int
thread_mutex_create(pthread_mutex_t *mtx)
{
    int            err;
    pthread_mutexattr_t  attr;// 定义互斥锁属性

    err = pthread_mutexattr_init(&attr);// 初始化互斥锁属性
    if (err != 0) {
        fprintf(stderr, "pthread_mutexattr_init() failed, reason: %s\n",strerror(errno));// 打印错误信息
        return ERROR;
    }

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);// 设置互斥锁属性为PTHREAD_MUTEX_ERRORCHECK
    if (err != 0) {
		fprintf(stderr, "pthread_mutexattr_settype(PTHREAD_MUTEX_ERRORCHECK) failed, reason: %s\n",strerror(errno));// 打印错误信息
        return ERROR;
    }

    err = pthread_mutex_init(mtx, &attr);// 初始化互斥锁
    if (err != 0) {
        fprintf(stderr,"pthread_mutex_init() failed, reason: %s\n",strerror(errno));// 打印错误信息
        return ERROR;
    }

    err = pthread_mutexattr_destroy(&attr);// 销毁互斥锁属性
    if (err != 0) {
		fprintf(stderr,"pthread_mutexattr_destroy() failed, reason: %s\n",strerror(errno));// 打印错误信息
    }

    return OK;
}

// 销毁互斥锁
int
thread_mutex_destroy(pthread_mutex_t *mtx)
{
    int  err;

    err = pthread_mutex_destroy(mtx);// 销毁互斥锁
    if (err != 0) {
        fprintf(stderr,"pthread_mutex_destroy() failed, reason: %s\n",strerror(errno));// 打印错误信息
        return ERROR;
    }

    return OK;
}


// 上锁
int
thread_mutex_lock(pthread_mutex_t *mtx)
{
    int  err;

    err = pthread_mutex_lock(mtx);// 上锁
    if (err == 0) {
        return OK;
    }
    // 打印错误信息
	fprintf(stderr,"pthread_mutex_lock() failed, reason: %s\n",strerror(errno));

    return ERROR;
}


// 解锁
int
thread_mutex_unlock(pthread_mutex_t *mtx)
{
    int  err;

    err = pthread_mutex_unlock(mtx);// 解锁

#if 0  // 如果定义了0，则不执行下面的代码
    ngx_time_update();
#endif

    if (err == 0) {
        return OK;
    }
    // 打印错误信息
	fprintf(stderr,"pthread_mutex_unlock() failed, reason: %s\n",strerror(errno));
    return ERROR;
}
