#ifndef BRKS_BUS_MAIN_H_
#define BRKS_BUS_MAIN_H_

#include "user_event_handler.h"
#include "sqlconnection.h"

class BusinessProcessor {
public:
	BusinessProcessor(std::shared_ptr<MysqlConnection>conn);
	bool init();
	virtual ~BusinessProcessor();//ʲôЩ���̳���?
private:
	std::shared_ptr<MysqlConnection>mysqlconn_;//���ӵĹ���ָ��
	std::shared_ptr<UserEventHandler>ueh_;//�˺Ŵ������Ĺ���ָ��
};

#endif
