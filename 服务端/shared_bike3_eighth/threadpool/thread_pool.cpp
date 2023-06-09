#include "thread_pool.h"

// 定义静态函数
static void thread_pool_exit_handler(void *data);//线程退出处理函数，用于在线程退出时释放相关资源。
static void *thread_pool_cycle(void *data);//线程循环函数，用于从任务队列中获取任务并执行。
static int_t thread_pool_init_default(thread_pool_t *tpp, char *name);//默认线程池初始化设置，用于初始化线程池的一些默认参数。


// 定义全局变量
static uint_t       thread_pool_task_id;

static int debug = 0;
// 初始化线程池
thread_pool_t* thread_pool_init()
{
    int             err;
    pthread_t       tid;
    uint_t          n;
    pthread_attr_t  attr;//线程属性
	thread_pool_t   *tp=NULL;//一个线程池(我们定义的是该线程池包含4个线程,和一个任务队列,队列长度为65535)
    // 分配空间
	tp =(thread_pool_t*) calloc(1,sizeof(thread_pool_t));

	if(tp == NULL){
	    fprintf(stderr, "thread_pool_init: calloc failed!\n");
	}
    // 初始化线程池设置
	thread_pool_init_default(tp, NULL);
    // 初始化任务队列
    thread_pool_queue_init(&tp->queue);
    // 初始化互斥锁
    if (thread_mutex_create(&tp->mtx) != OK) {
		free(tp);
        return NULL;
    }
    // 初始化条件变量
    if (thread_cond_create(&tp->cond) != OK) {
        (void) thread_mutex_destroy(&tp->mtx);
		free(tp);
        return NULL;
    }
    // 初始化线程属性
    err = pthread_attr_init(&attr);
    if (err) {
        fprintf(stderr, "pthread_attr_init() failed, reason: %s\n",strerror(errno));
		free(tp);
        return NULL;
    }
    // 设置线程分离状态
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        fprintf(stderr, "pthread_attr_setdetachstate() failed, reason: %s\n",strerror(errno));
		free(tp);
        return NULL;
    }

    // 创建线程
    for (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, thread_pool_cycle, tp);
        if (err) {
            fprintf(stderr, "pthread_create() failed, reason: %s\n",strerror(errno));
			free(tp);
            return NULL;
        }
    }
    // 销毁线程属性
    (void) pthread_attr_destroy(&attr);

    return tp;
}

// 销毁线程池
void thread_pool_destroy(thread_pool_t *tp)
{
    uint_t           n;
    thread_task_t    task;
    volatile uint_t  lock;//在多线程编程中，有些变量可能会被多个线程访问并修改，如果不使用 volatile 声明，编译器可能会对变量进行一些优化，如缓存变量的值，从而可能导致线程间通信的错误。使用 volatile 声明变量后，编译器会强制要求每次访问该变量时都从内存中读取最新的值，从而确保了线程间通信的正确性。
    // 初始化任务
    memset(&task,'\0', sizeof(thread_task_t));
    // 设置任务处理函数和上下文
    task.handler = thread_pool_exit_handler;// 给线程分配任务,这个任务就是自杀,销毁线程
    task.ctx = (void *) &lock;
    // 发送任务给每个线程
    for (n = 0; n < tp->threads; n++) {
        lock = 1;//每个线程处理完退出任务后，需要将 lock 标记设为 0，以表示任务已经完成。

        if (thread_task_post(tp, &task) != OK) {
            return;
        }

        while (lock) {
            sched_yield();//sched_yield() 是一个系统调用，用于将 CPU 的控制权交给其他等待执行的进程或线程。当一个线程调用 sched_yield() 时，它会主动放弃 CPU 的使用权，让其他等待执行的线程有机会执行。
        }

        //task.event.active = 0;
    }
    // 销毁条件变量和互斥锁
    (void) thread_cond_destroy(&tp->cond);
    (void) thread_mutex_destroy(&tp->mtx);

	free(tp);
}

// 线程退出处理函数
static void
thread_pool_exit_handler(void *data)
{
    uint_t *lock = (uint_t*)data;// 将 void* 类型的 data 强制转换为 uint_t* 类型的指针

    *lock = 0;// 将 lock 指针所指向的值设为 0，表示任务已经完成

    pthread_exit(0);// 退出当前线程
}

// 分配任务空间
thread_task_t *
thread_task_alloc(size_t size)
{
    thread_task_t  *task;
    // 分配空间
    task = (thread_task_t*)calloc(1,sizeof(thread_task_t) + size);
    if (task == NULL) {
        return NULL;
    }
    // 设置上下文
    task->ctx = task + 1;// 将上下文空间指向 task + 1，即结构体之后的地址。

    return task;
}

void thread_task_free(thread_task_t* task)
{
    if (task) {
        free(task);
        task = NULL;
    }
}

// 发送任务
int_t
thread_task_post(thread_pool_t *tp, thread_task_t *task)
{
    // 加锁
    if (thread_mutex_lock(&tp->mtx) != OK) {
        return ERROR;
    }
    // 判断队列是否已满
    if (tp->waiting >= tp->max_queue) {
        (void) thread_mutex_unlock(&tp->mtx);

        fprintf(stderr,"thread pool \"%s\" queue overflow: %ld tasks waiting\n",
                      tp->name, tp->waiting);
        return ERROR;
    }

    //task->event.active = 1;
    // 设置任务ID和下一个任务指针
    task->id = thread_pool_task_id++;
    task->next = NULL;
    // 发送条件变量信号
    if (thread_cond_signal(&tp->cond) != OK) {
        (void) thread_mutex_unlock(&tp->mtx);
        return ERROR;
    }
    // 添加任务到队列末尾
    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;
    // 解锁
    (void) thread_mutex_unlock(&tp->mtx);

    if(debug)fprintf(stderr,"task #%lu added to thread pool \"%s\"\n",
                   task->id, tp->name);

    return OK;
}

// 线程循环函数
static void *
thread_pool_cycle(void *data)
{
    thread_pool_t *tp = (thread_pool_t*)data;

    //int                 err;
    thread_task_t       *task;


    if(debug)fprintf(stderr,"thread in pool \"%s\" started\n", tp->name);

   

    for ( ;; ) {
        if (thread_mutex_lock(&tp->mtx) != OK) {// 加锁
            return NULL;
        }

        
        tp->waiting--;// 减少等待任务计数

        while (tp->queue.first == NULL) {// 等待任务队列中的任务
            if (thread_cond_wait(&tp->cond, &tp->mtx)
                != OK)
            {
                (void) thread_mutex_unlock(&tp->mtx);
                return NULL;
            }
        }
        // 获取队列中的任务
        task = tp->queue.first;
        tp->queue.first = task->next;
        // 如果队列为空，设置队列末尾指针
        if (tp->queue.first == NULL) {
            tp->queue.last = &tp->queue.first;
        }
        // 解锁
        if (thread_mutex_unlock(&tp->mtx) != OK) {
            return NULL;
        }



        if(debug) fprintf(stderr,"run task #%lu in thread pool \"%s\"\n",
                       task->id, tp->name);
        // 执行任务处理函数
        task->handler(task->ctx);

        if(debug) fprintf(stderr,"complete task #%lu in thread pool \"%s\"\n",task->id, tp->name);

        task->next = NULL;
        thread_task_free(task);

        //notify 
    }
}



// 默认线程池初始化设置
static int_t
thread_pool_init_default(thread_pool_t *tpp, char *name)
{
	if(tpp)
    {
        tpp->threads = DEFAULT_THREADS_NUM;// 设置线程数和队列最大容量
        tpp->max_queue = DEFAULT_QUEUE_NUM;
            
        // 设置线程池名称
		tpp->name = strdup(name?name:"default");
        if(debug)fprintf(stderr,
                      "thread_pool_init, name: %s ,threads: %lu max_queue: %ld\n",
                      tpp->name, tpp->threads, tpp->max_queue);

        return OK;
    }

    return ERROR;
}




