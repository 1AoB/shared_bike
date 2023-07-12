#ifndef __USER_SERVICE_H_
#define __USER_SERVICE_H_

#include <memory>
#include "sqlconnection.h"
/*
* UserService 类的主要功能是与数据库进行交互，实现用户信息和自行车信息的增删改查等操作。
* 根据类中提供的公有成员函数，可以看出其具有如下的功能：
* 1.检查用户是否存在。
* 2.添加新用户信息。
* 3.验证用户登录信息。
* 4.添加新自行车信息。
* 5.删除指定编号的自行车信息。
* 6.更新自行车信息。
* 7.添加用户骑行记录
* 8.更新用户骑行记录。
* 9.查找满足指定条件的自行车信息。
* 10.查找满足指定条件的用户信息。
*/
class UserService
{
public:
	UserService(std::shared_ptr<MysqlConnection> sql_conn);//sql_conn用于建立到MySQL数据库的连接
	bool exist(std::string& userName);//查找用户是否存在
	bool insert(std::string& userName, std::string& userPwd);//插入用户(用户名,密码)

	/*补充的内容*/
	bool verifyUserInfo(std::string& userName, std::string& userPwd);//verify验证的意思,验证用户名+密码是否正确
	bool addBike(i32 bikeid, r64 longitude, r64 latitude);
	bool deleteBike(i32 bikeid);
	bool updataBikeinfo(char* sql_content);
	bool insertUseRideRecord(char* sql_content);
	bool userRecordModify(char* sql_content);
	bool findBikeInfo(char* sql_content, SqlRecordSet& record_set);
	bool findUserInfo(char* sql_content, SqlRecordSet& record_set);
private:
	std::shared_ptr<MysqlConnection> sql_conn_;

};

#endif // !__USER_SERVICE_H_




