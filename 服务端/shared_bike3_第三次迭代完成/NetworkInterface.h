#ifndef __NETWORKINTERFACE_H_
#define __NETWORKINTERFACE_H_

#include <event.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <string>

#include "glo_def.h"
#include "ievent.h"

#define MESSAGE_HEADER_LEN 10
#define MESSAGE_HEADER_ID "FBEB"

enum class SESSION_STATUS
{
	SS_REQUEST,//request
	SS_RESPONSE//response
};

enum class MESSAGE_STATUS
{
	MS_READ_HEADER = 0,
	MS_READ_MESSAGE = 1, //��Ϣ����δ��ʼ
	MS_READ_DONE = 2,	 //��Ϣ�������
	MS_SENDING = 3		 //��Ϣ������
};

typedef struct _ConnectSession {
	char remote_ip[32];

	SESSION_STATUS session_stat;

	iEvent* request;
	MESSAGE_STATUS req_stat;

	iEvent* response;
	MESSAGE_STATUS res_stat;

	i32 eid;	//���浱ǰ������¼�id
	i32 fd;		//���浱ǰ���͵��ļ����

	struct bufferevent* bev;//����ȥ
	u32 message_len;        //�Ѿ���д��Ϣ�ĳ���
	u32 read_message_len;   //�Ѿ���ȡ����Ϣ����

	char* read_buf; //�������Ϣ�Ļ�����
	char header[MESSAGE_HEADER_LEN + 1];//����ͷ����10�ֽ�+1�ֽ�
	char* write_buf;
	u32 sent_len;   //�Ѿ����͵ĳ���
	u32 read_header_len; //�Ѷ�ȡ��ͷ������
}ConnectSession;

class NetworkInterface
{
public:
	NetworkInterface();
	~NetworkInterface();

	bool start(int port);
	void close();

	static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd,
		struct sockaddr* sock, int socklen, void* arg);

	static void handle_request(struct bufferevent* bev, void* arg); //������ص�
	static void handle_response(struct bufferevent* bev, void* arg);//�ظ�����ص�
	static void handle_error(struct bufferevent* bev, short event, void* arg);

	void network_event_dispatch();
	void send_response_message(ConnectSession* cs);

	/*����*/
	void session_reset(ConnectSession* cs);

private:
	struct evconnlistener* listener_;
	struct event_base* base_;

	ConnectSession* m_cs;
};

#endif // !BIK_INTERFACE_NETWORK_INTERFACE_H_