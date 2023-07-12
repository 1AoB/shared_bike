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
	//未来需要实现订阅事件的处理
	DispatchMsgService::getInstance()->subscribe(EEVENTID_GET_MOBLIE_CODE_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_LOGIN_REQ, this);
	thread_mutex_create(&pm_);
}

UserEventHandler::~UserEventHandler()
{
	//未来需要实现退订事件的处理
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
* 根据MobileCodeReqEv事件中的请求信息生成验证码，
* 并将其存储在m2c_映射中，然后创建MobileCodeRspEv类型的事件对象并返回它。
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
* 接收LoginReqEv类型的事件对象，并返回LoginResEv类型的事件对象。
* 这个函数的作用是:
* 根据LoginReqEv事件中的请求信息，检查其提供的登录凭据是否正确，
* 然后创建LoginResEv类型的事件对象并返回它。
*/
LoginResEv* UserEventHandler::handle_login_req(LoginReqEv* ev)
{
	LoginResEv* loginEv = nullptr;//再这里定义是为了统一返回,并释放锁(防止出现忘记释放锁的情况)
	std::string mobile = ev->get_mobile();
	i32 code = ev->get_icode();//这个code是用户给我们的验证码

	LOG_DEBUG("<user_event_handler.cpp> - handle_log_req \
		try to handle login ev,mobile = %s , code = %d\n",
		mobile.c_str(), code);

	thread_mutex_lock(&pm_);
	auto iter = m2c_.find(mobile);//找正确的验证码(在之前的函数中,已经保存在m2c_中了)
	if ((iter != m2c_.end()) && (code != iter->second))
	{
		//验证码无效
		loginEv = new LoginResEv(ERRC_INVALID_DATA);
	}

	thread_mutex_unlock(&pm_);
	if (loginEv)return loginEv;//loginEv被初始化为"被标记了无效数据"的对象(ERRC_INVALID_DATA),直接返回
	//否则,下面就是验证成功的情况

	//如果验证成功,则要判断用户是否在数据库中存在,不存在则插入到用户记录中
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);

	//连接到数据库
	st_env_config conf_args = Iniconfig::getInstnce()->getconfig();//先拿到数据库
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
	if(!us.exist(mobile)){//如果数据库中没有该用户
		result = us.insert(mobile);//就插入该用户
		if(!result){//插入如果不成功,就报错
			LOG_DEBUG("<user_event_handler.cpp> - insert user(%s) to db failed\n",
				mobile.c_str());
			return new LoginResEv(ERRO_PROCCESS_FAILED);
		}
	}
	return new LoginResEv(ERRC_SUCCESS);
}

/*
* 生成一个随机的整数作为验证码
*/
i32 UserEventHandler::code_gen()
{
	i32 code = 0;
	srand((unsigned int)time(NULL));

	code = (unsigned int)(rand() % (999999 - 100000) + 100000);

	return code;
}



