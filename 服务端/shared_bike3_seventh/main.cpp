#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include"bike.pb.h"
#include"ievent.h"
#include"events_def.h"
#include "user_event_handler.h"
#include "DispatchMsgService.h"

using namespace std;

int main()
{
	tutorial::mobile_request msg;
	msg.set_mobile("15690578178");
	cout << msg.mobile()<<endl;

	printf("this is shared_bike3\n");
	/*****************�����****************************/

	iEvent* ie = new iEvent(EEVENTID_GET_MOBLIE_CODE_REQ,2);//2��������
	cout << ie->get_eid() << endl;
	cout<<ie->get_sn()<<endl;
	/*****************�¼���Ķ�����ʵ��********************/
	MobileCodeReqEv me("15690578178");
	cout << msg.mobile() << endl;
	cout << me.get_mobile() << endl;
	cout<<me.get_sn() << endl;
	me.dump(cout);
	/*****************��ȡ�ֻ���֤���¼�ʵ��****************************/
	MobileCodeRspEv mcre(200, 666666);
	mcre.dump(cout);
	/*****************��ȡ��֤����Ӧʵ��****************************/
	UserEventHandler ueh1;
	ueh1.handle(&me);

	/*****************ʵ�ֻ�ȡ��֤��������****************************/
	/*****************ʵ���̳߳ص�����****************************/


	/**************ʵ���¼��ַ��̳߳���ؽӿڵ�ʵ��************************/
	DispatchMsgService* DMS = DispatchMsgService::getInstance();
	DMS->open();
	MobileCodeReqEv* pmcre = new MobileCodeReqEv("15690578178");
	DMS->enqueue(pmcre);
	sleep(5);
	DMS->close();
	sleep(5);
	/*********************�����¼��ַ��ӿڵ�ʵ��**********************/

	/****************libevent����ӿ���ͷ�ļ�����********************/
	
	/********************����ӿ������ӻص�ʵ��*******************/
	/********************����ӿ��ദ������ص�ʵ��*******************/
	/*******************��������ģ���Բ�*******************/
	return 0;
}