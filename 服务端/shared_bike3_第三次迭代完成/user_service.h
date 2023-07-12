#ifndef __USER_SERVICE_H_
#define __USER_SERVICE_H_

#include <memory>
#include "sqlconnection.h"
/*
* UserService �����Ҫ�����������ݿ���н�����ʵ���û���Ϣ�����г���Ϣ����ɾ�Ĳ�Ȳ�����
* ���������ṩ�Ĺ��г�Ա���������Կ�����������µĹ��ܣ�
* 1.����û��Ƿ���ڡ�
* 2.������û���Ϣ��
* 3.��֤�û���¼��Ϣ��
* 4.��������г���Ϣ��
* 5.ɾ��ָ����ŵ����г���Ϣ��
* 6.�������г���Ϣ��
* 7.����û����м�¼
* 8.�����û����м�¼��
* 9.��������ָ�����������г���Ϣ��
* 10.��������ָ���������û���Ϣ��
*/
class UserService
{
public:
	UserService(std::shared_ptr<MysqlConnection> sql_conn);//sql_conn���ڽ�����MySQL���ݿ������
	bool exist(std::string& userName);//�����û��Ƿ����
	bool insert(std::string& userName, std::string& userPwd);//�����û�(�û���,����)

	/*���������*/
	bool verifyUserInfo(std::string& userName, std::string& userPwd);//verify��֤����˼,��֤�û���+�����Ƿ���ȷ
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




