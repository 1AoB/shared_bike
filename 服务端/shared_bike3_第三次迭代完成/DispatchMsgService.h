/***********************************************************
* ����ַ���Ϣ����ģ�飬��ʵ���ǰ��ⲿ�յ�����Ϣ��ת��Ϊ�ڲ��¼���Ҳ����data->msg->event�Ľ������
* Ȼ���ٰ��¼�Ͷ�ݵ��߳ص���Ϣ���У����̳߳ص�����process�������¼����д������յ���ÿ��event��handler����
* ������event,��ʱÿ��event handler��Ҫsubscrive��event��Żᱻ���õ�.
***********************************************************/

#ifndef BRK_SERVICE_DISPATCH_EVENT_SERVICE_H_
#define BRK_SERVICE_DISPATCH_EVENT_SERVICE_H_

#include <map>
#include <vector>
#include <queue>
//forward_list��C++11�����ĵ�����洢��ͬ������ͬ����Ԫ�أ���������õ��ڴ�ռ���٣�
//�ռ������ʸ��ߣ����Ҷ���ʵ��ĳЩ�����������ִ��Ч��Ҳ���ߡ�
#include <forward_list> 
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

	static DispatchMsgService* getInstance();//����ģʽ
	iEvent* parseEvent(const char* message, u32 len, u32 eid);
	void handleAllResponseEvent(NetworkInterface* interface);

	/*��������*/
	static void addClientNumber();//�����ӵĿͻ�������+1
	static void addUserToList(struct bufferevent* user);//�������ӵĿͻ��˲��뵽������
	static void decUserFromList(struct bufferevent* user);//��ָ���Ŀͻ��˴��������Ƴ�
	static std::forward_list<struct bufferevent*> getUserList();//��ȡ����ͻ��˵�����
	//������Ӧ��Ϣ
	void sendPesponseMessage(iEvent* ev, EventID Eid, NetworkInterface* interface);

protected:
	thread_pool_t* tp;

	static DispatchMsgService* DMS_;
	
	

	typedef std::vector<iEventHandler*> T_EventHandlers;//�û��¼���������
	typedef std::map<u32, T_EventHandlers> T_EventHandlersMap;
	T_EventHandlersMap subscribers_;

	bool svr_exit_;//����״̬

	static std::queue<iEvent*> response_events;
	static pthread_mutex_t queue_mutext;
	
	/*��������*/
	static int clientNumber;//�Ѿ����ӵĿͻ�������
	static NetworkInterface* NTIF_;
	static std::forward_list<struct bufferevent*> m_userList;//�������ӵ��Ŀͻ���
};



#endif // !BRK_SERVICE_DISPATCH_EVENT_SERVICE_H_


