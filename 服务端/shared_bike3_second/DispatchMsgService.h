/*********************************************
* ����ַ���Ϣ����ģ�飬��ʵ���ǰ��ⲿ�յ�����Ϣ��ת�����ڲ��¼���Ҳ����data->msg->event�Ľ�����̣�
* Ȼ���ٰ��¼�Ͷ�����߳ص���Ϣ���У����̳߳ص�����process �������¼����д������յ���ÿ��event��handler����
* ������event����ʱÿ��event handler��Ҫsubscribe��event��Żᱻ���õ�.
**********************************************/

#ifndef BRK_SERVICE_DISPATCH_EVENT_SERVICE_H_
#define BRK_SERVICE_DISPATCH_EVENT_SERVICE_H_

#include <map>
#include <vector>
#include <queue>
#include "ievent.h"
#include "eventtype.h"
#include "iEventHandler.h"
#include "threadpool/thread_pool.h"
#include "NetworkInterface.h"

class DispatchMsgService
{
protected:
    DispatchMsgService();
public:

    virtual ~DispatchMsgService();

    virtual BOOL open();
    virtual void close();

    virtual void subscribe(u32 eid, iEventHandler* handler);
    virtual void unsubscribe(u32 eid, iEventHandler* handler);

    //���¼�Ͷ�ݵ��̳߳��н��д���
    virtual i32 enqueue(iEvent* ev);

    //�̳߳ػص�����
    static void svc(void* argv);

    //�Ծ�����¼����зַ�����
    virtual iEvent* process(const iEvent* ev);

    static DispatchMsgService* getInstance();

    iEvent* parseEvent(const char* message, u32 len, u32 eid);

    void handleAllResponseEvent(NetworkInterface* interface);

protected:

    thread_pool_t* tp;

    static DispatchMsgService* DMS_;

    typedef std::vector<iEventHandler*> T_EventHandlers;
    typedef std::map<u32, T_EventHandlers > T_EventHandlersMap;
    T_EventHandlersMap subscribers_;

    bool svr_exit_;

    static std::queue<iEvent*> response_events;
    static pthread_mutex_t   queue_mutex;
};

#endif
