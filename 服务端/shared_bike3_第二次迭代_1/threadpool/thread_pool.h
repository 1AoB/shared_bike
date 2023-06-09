#ifndef _THREAD_POOL_H_INCLUDED_
#define _THREAD_POOL_H_INCLUDED_

#ifdef __cplusplus //���ʹ��c++����������ô���������Ҫʹ��c���Եķ�ʽ������
extern "C" {
#endif

#include "thread.h"

#define DEFAULT_THREADS_NUM 4// Ĭ���߳���Ϊ4
#define DEFAULT_QUEUE_NUM  65535// Ĭ��������г���Ϊ65535


typedef unsigned long         atomic_uint_t;// ����atomic_uint_t����Ϊunsigned long
typedef struct thread_task_s  thread_task_t;// ����thread_task_t�ṹ������
typedef struct thread_pool_s  thread_pool_t;// ����thread_pool_t�ṹ������

// ����thread_task_t�ṹ������
struct thread_task_s {
    thread_task_t       *next;// ָ����һ�������ָ��
    uint_t               id;// �����ID
    void                *ctx;// �������������Ϣ:���������Ĳ�������������������Ҫ�����ݡ�
    void               (*handler)(void *data);// ����Ĵ�����
};
// ���������������
typedef struct {
    thread_task_t        *first;// ָ���һ�������ָ��
    thread_task_t        **last;// ָ�����һ�������ָ��
} thread_pool_queue_t;
/*
last ��һ��ָ�����һ�������ָ�룬����������һ��ָ�� thread_task_t ָ���ָ�룬Ҳ������ָ�롣
���������Ƿ����ڶ���ĩβ���������ʱ���¶������һ�������ָ�롣

��� last ��һ��ָ�룬������ָ��ָ���ָ�룬��ô�����������ʱ��
����Ҫ��������������У����ҵ����һ�������ָ�룬�����ή����������Ч�ʡ�
��ʹ��ָ��ָ���ָ�룬����ֻ��Ҫ���������ָ�븳ֵ�� *last��
�Ϳ��Է���ظ��¶������һ�������ָ�룬��������������С�
*/

// ����꣬���ڳ�ʼ���������
#define thread_pool_queue_init(q)                                         \
    (q)->first = NULL;                                                    \
    (q)->last = &(q)->first

// ����thread_pool_t�ṹ������
struct thread_pool_s {
    pthread_mutex_t        mtx;// ������л�����
    thread_pool_queue_t   queue;// �������
    int_t                 waiting;// �ȴ�������߳���
    pthread_cond_t         cond;// ��������

    char                  *name;// �̳߳�����
    uint_t                threads;// �߳���
    int_t                 max_queue;// ���������г���
};

thread_task_t *thread_task_alloc(size_t size);// �������ڷ��������ڴ�
void thread_task_free(thread_task_t* task);
int_t thread_task_post(thread_pool_t *tp, thread_task_t *task);// �����������̳߳��ύ����
thread_pool_t* thread_pool_init();// �������ڳ�ʼ���̳߳�
void thread_pool_destroy(thread_pool_t *tp);// �������������̳߳�

#ifdef __cplusplus
}
#endif

#endif /* _THREAD_POOL_H_INCLUDED_ */
