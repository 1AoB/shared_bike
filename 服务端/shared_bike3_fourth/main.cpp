#include<stdio.h>
#include<iostream>
#include"bike.pb.h"
#include"ievent.h"
#include"events_def.h"
#include "user_event_handler.h"

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


	/**************ʵ���¼��ɷ�������Ķ���************************/
	return 0;
}