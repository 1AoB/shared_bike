#include "BusProcessor.h"
#include "SqlTables.h"


BusinessProcessor::BusinessProcessor(std::shared_ptr<MysqlConnection> sqlconn)
	:mysqlconn_(sqlconn), ueh_(new UserEventHandler())
{

}

BusinessProcessor::~BusinessProcessor()
{
	//调用后，该智能指针不再管理任何对象。注意，这里的释放并不是真正地去释放，只是引用计数减 1，而释放是引用计数为 0 时自行进行的。
	ueh_.reset();
}

bool BusinessProcessor::init()
{
	SqlTables tables(mysqlconn_);
	LOG_DEBUG("数据库正在初始化......");
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
	LOG_DEBUG("数据库初始化完成......\n");
	return true;
}




