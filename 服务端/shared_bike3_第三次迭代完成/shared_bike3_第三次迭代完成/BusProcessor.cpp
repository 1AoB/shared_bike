#include "BusProcessor.h"
#include "SqlTables.h"


BusinessProcessor::BusinessProcessor(std::shared_ptr<MysqlConnection> sqlconn)
	:mysqlconn_(sqlconn), ueh_(new UserEventHandler())
{

}

BusinessProcessor::~BusinessProcessor()
{
	//���ú󣬸�����ָ�벻�ٹ����κζ���ע�⣬������ͷŲ�����������ȥ�ͷţ�ֻ�����ü����� 1�����ͷ������ü���Ϊ 0 ʱ���н��еġ�
	ueh_.reset();
}

bool BusinessProcessor::init()
{
	SqlTables tables(mysqlconn_);
	LOG_DEBUG("���ݿ����ڳ�ʼ��......");
	/* (!mysqlconn_->Execute("create table Wo666(name char(6), age int)"))
	{
		LOG_ERROR("create table Wo666(name char(6), age int) failed!\n");
		return false;
	}*/
	if (!tables.CreateBikeTable())
	{
		LOG_ERROR("<BusProcessor.cpp> create shared_bike table failed!\n");
		printf("<BusProcessor.cpp> create shared_bike table failed!\n");
		return false;
	}
	if (!tables.CreateUserInfo())
	{
		LOG_ERROR("<BusProcessor.cpp> create user table failed!\n");
		printf("<BusProcessor.cpp> create user table failed!\n");
		return false;
	}
	if (!tables.UserRideRecordInfo())
	{
		LOG_ERROR("<BusProcessor.cpp> create UserRideRecordInfo table failed!\n");
		printf("<BusProcessor.cpp> create UserRideRecordInfo table failed!\n");
		return false;
	}
	LOG_DEBUG("���ݿ��ʼ�����......\n");
	return true;
}




