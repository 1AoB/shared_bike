#ifndef BRKS_BUS_USERM_HANDLER_H_
#define BRKS_BUS_USERM_HANDLER_H_

#include "glo_def.h"
#include "iEventHandler.h"
#include "events_def.h"
#include "threadpool/thread.h"

#include <map>

class MysqlConnection;//��<sqlconnection.h>��

//�û���������
/*
* �������֮����ȥ���캯���а����Ƕ���
*/
class UserEventHandler : public iEventHandler
{
public:
	UserEventHandler();
	virtual ~UserEventHandler();
	virtual iEvent* handle(const iEvent* ev);

private:
	//���ڷ�װ�ĺ���,����������
	bool openDataBase(std::shared_ptr<MysqlConnection>& mysqlconn);

	i32 code_gen();//ģ���������֤��ķ���
	MobileCodeRspEv* handle_mobile_code_req(MobileCodeReqEv* ev);//���������֤��
	LoginResEv* handle_login_req(LoginReqEv* ev);//�û���¼����

	RegisterResEv* handle_user_register_req(RegisterReqEv* ev);//�û�ע��
	
	AddBikeResEv* handle_addBike_req(AddBikeReqEv* ev);//��ӵ���
	DeleteBikeResEv* handle_deleteBike_req(DeleteBikeReqEv* ev);//ɾ������

	bikeScanQRStartResponseEv* handle_bike_scanQRSatrt_req(bikeScanQRStartReqEv* ev);//����ɨ�뿪ʼ����
	bikeScanQREndResponseEv* handle_bike_scanQREnd_req(bikeScanQREndReqEv* ev);//����ɨ���������
	bikeScanQRFaultResponseEv* handle_bike_scanQRFault_req(bikeScanQRFaultReqEv* ev);//�������ϱ�������
	bikeScanQRRepairFinishResponseEv* handle_bike_scanQRRepairFinish_req(bikeScanQRRepairFinishReqEv* ev);//����ɨ���޸��������
	broadcastMsgResponseEv* handle_broadcast_msg_req(broadcastMsgReqEv* ev);//�㲥�����¼�
	bikeInfoResponseEv* handle_bikeInfo_req(bikeInfoReqEv* ev);//������Ϣͬ�������¼�

	ListAccountRecordsResonseEv* handle_list_account_records_req(ListAccountRecordsReqEv* ev);//�˻���¼����

private:
	std::map<std::string, i32> m2c_;  //first is mobile, second is code
	pthread_mutex_t pm_;
};

#endif // !BRKS_BUS_USERM_HANDLER_H_

