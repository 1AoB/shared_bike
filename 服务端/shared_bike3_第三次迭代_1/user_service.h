#ifndef BRKS_SVR_USER_SERVICE_H_
#define BRKS_SVR_USER_SERVICE_H_
#include"glo_def.h"
#include"sqlconnection.h"
#include"events_def.h"

#include<memory>


class UserService {
public:
	UserService(std::shared_ptr<MysqlConnection> sql_conn);
	bool exist(const std::string& mobile);//���ݿ����Ƿ���ھ��и����ֻ�������û�
	bool insert(const std::string& mobile);//���ֻ���������û��������ݿ�

	~UserService();

private:
	std::shared_ptr<MysqlConnection> sql_conn_;//�˳�Ա�������ڽ�����MySQL���ݿ�����Ӳ�ִ��SQL��ѯ

};


#endif
