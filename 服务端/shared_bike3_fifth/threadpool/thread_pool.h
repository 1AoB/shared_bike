#ifndef _THREAD_POOL_H_INCLUDED_
#define _THREAD_POOL_H_INCLUDED_

#ifdef __cplusplus //如果使用c++编译器，那么下面的内容要使用c语言的方式来编译
extern "C" {
#endif

#include "thread.h"

#define DEFAULT_THREADS_NUM 4// 默认线程数为4
#define DEFAULT_QUEUE_NUM  65535// 默认任务队列长度为65535


typedef unsigned long         atomic_uint_t;// 定义atomic_uint_t类型为unsigned long
typedef struct thread_task_s  thread_task_t;// 定义thread_task_t结构体类型
typedef struct thread_pool_s  thread_pool_t;// 定义thread_pool_t结构体类型

// 定义thread_task_t结构体类型
struct thread_task_s {
    thread_task_t       *next;// 指向下一个任务的指针
    uint_t               id;// 任务的ID
    void                *ctx;// 任务的上下文信息:任务处理函数的参数，用来传递任务需要的数据。
    void               (*handler)(void *data);// 任务的处理函数
};
// 定义任务队列类型
typedef struct {
    thread_task_t        *first;// 指向第一个任务的指针
    thread_task_t        **last;// 指向最后一个任务的指针
} thread_pool_queue_t;
/*
last 是一个指向最后一个任务的指针，它的类型是一个指向 thread_task_t 指针的指针，也即二级指针。
它的作用是方便在队列末尾添加新任务时更新队列最后一个任务的指针。

如果 last 是一级指针，而不是指向指针的指针，那么在添加新任务时，
就需要遍历整个任务队列，以找到最后一个任务的指针，这样会降低添加任务的效率。
而使用指向指针的指针，我们只需要将新任务的指针赋值给 *last，
就可以方便地更新队列最后一个任务的指针，无需遍历整个队列。
*/

// 定义宏，用于初始化任务队列
#define thread_pool_queue_init(q)                                         \
    (q)->first = NULL;                                                    \
    (q)->last = &(q)->first

// 定义thread_pool_t结构体类型
struct thread_pool_s {
    pthread_mutex_t        mtx;// 任务队列互斥锁
    thread_pool_queue_t   queue;// 任务队列
    int_t                 waiting;// 等待任务的线程数
    pthread_cond_t         cond;// 条件变量

    char                  *name;// 线程池名称
    uint_t                threads;// 线程数
    int_t                 max_queue;// 最大任务队列长度
};

thread_task_t *thread_task_alloc(size_t size);// 函数用于分配任务内存
void thread_task_free(thread_task_t* task);
int_t thread_task_post(thread_pool_t *tp, thread_task_t *task);// 函数用于向线程池提交任务
thread_pool_t* thread_pool_init();// 函数用于初始化线程池
void thread_pool_destroy(thread_pool_t *tp);// 函数用于销毁线程池

#ifdef __cplusplus
}
#endif

#endif /* _THREAD_POOL_H_INCLUDED_ */
