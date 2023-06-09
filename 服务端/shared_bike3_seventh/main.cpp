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
	MobileCodeRspEv mcre(200, 666666);
	mcre.dump(cout);
	/*****************获取验证码响应实现****************************/
	UserEventHandler ueh1;
	ueh1.handle(&me);

	/*****************实现获取验证请求处理器****************************/
	/*****************实现线程池的整合****************************/


	/**************实现事件分发线程池相关接口的实现************************/
	DispatchMsgService* DMS = DispatchMsgService::getInstance();
	DMS->open();
	MobileCodeReqEv* pmcre = new MobileCodeReqEv("15690578178");
	DMS->enqueue(pmcre);
	sleep(5);
	DMS->close();
	sleep(5);
	/*********************处理事件分发接口的实现**********************/

	/****************libevent网络接口类头文件声明********************/
	
	/********************网络接口类连接回调实现*******************/
	/********************网络接口类处理请求回调实现*******************/
	/*******************处理请求模块自测*******************/
	return 0;
}