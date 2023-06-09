
#include "thread.h"

// ����������
int
thread_mutex_create(pthread_mutex_t *mtx)
{
    int            err;
    pthread_mutexattr_t  attr;// ���廥��������

    err = pthread_mutexattr_init(&attr);// ��ʼ������������
    if (err != 0) {
        fprintf(stderr, "pthread_mutexattr_init() failed, reason: %s\n",strerror(errno));// ��ӡ������Ϣ
        return ERROR;
    }

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);// ���û���������ΪPTHREAD_MUTEX_ERRORCHECK
    if (err != 0) {
		fprintf(stderr, "pthread_mutexattr_settype(PTHREAD_MUTEX_ERRORCHECK) failed, reason: %s\n",strerror(errno));// ��ӡ������Ϣ
        return ERROR;
    }

    err = pthread_mutex_init(mtx, &attr);// ��ʼ��������
    if (err != 0) {
        fprintf(stderr,"pthread_mutex_init() failed, reason: %s\n",strerror(errno));// ��ӡ������Ϣ
        return ERROR;
    }

    err = pthread_mutexattr_destroy(&attr);// ���ٻ���������
    if (err != 0) {
		fprintf(stderr,"pthread_mutexattr_destroy() failed, reason: %s\n",strerror(errno));// ��ӡ������Ϣ
    }

    return OK;
}

// ���ٻ�����
int
thread_mutex_destroy(pthread_mutex_t *mtx)
{
    int  err;

    err = pthread_mutex_destroy(mtx);// ���ٻ�����
    if (err != 0) {
        fprintf(stderr,"pthread_mutex_destroy() failed, reason: %s\n",strerror(errno));// ��ӡ������Ϣ
        return ERROR;
    }

    return OK;
}


// ����
int
thread_mutex_lock(pthread_mutex_t *mtx)
{
    int  err;

    err = pthread_mutex_lock(mtx);// ����
    if (err == 0) {
        return OK;
    }
    // ��ӡ������Ϣ
	fprintf(stderr,"pthread_mutex_lock() failed, reason: %s\n",strerror(errno));

    return ERROR;
}


// ����
int
thread_mutex_unlock(pthread_mutex_t *mtx)
{
    int  err;

    err = pthread_mutex_unlock(mtx);// ����

#if 0  // ���������0����ִ������Ĵ���
    ngx_time_update();
#endif

    if (err == 0) {
        return OK;
    }
    // ��ӡ������Ϣ
	fprintf(stderr,"pthread_mutex_unlock() failed, reason: %s\n",strerror(errno));
    return ERROR;
}
