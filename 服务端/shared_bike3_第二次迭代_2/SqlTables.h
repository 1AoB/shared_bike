#ifndef BRKS_COMMON_DATASEVER_SQLTABLES_H
#define BRKS_COMMON_DATASEVER_SQLTABLES_H

#include <memory>
#include "sqlconnection.h"
#include "glo_def.h"

class SqlTables {
public:
	SqlTables(std::shared_ptr<MysqlConnection> sqlConn) :sqlconn_(sqlConn)
	{

	}
	bool CreateUserInfo()//用户信息表
	{
		const char* pUserInfoTable = "\
									CREATE TABLE IF EXISTS userinfo(\
									id            int(16)           NOT NULL primary key auto_increment,\
									mobile        varchar(16)       NOT NULL default '13000000000',\
									username      varchar(128)      NOT NULL default '',\
									verify        int(4)            NOT NULL default '0',\
									registertm    timestamp			NOT NULL default CURRENT_TIMESTAMP,\
									money		  int(4)			NOT NULL default 0,\
									INDEX         mobile_index(mobile) \
									)";
		if (!sqlconn_->Execute(pUserInfoTable))
		{
			LOG_ERROR("<SqlTable.h> : create table pUserInfoTable table failed. error msg : %s \n",sqlconn_->GetErrInfo());
			return false;
		}
		return true;
	}

	bool CreateBikeTable() {//单车表
		const char* pBikeInfoTable = "\
									CREATE TABLE IF NOT EXITS bikeinfo ( \
									id			int				NOT	NULL primary key suto_increment,\
									devo		int				NOT NULL default 0,\
									status		tinyint(1)		NOT NULL default 0,\
									trouble		int				NOT NULL default '',\
									tmsg		varchar(256)	NOT NULL default 0,\
									longitude	double(10,6)	NOT NULL default 0,\
									unique(devno)\
									";
		if (!sqlconn_->Execute(pBikeInfoTable))
		{
			LOG_ERROR("<SqlTable.h> : create table pBikeInfoTable table failed. error msg : %s \n", sqlconn_->GetErrInfo());
			return false;
		}
		return true;
	}

	~SqlTables()
	{

	}
private:
	std::shared_ptr<MysqlConnection> sqlconn_;
};



#endif
