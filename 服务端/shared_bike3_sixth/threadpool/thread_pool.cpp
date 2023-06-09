#include "thread_pool.h"

// ���徲̬����
static void thread_pool_exit_handler(void *data);//�߳��˳����������������߳��˳�ʱ�ͷ������Դ��
static void *thread_pool_cycle(void *data);//�߳�ѭ�����������ڴ���������л�ȡ����ִ�С�
static int_t thread_pool_init_default(thread_pool_t *tpp, char *name);//Ĭ���̳߳س�ʼ�����ã����ڳ�ʼ���̳߳ص�һЩĬ�ϲ�����


// ����ȫ�ֱ���
static uint_t       thread_pool_task_id;

static int debug = 0;
// ��ʼ���̳߳�
thread_pool_t* thread_pool_init()
{
    int             err;
    pthread_t       tid;
    uint_t          n;
    pthread_attr_t  attr;//�߳�����
	thread_pool_t   *tp=NULL;//һ���̳߳�(���Ƕ�����Ǹ��̳߳ذ���4���߳�,��һ���������,���г���Ϊ65535)
    // ����ռ�
	tp =(thread_pool_t*) calloc(1,sizeof(thread_pool_t));

	if(tp == NULL){
	    fprintf(stderr, "thread_pool_init: calloc failed!\n");
	}
    // ��ʼ���̳߳�����
	thread_pool_init_default(tp, NULL);
    // ��ʼ���������
    thread_pool_queue_init(&tp->queue);
    // ��ʼ��������
    if (thread_mutex_create(&tp->mtx) != OK) {
		free(tp);
        return NULL;
    }
    // ��ʼ����������
    if (thread_cond_create(&tp->cond) != OK) {
        (void) thread_mutex_destroy(&tp->mtx);
		free(tp);
        return NULL;
    }
    // ��ʼ���߳�����
    err = pthread_attr_init(&attr);
    if (err) {
        fprintf(stderr, "pthread_attr_init() failed, reason: %s\n",strerror(errno));
		free(tp);
        return NULL;
    }
    // �����̷߳���״̬
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        fprintf(stderr, "pthread_attr_setdetachstate() failed, reason: %s\n",strerror(errno));
		free(tp);
        return NULL;
    }

    // �����߳�
    for (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, thread_pool_cycle, tp);
        if (err) {
            fprintf(stderr, "pthread_create() failed, reason: %s\n",strerror(errno));
			free(tp);
            return NULL;
        }
    }
    // �����߳�����
    (void) pthread_attr_destroy(&attr);

    return tp;
}

// �����̳߳�
void thread_pool_destroy(thread_pool_t *tp)
{
    uint_t           n;
    thread_task_t    task;
    volatile uint_t  lock;//�ڶ��̱߳���У���Щ�������ܻᱻ����̷߳��ʲ��޸ģ������ʹ�� volatile ���������������ܻ�Ա�������һЩ�Ż����绺�������ֵ���Ӷ����ܵ����̼߳�ͨ�ŵĴ���ʹ�� volatile ���������󣬱�������ǿ��Ҫ��ÿ�η��ʸñ���ʱ�����ڴ��ж�ȡ���µ�ֵ���Ӷ�ȷ�����̼߳�ͨ�ŵ���ȷ�ԡ�
    // ��ʼ������
    memset(&task,'\0', sizeof(thread_task_t));
    // ��������������������
    task.handler = thread_pool_exit_handler;// ���̷߳�������,������������ɱ,�����߳�
    task.ctx = (void *) &lock;
    // ���������ÿ���߳�
    for (n = 0; n < tp->threads; n++) {
        lock = 1;//ÿ���̴߳������˳��������Ҫ�� lock �����Ϊ 0���Ա�ʾ�����Ѿ���ɡ�

        if (thread_task_post(tp, &task) != OK) {
            return;
        }

        while (lock) {
            sched_yield();//sched_yield() ��һ��ϵͳ���ã����ڽ� CPU �Ŀ���Ȩ���������ȴ�ִ�еĽ��̻��̡߳���һ���̵߳��� sched_yield() ʱ�������������� CPU ��ʹ��Ȩ���������ȴ�ִ�е��߳��л���ִ�С�
        }

        //task.event.active = 0;
    }
    // �������������ͻ�����
    (void) thread_cond_destroy(&tp->cond);
    (void) thread_mutex_destroy(&tp->mtx);

	free(tp);
}

// �߳��˳�������
static void
thread_pool_exit_handler(void *data)
{
    uint_t *lock = (uint_t*)data;// �� void* ���͵� data ǿ��ת��Ϊ uint_t* ���͵�ָ��

    *lock = 0;// �� lock ָ����ָ���ֵ��Ϊ 0����ʾ�����Ѿ����

    pthread_exit(0);// �˳���ǰ�߳�
}

// ��������ռ�
thread_task_t *
thread_task_alloc(size_t size)
{
    thread_task_t  *task;
    // ����ռ�
    task = (thread_task_t*)calloc(1,sizeof(thread_task_t) + size);
    if (task == NULL) {
        return NULL;
    }
    // ����������
    task->ctx = task + 1;// �������Ŀռ�ָ�� task + 1�����ṹ��֮��ĵ�ַ��

    return task;
}

void thread_task_free(thread_task_t* task)
{
    if (task) {
        free(task);
        task = NULL;
    }
}

// ��������
int_t
thread_task_post(thread_pool_t *tp, thread_task_t *task)
{
    // ����
    if (thread_mutex_lock(&tp->mtx) != OK) {
        return ERROR;
    }
    // �ж϶����Ƿ�����
    if (tp->waiting >= tp->max_queue) {
        (void) thread_mutex_unlock(&tp->mtx);

        fprintf(stderr,"thread pool \"%s\" queue overflow: %ld tasks waiting\n",
                      tp->name, tp->waiting);
        return ERROR;
    }

    //task->event.active = 1;
    // ��������ID����һ������ָ��
    task->id = thread_pool_task_id++;
    task->next = NULL;
    // �������������ź�
    if (thread_cond_signal(&tp->cond) != OK) {
        (void) thread_mutex_unlock(&tp->mtx);
        return ERROR;
    }
    // ������񵽶���ĩβ
    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;
    // ����
    (void) thread_mutex_unlock(&tp->mtx);

    if(debug)fprintf(stderr,"task #%lu added to thread pool \"%s\"\n",
                   task->id, tp->name);

    return OK;
}

// �߳�ѭ������
static void *
thread_pool_cycle(void *data)
{
    thread_pool_t *tp = (thread_pool_t*)data;

    //int                 err;
    thread_task_t       *task;


    if(debug)fprintf(stderr,"thread in pool \"%s\" started\n", tp->name);

   

    for ( ;; ) {
        if (thread_mutex_lock(&tp->mtx) != OK) {// ����
            return NULL;
        }

        
        tp->waiting--;// ���ٵȴ��������

        while (tp->queue.first == NULL) {// �ȴ���������е�����
            if (thread_cond_wait(&tp->cond, &tp->mtx)
                != OK)
            {
                (void) thread_mutex_unlock(&tp->mtx);
                return NULL;
            }
        }
        // ��ȡ�����е�����
        task = tp->queue.first;
        tp->queue.first = task->next;
        // �������Ϊ�գ����ö���ĩβָ��
        if (tp->queue.first == NULL) {
            tp->queue.last = &tp->queue.first;
        }
        // ����
        if (thread_mutex_unlock(&tp->mtx) != OK) {
            return NULL;
        }



        if(debug) fprintf(stderr,"run task #%lu in thread pool \"%s\"\n",
                       task->id, tp->name);
        // ִ����������
        task->handler(task->ctx);

        if(debug) fprintf(stderr,"complete task #%lu in thread pool \"%s\"\n",task->id, tp->name);

        task->next = NULL;
        thread_task_free(task);

        //notify 
    }
}



// Ĭ���̳߳س�ʼ������
static int_t
thread_pool_init_default(thread_pool_t *tpp, char *name)
{
	if(tpp)
    {
        tpp->threads = DEFAULT_THREADS_NUM;// �����߳����Ͷ����������
        tpp->max_queue = DEFAULT_QUEUE_NUM;
            
        // �����̳߳�����
		tpp->name = strdup(name?name:"default");
        if(debug)fprintf(stderr,
                      "thread_pool_init, name: %s ,threads: %lu max_queue: %ld\n",
                      tpp->name, tpp->threads, tpp->max_queue);

        return OK;
    }

    return ERROR;
}




