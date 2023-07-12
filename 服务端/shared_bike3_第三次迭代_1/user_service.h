#ifndef BRKS_SVR_USER_SERVICE_H_
#define BRKS_SVR_USER_SERVICE_H_
#include"glo_def.h"
#include"sqlconnection.h"
#include"events_def.h"

#include<memory>


class UserService {
public:
	UserService(std::shared_ptr<MysqlConnection> sql_conn);
	bool exist(const std::string& mobile);//数据库中是否存在具有给定手机号码的用户
	bool insert(const std::string& mobile);//给手机号码的新用户插入数据库

	~UserService();

private:
	std::shared_ptr<MysqlConnection> sql_conn_;//此成员变量用于建立到MySQL数据库的连接并执行SQL查询

};


#endif
