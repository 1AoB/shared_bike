#ifndef _DEMO_THREAD_H_INCLUDED_
#define _DEMO_THREAD_H_INCLUDED_

#ifdef __cplusplus //如果使用c++编译器，那么下面的内容要使用c语言的方式来编译
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

	// 定义int_t和uint_t类型
	typedef intptr_t        int_t;
	typedef uintptr_t       uint_t;

#define  T_OK          0 // 定义OK常量为0，表示操作成功
#define  T_ERROR      -1 // 定义ERROR常量为-1，表示操作失败


	// 以下函数用于互斥锁的操作
	int thread_mutex_create(pthread_mutex_t* mtx);// 创建互斥锁
	int thread_mutex_destroy(pthread_mutex_t* mtx);// 销毁互斥锁
	int thread_mutex_lock(pthread_mutex_t* mtx);// 上锁
	int thread_mutex_unlock(pthread_mutex_t* mtx);// 解锁

	// 以下函数用于条件变量的操作
	int thread_cond_create(pthread_cond_t* cond);// 创建条件变量
	int thread_cond_destroy(pthread_cond_t* cond);// 销毁条件变量
	int thread_cond_signal(pthread_cond_t* cond);// 发送信号
	int thread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mtx);// 等待信号

#ifdef __cplusplus
}
#endif

#endif /* _DEMO_THREAD_H_INCLUDED_ */




