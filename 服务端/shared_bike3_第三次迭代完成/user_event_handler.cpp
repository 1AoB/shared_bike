#include "user_event_handler.h"
#include "DispatchMsgService.h"
#include "sqlconnection.h"
#include "iniconfig.h"
#include "user_service.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>

/*���򲹳�ķ�װ����*/
//�����ݿ�
/*
* ����ָ������Ŀ�е�Ӧ������:���ڽ��������ݿ������
*/
bool UserEventHandler::openDataBase(std::shared_ptr<MysqlConnection>& mysqlconn)//�Թ���ָ�����ʽ����
{
	st_env_config conf_args = Iniconfig::getInstance()->getconfig();
	return (mysqlconn->Init(conf_args.db_ip.c_str(),
		conf_args.db_port,
		conf_args.db_user.c_str(),
		conf_args.db_pwd.c_str(),
		conf_args.db_name.c_str())
		);
}

//���ö����߷�����ģʽ,���ڹ��캯���н�Ҫ���ĵ��ඩ��һ��
UserEventHandler::UserEventHandler() :iEventHandler("UserEventHandler")
{
	//δ����Ҫ�����¼��Ĵ���
	DispatchMsgService::getInstance()->subscribe(EEVENTID_GET_MOBILE_CODE_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_REGISTER_USER_REQ, this);

	DispatchMsgService::getInstance()->subscribe(EEVENTID_LOGIN_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_ADDBIKE_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_DELETEBIKE_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_SCANQRSTART_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_SCANQREND_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_FAULT_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_REPAIRFINISH_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_BROADCAST_MSG_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_LIST_BIKEINFO_REQ, this);

	thread_mutex_create(&pm_);
}

UserEventHandler::~UserEventHandler()
{
	//δ����Ҫʵ���˶��¼��Ĵ���
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_GET_MOBILE_CODE_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_REGISTER_USER_REQ, this);

	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_LOGIN_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_ADDBIKE_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_DELETEBIKE_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_SCANQRSTART_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_SCANQREND_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_FAULT_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_REPAIRFINISH_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_BROADCAST_MSG_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_LIST_BIKEINFO_REQ, this);
	
	thread_mutex_destroy(&pm_);
}
//�������˸����¼�Id����������������Ӧ���ظ��ͻ���
iEvent* UserEventHandler::handle(const iEvent* ev)
{
	u32 eid = ev->get_eid();
	switch (eid)//ͨ��eid�ж����ĸ�����,��һ���ǵ����ĸ�������
	{
	case EEVENTID_GET_MOBILE_CODE_REQ:
		return handle_mobile_code_req((MobileCodeReqEv*)ev);
		break;
	case EEVENTID_REGISTER_USER_REQ:
		return handle_user_register_req((RegisterReqEv*)ev);
		break;
	case EEVENTID_LOGIN_REQ:
		return handle_login_req((LoginReqEv*)ev);
		break;
	case EEVENTID_SCANQRSTART_REQ:
		return handle_bike_scanQRSatrt_req((bikeScanQRStartReqEv*)ev);
		break;
	case EEVENTID_SCANQREND_REQ:
		return handle_bike_scanQREnd_req((bikeScanQREndReqEv*)ev);
		break;
	case EEVENTID_FAULT_REQ:
		return handle_bike_scanQRFault_req((bikeScanQRFaultReqEv*)ev);
		break;
	case EEVENTID_REPAIRFINISH_REQ:
		return handle_bike_scanQRRepairFinish_req((bikeScanQRRepairFinishReqEv*)ev);
		break;
	case EEVENTID_BROADCAST_MSG_REQ:
		return handle_broadcast_msg_req((broadcastMsgReqEv*)ev);
		break;
	case EEVENTID_LIST_BIKEINFO_REQ:
		return handle_bikeInfo_req((bikeInfoReqEv*)ev);
		break;
	case EEVENTID_LIST_ACCOUNT_RECORDS_REQ:
		return handle_list_account_records_req((ListAccountRecordsReqEv*)ev);
		break;
	case EEVENTID_ADDBIKE_REQ:
		return handle_addBike_req((AddBikeReqEv*)ev);
		break;
	case EEVENTID_DELETEBIKE_REQ:
		return handle_deleteBike_req((DeleteBikeReqEv*)ev);
		break;
	default:
		break;
	}
	return NULL;
}

//	handle_mobile_code_req((MobileCodeReqEv*)ev);
MobileCodeRspEv* UserEventHandler::handle_mobile_code_req(MobileCodeReqEv* ev)
{
	i32 verCode = 0;
	std::string userName = ev->get_userName();
	LOG_DEBUG("try to get moblie phone. %s validate code .", userName.c_str());
	LOG_DEBUG("����Ϊ�û�[%s]���������֤��......", userName.c_str());

	//printf("try to get moblie phone %s validate code .\n", mobile_.c_str());
	verCode = code_gen();
	thread_mutex_lock(&pm_);
	//auto iter = m2c_.find(mobile_);
	m2c_[userName] = verCode;
	thread_mutex_unlock(&pm_);
	//printf("mobile:%s, code:%d\n", mobile_.c_str(), icode);
	LOG_DEBUG("�û�[%s]��֤��[%d]�����ɣ�", userName.c_str(), verCode);

	return new MobileCodeRspEv(ERRC_SUCCESS, verCode);
}

RegisterResEv* UserEventHandler::handle_user_register_req(RegisterReqEv* ev)
{
	std::string userName = ev->get_userName();//ȡ���û���/�ֻ�����
	std::string userPwd = ev->get_userPwd();
	i32 verCode = ev->get_verCode();

	const char* str_userName = userName.c_str();
	//i32 icode = ev->get_icode();//�ó���֤��
	LOG_DEBUG("<user_event_handler.cpp>-handle_user_register_req:����ƥ���û�[%s]->��֤��[%d]......", str_userName, verCode);
	thread_mutex_lock(&pm_);

	auto iter = m2c_.find(userName);
	if (iter == m2c_.end())
	{
		return NULL;
	}
	//��֤���Ƿ�ƥ��
	if (((iter != m2c_.end()) && (verCode != iter->second)) || (iter == m2c_.end()))
	{
		LOG_WARN("<user_event_handler.cpp>-handle_user_register_req:�û�[%s]����֤��[%d]ƥ��ʧ�ܣ�\n", str_userName, verCode);
		thread_mutex_unlock(&pm_);
		return new RegisterResEv(userName, userPwd, ERRC_INVALID_DATA);
	}
	thread_mutex_unlock(&pm_);
	LOG_DEBUG("�û�[%s]��֤��[%d]ƥ��ɹ���", str_userName, verCode);

	//�������ݿ�
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	LOG_DEBUG("UserEventHandler::handle_user_register_req: Opening Database.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str())
	{
		LOG_ERROR("UserEventHandler::handle_user_register_req - Database init failed. exit!\n");
		return new RegisterResEv(userName, userPwd, ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�����ݿ�ɹ���");
	UserService us(mysqlconn);
	bool result = false;

	if (!us.exist(userName))
	{
		LOG_DEBUG("���û�Ϊ���������û�������Ϊ���û�[%s]��ʼ���˻�......", userName.c_str());
		//std::cout << "us.insert(mobile)=" << mobile << std::endl;
		result = us.insert(userName, userPwd);
		if (!result)
		{
			LOG_DEBUG("<user_event_handler.cpp>-handle_user_register)req:���û�[%s]��Ϣ��ʼ��ʧ�ܣ�", userName.c_str());
			return new RegisterResEv(userName, userPwd, ERRO_PROCCESS_FALED);
		}
		LOG_DEBUG("���û�[%s]��Ϣע����ɣ���Ϣ��¼�����ݿ�.", userName.c_str());
		return new RegisterResEv(userName, userPwd, ERRC_SUCCESS);
	}
	else
	{
		LOG_DEBUG("�û�[%s]��ע�ᣬ����ע������", userName.c_str());
		return new RegisterResEv(userName, userPwd, ERRC_INVALID_MSG);
	}

}

//��ӵ�������
AddBikeResEv* UserEventHandler::handle_addBike_req(AddBikeReqEv* ev)
{
	const i32 bikeid = ev->get_bikeId();
	const r64 longitude = ev->get_longitude();
	const r64 latitude = ev->get_latitude();

	//�������ݿ�(����ָ���ʹ�ó���)
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	LOG_DEBUG("���ڴ����ݿ�.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str())
	{
		LOG_ERROR("UserEventHandler::handle_addBike_req - Database init failed. exit!\n");
		return new AddBikeResEv(bikeid, longitude, latitude, ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�����ݿ�ɹ���");
	UserService us(mysqlconn);

	//�������ݿ����ǲ����е���
	char sql_content[1024];
	sprintf(sql_content, "SELECT devno FROM bikeinfo");//�����豸��devno
	SqlRecordSet record_set;
	if (!us.findBikeInfo(sql_content, record_set))
	{
		LOG_DEBUG("<user_event_handle.cpp>-handleaddBike-req :sql[%s]���ִ��ʧ�ܣ�������Ϣ��%s", sql_content, mysqlconn->GetErrInfo());
		return new AddBikeResEv(bikeid, longitude, latitude, ERRO_PROCCESS_FALED);
	}

	MYSQL_ROW row = record_set.FetchRow();
	i32 maxBikeId = 0;

	if (row)//�Ѿ����豸��bikeInfo��
	{
		memset(sql_content, '\0', sizeof(sql_content));
		sprintf(sql_content, "SELECT MAX(devno) FROM bikeinfo");
		SqlRecordSet record_set1;
		if (!us.findBikeInfo(sql_content, record_set1))
		{
			LOG_DEBUG("��ѯ������ϢMAX(devno)ʧ�ܣ�������Ϣ��%s", mysqlconn->GetErrInfo());
			return new AddBikeResEv(bikeid, longitude, latitude, ERRO_PROCCESS_FALED);
		}
		MYSQL_ROW row1 = record_set1.FetchRow();
		maxBikeId = atoi(row1[0]) + 1;//�ַ���ת��int + 1 
	}
	else {//û���豸��bikeInfo��

		maxBikeId = 1;
	}

	if (!us.addBike(maxBikeId, longitude, latitude))
	{
		LOG_DEBUG("<user_event_handle.cpp>-handle_addBike_req :��ӵ���[%d]��Ϣʧ��", maxBikeId);
		return new AddBikeResEv(maxBikeId, longitude, latitude, ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("��ӵ���[%d]����γ��(%lf, %lf)�ɹ���", maxBikeId, longitude, latitude);
	return new AddBikeResEv(maxBikeId, longitude, latitude, ERRC_SUCCESS);
}

DeleteBikeResEv* UserEventHandler::handle_deleteBike_req(DeleteBikeReqEv* ev)
{
	//��ȡ��Ҫɾ���ĵ���id
	const i32 bikeid = ev->get_bikeId();
	//�������ݿ�
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	LOG_DEBUG("���ڴ����ݿ�.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str())
	{
		LOG_ERROR("<user_event_handle.cpp>:UserEventHandler::handle_deleteBike_req: - Database init failed. exit!\n");
		return new DeleteBikeResEv(bikeid, ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�����ݿ�ɹ���");


	//ɾ������
	UserService us(mysqlconn);//�൱���ǽ����ݿ����Ӿ��������,�������ҵ�����

	if (!us.deleteBike(bikeid))
	{
		LOG_DEBUG("<user_event_handle.cpp>:UserEventHandler::handle_deleteBike_req:�Ƴ�����[%d]ʧ��", bikeid);
		return new DeleteBikeResEv(bikeid, ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�Ƴ�����[%d]�ɹ���", bikeid);
	return new DeleteBikeResEv(bikeid, ERRC_SUCCESS);
}



i32 UserEventHandler::code_gen()
{
	i32 code = 0;
	srand((unsigned int)time(NULL));

	code = (unsigned int)(rand() % (999999 - 100000) + 100000);
	LOG_DEBUG("�����֤�������ɣ�");
	return code;
}

LoginResEv* UserEventHandler::handle_login_req(LoginReqEv* ev)
{
	std::string userName = ev->get_userName();//ȡ���û���/�ֻ�����
	std::string userPwd = ev->get_userPwd();


	//�������ݿ�
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);//���ݿ����ӵĶ���mysqlconnection
	LOG_DEBUG("���ڴ����ݿ�.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str())
	{
		LOG_ERROR("<user_event_handle.cpp> UserEventHandler::handle_login_req - Database init failed. exit!\n");
		return new LoginResEv(userName, userPwd, 0, ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�����ݿ�ɹ���");

	UserService us(mysqlconn);
	if (!us.verifyUserInfo(userName, userPwd))
	{
		LOG_DEBUG("<user_event_handle.cpp>  �û��˺�[%s]������[%s]��Ϣƥ��ʧ��!\n", userName.c_str(), userPwd.c_str());
		return new LoginResEv(userName, userPwd, 0, ERRC_INVALID_MSG);;
	}
	//�ж��ǲ��ǹ���Ա�û�
	if (userName == "root" && userPwd == "root") {
		LOG_DEBUG("����Ա[%s]�ѵ�¼!\n", userName.c_str());
		return new LoginResEv(userName, userPwd, 1, ERRC_SUCCESS);
	}
	//��ͨ�û�
	LOG_DEBUG("�û�[%s]��½�ɹ�!\n", userName.c_str());
	return new LoginResEv(userName, userPwd, 0, ERRC_SUCCESS);;
}

//�����û�ɨ��������������ݽ��д���������ɨ��������Ӧ�¼�
bikeScanQRStartResponseEv* UserEventHandler::handle_bike_scanQRSatrt_req(bikeScanQRStartReqEv* ev)
{
	//�����ݿ�
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	LOG_DEBUG("���ڴ����ݿ�.....handle_bike_scanQRSatrt_req");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str())
	{
		LOG_ERROR("UserEventHandler::handle_login_req - Database init failed. exit!\n");
		return new bikeScanQRStartResponseEv(ERRO_PROCCESS_FALED, 0, 0, 0);
	}

	LOG_DEBUG("�����ݿ�ɹ���");

	UserService us(mysqlconn);

	const double longitude = ev->get_longitude();

	char sql_content_ride_record[1024];
	char sql_content_updataBikeinfo[1024];
	//���µ�����Ϣ��
	sprintf(sql_content_updataBikeinfo, "UPDATE bikeinfo SET status=%d, tmsg = 'bike ride start, riding......', \ 
		time = FROM_UNIXTIME(% d, '%%Y-%%m-%%d %%H:%%i:%%S'), bike_point = GeomFromText('POINT(%lf %lf)') WHERE devno = % d",
		BIKE_RIDE_START, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude(), ev->get_bikeId());
	//�����û����м�¼��
	sprintf(sql_content_ride_record, "insert into user_ride_record_info(username,bikeId,bikeStatus,start_time, start_point) \
		values(\"%s\", %d, %d, FROM_UNIXTIME(%d, '%%Y-%%m-%%d %%H:%%i:%%S'),GeomFromText('POINT(%lf %lf)'))",
		ev->get_userName().c_str(), ev->get_bikeId(), BIKE_RIDE_START, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude());
	LOG_DEBUG("sql_content:[%s]", sql_content_ride_record);

	//ִ��sql���
	if (!us.insertUseRideRecord(sql_content_ride_record))
	{
		LOG_DEBUG("�û����м�¼����user_ride_record_infoʧ�ܣ�");
		LOG_DEBUG("������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeScanQRStartResponseEv(ERRO_PROCCESS_FALED, 0, 0, 0);
	}
	if (!us.updataBikeinfo(sql_content_updataBikeinfo))
	{
		LOG_DEBUG("����״̬bikeinfo��Ϣ����ʧ�ܣ�");
		LOG_DEBUG("������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeScanQRStartResponseEv(ERRO_PROCCESS_FALED, 0, 0, 0);
	}
	LOG_DEBUG("�û����м�¼����user_ride_record_info�ɹ�������״̬bikeinfo��Ϣ�Ѹ��£�");
	return new bikeScanQRStartResponseEv(ERRC_SUCCESS, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude());;
}

//�����û�ɨ�뻹����������ݽ��д���������ɨ�뻹����Ӧ�¼�
bikeScanQREndResponseEv* UserEventHandler::handle_bike_scanQREnd_req(bikeScanQREndReqEv* ev)
{
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	//_st_env_config conf_args = Iniconfig::getInstance()->getconfig();
	LOG_DEBUG("���ڴ����ݿ�.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str()))
	{
		LOG_ERROR("UserEventHandler::handle_login_req - Database init failed. exit!\n");
		return new bikeScanQREndResponseEv(ERRO_PROCCESS_FALED, 0, 0, 0);
	}
	LOG_DEBUG("�����ݿ�ɹ���handle_bike_scanQREnd_req");

	UserService us(mysqlconn);
	char sql_content_ride_record[1024];
	char sql_content_updataBikeinfo[1024];

	LOG_DEBUG("���ڸ��µ�����Ϣ...");
	sprintf(sql_content_updataBikeinfo, "UPDATE bikeinfo SET status=%d, tmsg= 'bike ride end, waiting for use', \ 
		time = FROM_UNIXTIME(% d, '%%Y-%%m-%%d %%H:%%i:%%S'), bike_point = GeomFromText('POINT(%lf %lf)') WHERE devno = % d",
		BIKE_RIDE_END, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude(), ev->get_bikeId());
	LOG_DEBUG("���ڸ����û����м�¼...");
	sprintf(sql_content_ride_record, "insert into user_ride_record_info(username,bikeId,bikeStatus,end_time, end_point) \
		values(\"%s\", %d, %d, FROM_UNIXTIME(%d, '%%Y-%%m-%%d %%H:%%i:%%S'),GeomFromText('POINT(%lf %lf)'))",
		ev->get_userName().c_str(), ev->get_bikeId(), BIKE_RIDE_END, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude());

	if (!us.insertUseRideRecord(sql_content_ride_record))
	{
		LOG_DEBUG("�������ݿ�user_ride_record_infoʧ�ܣ�������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeScanQREndResponseEv(ERRO_PROCCESS_FALED, 0, 0, 0);
	}
	if (!us.updataBikeinfo(sql_content_updataBikeinfo))
	{
		LOG_DEBUG("����״̬bikeinfo��Ϣ����ʧ�ܣ�");
		LOG_DEBUG("������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeScanQREndResponseEv(ERRO_PROCCESS_FALED, 0, 0, 0);
	}
	LOG_DEBUG("������Ϣ���û����м�¼���³ɹ���������ɨ��������Ӧ�¼�");

	return new bikeScanQREndResponseEv(ERRC_SUCCESS, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude());
}

//�����û�ɨ�뱨����������ݽ��д���������ɨ�뱨����Ӧ�¼�
bikeScanQRFaultResponseEv* UserEventHandler::handle_bike_scanQRFault_req(bikeScanQRFaultReqEv* ev)
{
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	//_st_env_config conf_args = Iniconfig::getInstance()->getconfig();
	LOG_DEBUG("���ڴ����ݿ�.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str()))
	{
		LOG_ERROR("UserEventHandler::handle_login_req - Database init failed. exit!\n");
		return new bikeScanQRFaultResponseEv(ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�����ݿ�ɹ���");

	UserService us(mysqlconn);

	char sql_content_ride_record[1024];
	char sql_content_updataBikeinfo[1024];

	sprintf(sql_content_updataBikeinfo, "UPDATE bikeinfo SET status=%d, tmsg = 'bike fault, waiting for repair......', \ 
		time = FROM_UNIXTIME(% d, '%%Y-%%m-%%d %%H:%%i:%%S'), bike_point = GeomFromText('POINT(%lf %lf)') WHERE devno = % d",
		BIKE_FAULT, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude(), ev->get_bikeId());

	sprintf(sql_content_ride_record, "insert into user_ride_record_info(username,bikeId,bikeStatus,start_time, start_point) \
		values(\"%s\", %d, %d, FROM_UNIXTIME(%d, '%%Y-%%m-%%d %%H:%%i:%%S'),GeomFromText('POINT(%lf %lf)'))",
		ev->get_userName().c_str(), ev->get_bikeId(), BIKE_FAULT, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude());

	if (!us.insertUseRideRecord(sql_content_ride_record))
	{
		LOG_DEBUG("�����û���¼��bikeinfoʧ�ܣ�������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeScanQRFaultResponseEv(ERRO_PROCCESS_FALED);
	}
	if (!us.updataBikeinfo(sql_content_updataBikeinfo))
	{
		LOG_DEBUG("�������ݱ�bikeinfoʧ�ܣ�������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeScanQRFaultResponseEv(ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�û�������Ϣbikeinfo�ɹ���");

	return new bikeScanQRFaultResponseEv(ERRC_SUCCESS);
}

//�����û�ɨ�뱨�������������ݽ��д���������ɨ�뱨�������Ӧ�¼�
bikeScanQRRepairFinishResponseEv* UserEventHandler::handle_bike_scanQRRepairFinish_req(bikeScanQRRepairFinishReqEv* ev)
{
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	//_st_env_config conf_args = Iniconfig::getInstance()->getconfig();
	LOG_DEBUG("���ڴ����ݿ�.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str()))
	{
		LOG_ERROR("UserEventHandler::handle_login_req - Database init failed. exit!\n");
		return new bikeScanQRRepairFinishResponseEv(ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�����ݿ�ɹ���");

	UserService us(mysqlconn);

	char sql_content_ride_record[1024];
	char sql_content_updataBikeinfo[1024];

	sprintf(sql_content_updataBikeinfo, "UPDATE bikeinfo SET status=%d, tmsg = 'bike repair finish, waiting for use', \ 
		time = FROM_UNIXTIME(% d, '%%Y-%%m-%%d %%H:%%i:%%S'), bike_point = GeomFromText('POINT(%lf %lf)') WHERE devno = % d",
		BIKE_REPAIR_FINISH, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude(), ev->get_bikeId());

	sprintf(sql_content_ride_record, "insert into user_ride_record_info(username,bikeId,bikeStatus,end_time, end_point) \
		values(\"%s\", %d, %d, FROM_UNIXTIME(%d, '%%Y-%%m-%%d %%H:%%i:%%S'),GeomFromText('POINT(%lf %lf)'))",
		ev->get_userName().c_str(), ev->get_bikeId(), BIKE_REPAIR_FINISH, ev->get_timestamp(), ev->get_longitude(), ev->get_latitude());

	if (!us.insertUseRideRecord(sql_content_ride_record))
	{
		LOG_DEBUG("�����û���¼��user_ride_record_infoʧ�ܣ�������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeScanQRRepairFinishResponseEv(ERRO_PROCCESS_FALED);
	}

	if (!us.updataBikeinfo(sql_content_updataBikeinfo))
	{
		LOG_DEBUG("�������ݱ�bikeinfoʧ�ܣ�������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeScanQRRepairFinishResponseEv(ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�û�������Ϣbikeinfo�ɹ���");

	return new bikeScanQRRepairFinishResponseEv(ERRC_SUCCESS);
}

broadcastMsgResponseEv* UserEventHandler::handle_broadcast_msg_req(broadcastMsgReqEv* ev)
{
	return new broadcastMsgResponseEv(ev->get_bikeid(), ev->get_bikestatus(), ev->get_bikeLongitude(), ev->get_bikeLatitude());
}

bikeInfoResponseEv* UserEventHandler::handle_bikeInfo_req(bikeInfoReqEv* ev)
{

	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	//_st_env_config conf_args = Iniconfig::getInstance()->getconfig();
	LOG_DEBUG("���ڴ����ݿ�.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str()))
	{
		LOG_ERROR("UserEventHandler::handle_login_req - Database init failed. exit!\n");
		return new bikeInfoResponseEv(ERRO_PROCCESS_FALED);
	}
	LOG_DEBUG("�����ݿ�ɹ���");

	UserService us(mysqlconn);
	//��ѯ�豸��Ϣ
	char sql_content[1024];
	//����Point����bike_point��Ϊ˫��������x����ֵ
	sprintf(sql_content, "SELECT devno, status, x(bike_point) x, y(bike_point) y FROM bikeinfo");
	SqlRecordSet record_set;
	if (!us.findBikeInfo(sql_content, record_set))
	{
		LOG_DEBUG("��ѯ������Ϣbikeinfoʧ�ܣ�������Ϣ��%s", mysqlconn->GetErrInfo());
		return new bikeInfoResponseEv(ERRO_PROCCESS_FALED);
	}
	bikeInfoResponseEv* bikeResEv = new bikeInfoResponseEv(ERRC_SUCCESS);
	MYSQL_RES* res = record_set.GetResult();
	MYSQL_ROW row;
	int columns = mysql_num_fields(res);//û���õ�
	//����һ��������ϵ���һ�С�����mysql_store_result()֮��ʹ��ʱ�����û�и�����пɼ���ʱ��
	//mysql_fetch_row()����NULL������mysql_use_result()֮��ʹ��ʱ����û�и�����пɼ���ʱ���������һ������mysql_fetch_row()����NULL��
	while (row = mysql_fetch_row(res))
	{
		//���ַ���ת���ɸ�����
		bikeResEv->set_bikeResponse_msg(atoi(row[0]), atoi(row[1]), strtod(row[2], NULL), strtod(row[3], NULL));
	}
	LOG_DEBUG("ͬ����Ϣ�����л���ɣ�");
	return bikeResEv;
}

//�˻���¼����
ListAccountRecordsResonseEv* UserEventHandler::handle_list_account_records_req(ListAccountRecordsReqEv* ev)
{
	std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);
	//_st_env_config conf_args = Iniconfig::getInstance()->getconfig();
	LOG_DEBUG("���ڴ����ݿ�.....");
	if (!openDataBase(mysqlconn))//mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),conf_args.db_pwd.c_str(), conf_args.db_name.c_str()))
	{
		LOG_ERROR("UserEventHandler::handle_login_req - Database init failed. exit!\n");
		return new ListAccountRecordsResonseEv(ERRO_PROCCESS_FALED, 0, 0, 0);
	}
	LOG_DEBUG("�����ݿ�ɹ���");
	/*
	UserService us(mysqlconn);
	bool result = false;

	char sql_content[1024] = "insert into use_record_info(mobile,start, end,time,money) \
		values(\"%s\", \"%s\", \"%s\", FROM_UNIXTIME(%d, '%Y-%m-%d %H:%i:%S'),\"%d\")";

	*/
	//us.userRecordModify(sql_content)
	//us.

	return nullptr;
}
