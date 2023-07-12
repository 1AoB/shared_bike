#ifndef BRKS_BUS_USERM_HANDLER_H_
#define BRKS_BUS_USERM_HANDLER_H_

#include "glo_def.h"
#include "iEventHandler.h"
#include "events_def.h"
#include "threadpool/thread.h"

#include <map>

class MysqlConnection;//在<sqlconnection.h>中

//用户处理器类
/*
* 创建完成之后先去构造函数中把他们订阅
*/
class UserEventHandler : public iEventHandler
{
public:
	UserEventHandler();
	virtual ~UserEventHandler();
	virtual iEvent* handle(const iEvent* ev);

private:
	//用于封装的函数,方便后序调用
	bool openDataBase(std::shared_ptr<MysqlConnection>& mysqlconn);

	i32 code_gen();//模拟产生的验证码的方法
	MobileCodeRspEv* handle_mobile_code_req(MobileCodeReqEv* ev);//生成随机验证码
	LoginResEv* handle_login_req(LoginReqEv* ev);//用户登录请求

	RegisterResEv* handle_user_register_req(RegisterReqEv* ev);//用户注册
	
	AddBikeResEv* handle_addBike_req(AddBikeReqEv* ev);//添加单车
	DeleteBikeResEv* handle_deleteBike_req(DeleteBikeReqEv* ev);//删除单车

	bikeScanQRStartResponseEv* handle_bike_scanQRSatrt_req(bikeScanQRStartReqEv* ev);//单车扫码开始骑行
	bikeScanQREndResponseEv* handle_bike_scanQREnd_req(bikeScanQREndReqEv* ev);//单车扫码结束骑行
	bikeScanQRFaultResponseEv* handle_bike_scanQRFault_req(bikeScanQRFaultReqEv* ev);//单车故障报修请求
	bikeScanQRRepairFinishResponseEv* handle_bike_scanQRRepairFinish_req(bikeScanQRRepairFinishReqEv* ev);//单车扫码修复完成请求
	broadcastMsgResponseEv* handle_broadcast_msg_req(broadcastMsgReqEv* ev);//广播请求事件
	bikeInfoResponseEv* handle_bikeInfo_req(bikeInfoReqEv* ev);//单车信息同步请求事件

	ListAccountRecordsResonseEv* handle_list_account_records_req(ListAccountRecordsReqEv* ev);//账户记录请求

private:
	std::map<std::string, i32> m2c_;  //first is mobile, second is code
	pthread_mutex_t pm_;
};

#endif // !BRKS_BUS_USERM_HANDLER_H_

