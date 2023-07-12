#ifndef BRKS_BUS_MAIN_H_
#define BRKS_BUS_MAIN_H_

#include "user_event_handler.h"
#include "sqlconnection.h"

class BusinessProcessor {
public:
	BusinessProcessor(std::shared_ptr<MysqlConnection>conn);
	bool init();
	virtual ~BusinessProcessor();//什么些类会继承他?
private:
	std::shared_ptr<MysqlConnection>mysqlconn_;//连接的共享指针
	std::shared_ptr<UserEventHandler>ueh_;//账号处理器的共享指针
};

#endif
