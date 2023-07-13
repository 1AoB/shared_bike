#include "thread.h"


// ������������
int
thread_cond_create(pthread_cond_t *cond)
{
    int  err;

    err = pthread_cond_init(cond, NULL);// ��ʼ����������
    if (err == 0) {
        return T_OK;
    }
    // ��ӡ������Ϣ
    fprintf(stderr, "pthread_cond_init() failed, reason: %s\n",strerror(errno));
    return T_ERROR;
}


// ������������
int
thread_cond_destroy(pthread_cond_t *cond)
{
    int  err;

    err = pthread_cond_destroy(cond); // ������������
    if (err == 0) {
        return T_OK;
    }
    // ��ӡ������Ϣ
	fprintf(stderr, "pthread_cond_destroy() failed, reason: %s\n",strerror(errno));
    return T_ERROR;
}

// ���������ź�
int
thread_cond_signal(pthread_cond_t *cond)
{
    int  err;

    err = pthread_cond_signal(cond);// ���������ź�
    if (err == 0) {
        return T_OK;
    }
    // ��ӡ������Ϣ
	fprintf(stderr, "pthread_cond_signal() failed, reason: %s\n",strerror(errno));
    return T_ERROR;
}

// �ȴ���������
int
thread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mtx)
{
    int  err;

    err = pthread_cond_wait(cond, mtx);// �ȴ���������

    if (err == 0) {
        return T_OK;
    }

    // ��ӡ������Ϣ
	fprintf(stderr, "pthread_cond_wait() failed, reason: %s\n",strerror(errno));
    return T_ERROR;
}

