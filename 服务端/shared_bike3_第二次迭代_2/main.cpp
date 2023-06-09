#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include"bike.pb.h"
#include"ievent.h"
#include"events_def.h"
#include "user_event_handler.h"
#include "DispatchMsgService.h"
#include "NetworkInterface.h"
#include "iniconfig.h"
#include "Logger.h"
#include "sqlconnection.h"
#include "BusProcessor.h"
#include <memory>

using namespace std;

int main(int argc , char ** argv)
{
	if (argc != 3)
	{
		printf("Please input shbk <conigd file path> <log file config>!\n");
		return -1;
	}

	if (!Logger::instance()->init(std::string(argv[2])))
	{
		fprintf(stderr,"init log module failed.\n");
		return -2;
	}

	Iniconfig config;
	if (!config.loadfile(std::string(argv[1])))
	{
		//printf("load %s failed\n",argv[1]);
		LOG_ERROR("load %s failed.\n", argv[1]);
		Logger::instance()->GetHandle()->error("load %s failed.\n",argv[1]);
		return -3;
	}

	st_env_config conf_args = config.getconfig();
	LOG_INFO("[database] ip:%s  port:%d  user:%s pwd:%s db:%s [server]port:%d\n",
		conf_args.db_ip.c_str(),conf_args.db_port,
		conf_args.db_user.c_str(),conf_args.db_pwd.c_str(),
		conf_args.db_name.c_str(),conf_args.svr_port);


	//tutorial::mobile_request msg;
	//msg.set_mobile("15690578178");
	//cout << msg.mobile()<<endl;

	//printf("this is shared_bike3\n");
	/*****************环境搭建****************************/

	//iEvent* ie = new iEvent(EEVENTID_GET_MOBLIE_CODE_REQ,2);//2是随便填的
	//cout << ie->get_eid() << endl;
	//cout<<ie->get_sn()<<endl;
	/*****************事件类的定义与实现********************/
	//MobileCodeReqEv me("15690578178");
	//cout << msg.mobile() << endl;
	//cout << me.get_mobile() << endl;
	//cout<<me.get_sn() << endl;
	//me.dump(cout);
	/*****************获取手机验证码事件实现****************************/
	//MobileCodeRspEv mcre(200, 666666);
	//mcre.dump(cout);
	/*****************获取验证码响应实现****************************/
	
	//MysqlConnection mysqlConn;
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	if (!mysqlconn->Init(conf_args.db_ip.c_str(),
		conf_args.db_port,
		conf_args.db_user.c_str(),
		conf_args.db_pwd.c_str(),
		conf_args.db_name.c_str()
	)) {
		LOG_ERROR("<main.cpp> mysqlconn->Init failed.\n");
		return  -4;
	}
	BusinessProcessor busPro(mysqlconn);
	busPro.init();//这个函数是创建两个表

	//UserEventHandler ueh1;
	//ueh1.handle(&me);

	/*****************实现获取验证请求处理器****************************/
	/*****************实现线程池的整合****************************/


	/**************实现事件分发线程池相关接口的实现************************/
	DispatchMsgService* DMS = DispatchMsgService::getInstance();
	DMS->open();
	//MobileCodeReqEv* pmcre = new MobileCodeReqEv("15690578178");
	//DMS->enqueue(pmcre);
	//sleep(5);
	//DMS->close();
	//sleep(5);
	/*********************处理事件分发接口的实现**********************/

	/****************libevent网络接口类头文件声明********************/
	
	/********************网络接口类连接回调实现*******************/
	/********************网络接口类处理请求回调实现*******************/
	NetworkInterface* NTF = new NetworkInterface();
	NTF->start(8888);
	while (1 == 1)
	{
		NTF->network_event_dispatch();
		sleep(1);
		LOG_DEBUG("network_event_dispatch...\n");
	}

	sleep(5);
	DMS->close();
	sleep(5);
	/**********************处理请求模块自测***********************/
	/**************实现所有响应事件队列处理************************/
	/*****************************用户注册*************************/
	/************************随机码生成***************************/
	return 0;
}

/*
* ./shared_bike3.out /home/wxncom/projects/conf/shared_bike.ini /home/wxncom/projects/conf/log.conf 运行项目
* echo "">  /var/log/shared_bike.log 清掉日志
* tail -f /var/log/shared_bike.log 打印日志
* 
* 如果你的log4cpp无法打印日志,一定是权限不够 直接执行chmod 777 /var/log/shared_bike.log 权限给到最大
*/