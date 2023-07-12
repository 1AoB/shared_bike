#include "user_event_handler.h"
#include "DispatchMsgService.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "sqlconnection.h"
#include "configdef.h"
#include "iniconfig.h"
#include "user_service.h"


UserEventHandler::UserEventHandler() :iEventHandler("UserEventHandler")
{
	//δ����Ҫʵ�ֶ����¼��Ĵ���
	DispatchMsgService::getInstance()->subscribe(EEVENTID_GET_MOBLIE_CODE_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_LOGIN_REQ, this);
	thread_mutex_create(&pm_);
}

UserEventHandler::~UserEventHandler()
{
	//δ����Ҫʵ���˶��¼��Ĵ���
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_GET_MOBLIE_CODE_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_LOGIN_REQ, this);
	thread_mutex_destroy(&pm_);

}


iEvent* UserEventHandler::handle(const iEvent* ev)
{
	if (ev == NULL)
	{
		//LOG_ERROR("input ev is NULL");
		printf("input ev is NULL");
	}

	u32 eid = ev->get_eid();

	if (eid == EEVENTID_GET_MOBLIE_CODE_REQ)
	{
		return handle_mobile_code_req((MobileCodeReqEv*)ev);
	}
	else if (eid == EEVENTID_LOGIN_REQ)
	{
		return handle_login_req((LoginReqEv*) ev);
	}
	else if (eid == EEVENTID_RECHARGE_REQ)
	{
		//return handle_recharge_req((RechargeEv*) ev);
	}
	else if (eid == EEVENTID_GET_ACCOUNT_BALANCE_REQ)
	{
		//return handle_get_account_balance_req((GetAccountBalanceEv*) ev);
	}
	else if (eid == EEVENTID_LIST_ACCOUNT_RECORDS_REQ)
	{
		//return handle_list_account_records_req((ListAccountRecordsReqEv*) ev);
	}

	return NULL;
}
/*
* ����MobileCodeReqEv�¼��е�������Ϣ������֤�룬
* ������洢��m2c_ӳ���У�Ȼ�󴴽�MobileCodeRspEv���͵��¼����󲢷�������
*/
MobileCodeRspEv* UserEventHandler::handle_mobile_code_req(MobileCodeReqEv* ev)
{
	i32  icode = 0;
	std::string mobile_ = ev->get_mobile();
	//LOG_DEBUG("try to get moblie phone %s validate code .", mobile_.c_str());
	printf("try to get moblie phone %s validate code .\n", mobile_.c_str());

	icode = code_gen();
	thread_mutex_lock(&pm_);
	m2c_[mobile_] = icode;
	thread_mutex_unlock(&pm_);
	printf("mobile: %s, code: %d\n", mobile_.c_str(), icode);

	return new MobileCodeRspEv(200, icode);
}
/*
* ����LoginReqEv���͵��¼����󣬲�����LoginResEv���͵��¼�����
* ���������������:
* ����LoginReqEv�¼��е�������Ϣ��������ṩ�ĵ�¼ƾ���Ƿ���ȷ��
* Ȼ�󴴽�LoginResEv���͵��¼����󲢷�������
*/
LoginResEv* UserEventHandler::handle_login_req(LoginReqEv* ev)
{
	LoginResEv* loginEv = nullptr;//�����ﶨ����Ϊ��ͳһ����,���ͷ���(��ֹ���������ͷ��������)
	std::string mobile = ev->get_mobile();
	i32 code = ev->get_icode();//���code���û������ǵ���֤��

	LOG_DEBUG("<user_event_handler.cpp> - handle_log_req \
		try to handle login ev,mobile = %s , code = %d\n",
		mobile.c_str(), code);

	thread_mutex_lock(&pm_);
	auto iter = m2c_.find(mobile);//����ȷ����֤��(��֮ǰ�ĺ�����,�Ѿ�������m2c_����)
	if ((iter != m2c_.end()) && (code != iter->second))
	{
		//��֤����Ч
		loginEv = new LoginResEv(ERRC_INVALID_DATA);
	}

	thread_mutex_unlock(&pm_);
	if (loginEv)return loginEv;//loginEv����ʼ��Ϊ"���������Ч����"�Ķ���(ERRC_INVALID_DATA),ֱ�ӷ���
	//����,���������֤�ɹ������

	//�����֤�ɹ�,��Ҫ�ж��û��Ƿ������ݿ��д���,����������뵽�û���¼��
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);

	//���ӵ����ݿ�
	st_env_config conf_args = Iniconfig::getInstnce()->getconfig();//���õ����ݿ�
	if (!mysqlconn->Init(conf_args.db_ip.c_str(),
		conf_args.db_port,
		conf_args.db_user.c_str(),
		conf_args.db_pwd.c_str(),
		conf_args.db_name.c_str()
	)) {
		LOG_DEBUG("<user_event_handler.cpp> - handle_login_req->Init failed\n");
		return new LoginResEv(ERRO_PROCCESS_FAILED);
	}

	UserService us(mysqlconn);
	bool result = false;
	if(!us.exist(mobile)){//������ݿ���û�и��û�
		result = us.insert(mobile);//�Ͳ�����û�
		if(!result){//����������ɹ�,�ͱ���
			LOG_DEBUG("<user_event_handler.cpp> - insert user(%s) to db failed\n",
				mobile.c_str());
			return new LoginResEv(ERRO_PROCCESS_FAILED);
		}
	}
	return new LoginResEv(ERRC_SUCCESS);
}

/*
* ����һ�������������Ϊ��֤��
*/
i32 UserEventHandler::code_gen()
{
	i32 code = 0;
	srand((unsigned int)time(NULL));

	code = (unsigned int)(rand() % (999999 - 100000) + 100000);

	return code;
}



