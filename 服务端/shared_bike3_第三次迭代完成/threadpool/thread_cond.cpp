#include "thread.h"


// 创建条件变量
int
thread_cond_create(pthread_cond_t *cond)
{
    int  err;

    err = pthread_cond_init(cond, NULL);// 初始化条件变量
    if (err == 0) {
        return T_OK;
    }
    // 打印错误信息
    fprintf(stderr, "pthread_cond_init() failed, reason: %s\n",strerror(errno));
    return T_ERROR;
}


// 销毁条件变量
int
thread_cond_destroy(pthread_cond_t *cond)
{
    int  err;

    err = pthread_cond_destroy(cond); // 销毁条件变量
    if (err == 0) {
        return T_OK;
    }
    // 打印错误信息
	fprintf(stderr, "pthread_cond_destroy() failed, reason: %s\n",strerror(errno));
    return T_ERROR;
}

// 发送条件信号
int
thread_cond_signal(pthread_cond_t *cond)
{
    int  err;

    err = pthread_cond_signal(cond);// 发送条件信号
    if (err == 0) {
        return T_OK;
    }
    // 打印错误信息
	fprintf(stderr, "pthread_cond_signal() failed, reason: %s\n",strerror(errno));
    return T_ERROR;
}

// 等待条件变量
int
thread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mtx)
{
    int  err;

    err = pthread_cond_wait(cond, mtx);// 等待条件变量

    if (err == 0) {
        return T_OK;
    }

    // 打印错误信息
	fprintf(stderr, "pthread_cond_wait() failed, reason: %s\n",strerror(errno));
    return T_ERROR;
}

