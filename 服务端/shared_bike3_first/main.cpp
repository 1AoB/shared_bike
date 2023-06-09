#include<stdio.h>
#include<iostream>
#include"bike.pb.h"
#include"ievent.h"
#include"events_def.h"

using namespace std;

int main()
{
	tutorial::mobile_request msg;
	msg.set_mobile("15690578178");
	cout << msg.mobile()<<endl;

	printf("this is shared_bike3\n");
	/*****************环境搭建****************************/

	iEvent* ie = new iEvent(EEVENTID_GET_MOBLIE_CODE_REQ,2);//2是随便填的
	cout << ie->get_eid() << endl;
	cout<<ie->get_sn()<<endl;
	/*****************事件类的定义与实现********************/
	MobileCodeReqEv me("15690578178");
	cout << msg.mobile() << endl;
	cout << me.get_mobile() << endl;
	cout<<me.get_sn() << endl;
	me.dump(cout);
	/*****************获取手机验证码事件实现****************************/

	/*****************获取验证码响应实现****************************/
	return 0;
}