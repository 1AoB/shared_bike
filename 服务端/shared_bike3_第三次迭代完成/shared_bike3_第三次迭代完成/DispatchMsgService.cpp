#include "DispatchMsgService.h"
#include <algorithm>
#include "bike.pb.h"
#include "events_def.h"



DispatchMsgService* DispatchMsgService::DMS_ = nullptr;
std::queue<iEvent*> DispatchMsgService::response_events;
pthread_mutex_t DispatchMsgService::queue_mutext;

/*��������*/
int DispatchMsgService::clientNumber = 0;//�Ѿ����ӵĿͻ�������
std::forward_list<struct bufferevent*> DispatchMsgService::m_userList;//�������ӵ��Ŀͻ���
NetworkInterface* DispatchMsgService::NTIF_ = nullptr;

DispatchMsgService::DispatchMsgService() :tp(nullptr)
{


}

DispatchMsgService::~DispatchMsgService()
{

}

BOOL DispatchMsgService::open()
{
	svr_exit_ = false;

	thread_mutex_create(&queue_mutext);
	LOG_DEBUG("�̳߳س�ʼ��......");
	//printf("�̳߳س�ʼ��......\n");
	tp = thread_pool_init();
	LOG_DEBUG("��ǰ�̳߳��е��߳���Ϊ��tp->threads=%d", tp->threads);
	LOG_DEBUG("�̳߳��ܴ�������������Ϊ��tp->max_queue=%d", tp->max_queue);
	LOG_DEBUG("�̳߳س�ʼ���ɹ���");
	//printf("�̳߳س�ʼ���ɹ���\n");
	return tp ? TRUE : FALSE;
}

void DispatchMsgService::close()
{
	svr_exit_ = true;

	thread_pool_destroy(tp);
	thread_mutex_destroy(&queue_mutext);
	subscribers_.clear();

	tp = NULL;
}

void DispatchMsgService::subscribe(u32 eid, iEventHandler* handler)
{
	//LOG_DEBUG("DispatchMsgService::subscribe eid:%d\n", eid);
	T_EventHandlersMap::iterator iter = subscribers_.find(eid);
	/*map<string, int> m_stlmap;
	m_stlmap[��xiaomi��] = 88;]

	auto mpit = m_stlmap.begin();
	first��õ�Map��key����Чֵ��
	second��õ�Map��value����Чֵ��

	����
	mpit ->first; // �õ��� string ֵ�� ��xiaomi��
	mpit ->second; //�õ��� int ֵ�� 88*/
	/*find �㷨�᷵��һ��ָ���ҵ�����ĵ����������û���ҵ����󣬻᷵��������еĽ���������*/
	if (iter != subscribers_.end())
	{

		T_EventHandlers::iterator hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler);
		if (hdl_iter == iter->second.end())
		{
			iter->second.push_back(handler);

		}
	}
	else
	{
		subscribers_[eid].push_back(handler);
	}
}

void DispatchMsgService::unsubscribe(u32 eid, iEventHandler* handler)
{
	T_EventHandlersMap::iterator iter = subscribers_.find(eid);
	if (iter != subscribers_.end())
	{
		T_EventHandlers::iterator hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler);
		if (hdl_iter != iter->second.end())
		{
			iter->second.erase(hdl_iter);//�൱�ڲ�����ֵ//
		}
	}
}

i32 DispatchMsgService::enqueue(iEvent* ev)
{
	if (NULL == ev)
	{
		return -1;
	}
	ConnectSession* cs = (ConnectSession*)ev->get_args();
	LOG_DEBUG("���ͻ���[%s][%p]����[%d]Ͷ�ݵ����������.......", cs->remote_ip, cs->bev, cs->eid);
	thread_task_t* task = thread_task_alloc(0);
	task->handler = DispatchMsgService::svc;
	task->ctx = ev;

	return thread_task_post(tp, task);
}

void DispatchMsgService::svc(void* argv)
{
	DispatchMsgService* dms = DispatchMsgService::getInstance();

	iEvent* ev = (iEvent*)argv;
	if (!dms->svr_exit_)
	{
		LOG_DEBUG("DispatchMsgService::svc....\n");
		//printf("DispatchMsgService::svc....\n");
		//printf("!ev->eid=%d\n", ev->get_eid());

		//�����û��������ɶ�Ӧ����Ӧ�¼�
		iEvent* rsp = dms->process(ev);
		if (rsp)
		{
			rsp->dump(std::cout);
			rsp->set_args(ev->get_args());

		}
		else
		{
			//������ֹ��Ӧ�¼�
			rsp = new ExitRspEv();
			rsp->set_args(ev->get_args());
		}
		//
		ConnectSession* cs = (ConnectSession*)rsp->get_args();

		LOG_DEBUG("���ڽ��¼�[%s][%p]������Ӧ�¼����е���,eid=%d.......", cs->remote_ip, cs->bev, cs->eid);
		thread_mutex_lock(&queue_mutext);
		response_events.push(rsp); //����ʲô�¼�����ͳһ������Ӧ�¼����е���
		thread_mutex_unlock(&queue_mutext);
	}
}

//�ɷ������¼�
iEvent* DispatchMsgService::process(const iEvent* ev)
{
	LOG_DEBUG("DispatchMsgService::process - ev: %p\n", ev);

	if (ev == NULL)
	{
		return nullptr;
	}
	u32 eid = ev->get_eid();
	LOG_DEBUG("DispatchMsgService::process-eid: %u\n", eid);
	//printf("DispatchMsgService::process-eid: %u\n", eid);
	if (eid == EEVNETID_UNKOWN)
	{
		LOG_WARN("DispatchMsgService: unknow event id %d\n", eid);
		printf("DispatchMsgService: unknow event id %d\n", eid);
		return nullptr;
	}
	T_EventHandlersMap::iterator handlers = subscribers_.find(eid);
	if (handlers == subscribers_.end())
	{
		LOG_WARN("DispatchMsgService��no any event handler subscribed: %d\n", eid);
		printf("DispatchMsgService��no any event handler subscribed: %d\n", eid);
		return nullptr;
	}
	iEvent* rsp = NULL;
	for (auto iter = handlers->second.begin(); iter != handlers->second.end(); iter++)
	{
		iEventHandler* handler = *iter;
		LOG_DEBUG("DispatchMsgService��get handler: %s\n", handler->get_name().c_str());

		rsp = handler->handle(ev);//����ʹ��vector��list���ض�� rsp��Ŀǰ�����󻹲���Ҫ����Ϊÿ���û�����ֻ��һ��Ҫ������¼�		
	}
	//����
	return rsp;
}

DispatchMsgService* DispatchMsgService::getInstance()
{
	if (DMS_ == nullptr)
	{
		DMS_ = new DispatchMsgService();

	}
	return DMS_;
}

void DispatchMsgService::addClientNumber()
{
	//clientNumber++;
	LOG_DEBUG("current client number:%d\n", ++clientNumber);
}

void DispatchMsgService::addUserToList(struct bufferevent* user)
{
	m_userList.push_front(user);
	LOG_DEBUG("current client number:%d\n", ++clientNumber);
}

void DispatchMsgService::decUserFromList(struct bufferevent* user)
{
	m_userList.remove(user);
	LOG_DEBUG("current client number:%d\n", --clientNumber);

}

std::forward_list<struct bufferevent*> DispatchMsgService::getUserList()
{
	return m_userList;
}

iEvent* DispatchMsgService::parseEvent(const char* message, u32 len, u32 eid)
{
	if (!message)
	{
		LOG_ERROR("DispatchMsgService::parseEvent - message is null[eid=%d]\n", eid);
		return nullptr;
	}
	switch (eid)//�ж����eid���ĸ��¼�
	{
		//��ȡ�ֻ���֤��
	case EEVENTID_GET_MOBILE_CODE_REQ:
	{
		tutorial::mobile_request mr;
		//mr.set_mobile(message);
		LOG_DEBUG("�ͻ������ڳ��Ի�ȡ�ֻ���֤��......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		if (mr.ParseFromArray(message, len))//��������
		{
			LOG_DEBUG("�����������.�ͻ����û���Ϊ��%s", mr.username().c_str());
			MobileCodeReqEv* ev = new MobileCodeReqEv(mr.username());
			return ev;
		}
		break;
	}
	//ע������
	case EEVENTID_REGISTER_USER_REQ:
	{
		LOG_DEBUG("�ͻ�����������ע��......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::registerUser_request rr;
		//�������������
		//lr.set_icode(eid);
		if (rr.ParseFromArray(message, len))
		{
			LOG_DEBUG("�����������.�ͻ���ע����û���Ϊ��%s�� ����Ϊ��%s����֤��Ϊ��%d", rr.username().c_str(), rr.userpwd().c_str(), rr.vercode());
			RegisterReqEv* ev = new RegisterReqEv(rr.username(), rr.userpwd(), rr.vercode());
			return ev;
		}
		break;
	}
	//��ӵ�������
	case EEVENTID_ADDBIKE_REQ:
	{
		LOG_DEBUG("�ͻ������ڳ�����ӵ���......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::addBike_request ar;
		//�������������
		//lr.set_icode(eid);
		if (ar.ParseFromArray(message, len))
		{
			LOG_DEBUG("�����������.�ͻ���������ӵĵ���IdΪ��%d�� ���ȣ�%lf, γ�ȣ�%lf", ar.bikeid(), ar.longitude(), ar.latitude());
			AddBikeReqEv* ev = new AddBikeReqEv(ar.bikeid(), ar.longitude(), ar.latitude());
			return ev;
		}
		break;
	}
	//ɾ����������
	case EEVENTID_DELETEBIKE_REQ:
	{
		LOG_DEBUG("�ͻ������ڳ����Ƴ�����......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::deleteBike_request dr;
		//�������������
		//lr.set_icode(eid);
		if (dr.ParseFromArray(message, len))
		{
			LOG_DEBUG("�����������.�ͻ��������Ƴ��ĵ���IdΪ��%d", dr.bikeid());
			DeleteBikeReqEv* ev = new DeleteBikeReqEv(dr.bikeid());
			return ev;
		}
		break;
	}
	//��½
	case EEVENTID_LOGIN_REQ:
	{
		LOG_DEBUG("�ͻ������ڳ��Ե�½......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::login_request lr;
		//�������������
		//lr.set_icode(eid);
		if (lr.ParseFromArray(message, len))
		{
			LOG_DEBUG("�����������.�ͻ����û���Ϊ��%s�� ����Ϊ��%s", lr.username().c_str(), lr.userpwd().c_str());
			LoginReqEv* ev = new LoginReqEv(lr.username(), lr.userpwd());
			return ev;
		}
		break;
	}
	//��ȡɨ����������
	case EEVENTID_SCANQRSTART_REQ:
	{
		LOG_DEBUG("�ͻ�����������ɨ������......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::bike_scanQR_start_request bssr;
		if (bssr.ParseFromArray(message, len))
		{
			LOG_DEBUG("�����������.�ͻ����ֻ���Ϊ��%s��ʱ���Ϊ��%ld����γ:%lf�� ����:%lf��)",
				bssr.username().c_str(), bssr.timestamp(), bssr.longitude(), bssr.latitude());

			bikeScanQRStartReqEv* ev = new bikeScanQRStartReqEv(bssr.username(), bssr.bikeid(), bssr.timestamp(),
				bssr.longitude(), bssr.latitude());
			return ev;
		}
		break;
	}
	//��ȡɨ�뻹������
	case EEVENTID_SCANQREND_REQ:
	{
		LOG_DEBUG("�ͻ�����������ɨ�뻹��......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::bike_scanQR_end_request bser;
		if (bser.ParseFromArray(message, len))
		{
			LOG_DEBUG("�����������.�ͻ����ֻ���Ϊ��%s��ʱ���Ϊ��%d������:(��γ:%lf��,����:%lf��))",
				bser.username().c_str(), bser.timestamp(), bser.longitude(), bser.latitude());
			bikeScanQREndReqEv* ev = new bikeScanQREndReqEv(bser.username(), bser.bikeid(), bser.timestamp(),
				bser.longitude(), bser.latitude());
			return ev;
		}
		break;
	}
	//��ȡɨ����ϱ�������
	case EEVENTID_FAULT_REQ:
	{
		LOG_DEBUG("�ͻ�����������ɨ����ϱ���......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::bike_fault_request bfr;
		if (bfr.ParseFromArray(message, len))
		{
			LOG_DEBUG("�����������.���޵���idΪ��%d��ʱ���Ϊ��%d������:(��γ:%lf��,����:%lf��))",
				bfr.bike_id(), bfr.timestamp(), bfr.longitude(), bfr.latitude());
			bikeScanQRFaultReqEv* ev = new bikeScanQRFaultReqEv(bfr.username().c_str(),
				bfr.bike_id(), bfr.timestamp(), bfr.longitude(), bfr.latitude());
			return ev;
		}
		break;
	}
	//��ȡɨ�뱨���������
	case EEVENTID_REPAIRFINISH_REQ:
	{
		LOG_DEBUG("�ͻ�����������ɨ�뱨�����......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::bike_repair_finish_request brfr;
		if (brfr.ParseFromArray(message, len))
		{
			LOG_DEBUG("�����������.������ɵ���idΪ��%d��ʱ���Ϊ��%d������:(��γ:%lf��,����:%lf��))",
				brfr.bike_id(), brfr.timestamp(), brfr.longitude(), brfr.latitude());
			bikeScanQRRepairFinishReqEv* ev = new bikeScanQRRepairFinishReqEv(brfr.username().c_str(),
				brfr.bike_id(), brfr.timestamp(), brfr.longitude(), brfr.latitude());
			return ev;
		}
		break;
	}
	//�㲥����
	case EEVENTID_BROADCAST_MSG_REQ:
	{
		LOG_DEBUG("���ڽ�������Ϣ�㲥�������ͻ���......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::broadcast_msg_request bmr;
		if (bmr.ParseFromArray(message, len))
		{

			LOG_DEBUG("�����������.�㲥��Ϣ: ����Id:%d, ����״̬��%d����γ��(%lf,%lf))", bmr.bikeid(), bmr.bikestatus(), bmr.longitude(), bmr.latitude());
			broadcastMsgReqEv* ev = new broadcastMsgReqEv(bmr.bikeid(), bmr.bikestatus(), bmr.longitude(), bmr.latitude());
			return ev;
		}
		break;
	}
	case EEVENTID_LIST_BIKEINFO_REQ:
	{
		tutorial::list_all_bikeinfo_request labr;
		if (labr.ParseFromArray(message, len))
		{
			LOG_DEBUG("�û�[%s]�ѵ�½������������Ϣͬ��......", labr.username().c_str());
			bikeInfoReqEv* ev = new bikeInfoReqEv(labr.username());
			return ev;
		}
		break;
	}
	//��ȡ�˻���Ϣ����
	case EEVENTID_LIST_ACCOUNT_RECORDS_REQ:
	{
		LOG_DEBUG("�ͻ��˽������У����ڻ�ȡ�˻���Ϣ......");
		LOG_DEBUG("���ڴӿͻ������л���massage�н�������.......");
		tutorial::list_account_records_request recordRequest;
		if (recordRequest.ParseFromArray(message, len))
		{
			ListAccountRecordsReqEv* ev = new ListAccountRecordsReqEv(recordRequest.username());
			return ev;
		}
		break;
	}


	default:
		break;
	}

	return nullptr;
}

void DispatchMsgService::handleAllResponseEvent(NetworkInterface* interface)
{
	bool done = false;
	while (!done)
	{
		iEvent* ev = nullptr;
		thread_mutex_lock(&queue_mutext);
		if (!response_events.empty())
		{
			printf("!response_events.empty()\n");
			ev = response_events.front();//�ó���Ӧ�ĵ�һ���¼�
			response_events.pop();
		}
		else
		{
			done = true;
		}
		thread_mutex_unlock(&queue_mutext);

		if (!done)
		{
			u32 EventId = ev->get_eid();
			LOG_DEBUG("������Ӧ�¼�--eid=%d", EventId);
			switch (EventId)
			{
				//��ȡ�ֻ���֤����Ӧ
			case EEVENTID_GET_MOBILE_CODE_RSP:
			{
				LOG_DEBUG("������Ӧ�û���ȡ�ֻ���֤������--id:EEVENTID_GET_MOBILE_CODE_RSP\n");
				//������Ӧ��Ϣ
				sendPesponseMessage(ev, EEVENTID_GET_MOBILE_CODE_RSP, interface);
				break;
			}
			//�û�ע����Ӧ
			case EEVENTID_REGISTER_USER_RSP:
			{
				LOG_DEBUG("������Ӧ�û�ע������--id:EEVENTID_REGISTER_USER_RSP\n");
				//������Ӧ��Ϣ
				sendPesponseMessage(ev, EEVENTID_REGISTER_USER_RSP, interface);
				break;
			}
			//��½��Ӧ
			case EEVENTID_LOGIN_RSP:
			{
				LOG_DEBUG("������Ӧ�û��ֻ���½����--id:EEVENTID_LOGIN_RSP\n");
				ConnectSession* cs = (ConnectSession*)ev->get_args();
				//DispatchMsgService::addUserToList(cs->bev);
				sendPesponseMessage(ev, EEVENTID_LOGIN_RSP, interface);
				break;
			}
			//ɨ��������Ӧ
			case EEVENTID_SCANQRSTART_PSP:
			{
				LOG_DEBUG("������Ӧ�û�ɨ����������--id:EEVENTID_SCANQRSTART_PSP\n");
				sendPesponseMessage(ev, EEVENTID_SCANQRSTART_PSP, interface);
				break;
			}
			//ɨ�뻹����Ӧ
			case EEVENTID_SCANQREND_PSP:
			{
				LOG_DEBUG("������Ӧ�û�ɨ�뻹������--id:EEVENTID_SCANQREND_PSP\n");
				sendPesponseMessage(ev, EEVENTID_SCANQREND_PSP, interface);
				break;
			}
			//ɨ����ϱ�����Ӧ
			case EEVENTID_FAULT_PSP:
			{
				LOG_DEBUG("������Ӧ�û�ɨ����ϱ�������--id:EEVENTID_FAULT_PSP\n");
				sendPesponseMessage(ev, EEVENTID_FAULT_PSP, interface);
				break;
			}
			//ɨ�뱨�������Ӧ
			case EEVENTID_REPAIRFINISH_PSP:
			{
				LOG_DEBUG("������Ӧ�û�ɨ����ϱ�������--id:EEVENTID_FAULT_PSP\n");
				sendPesponseMessage(ev, EEVENTID_REPAIRFINISH_PSP, interface);
				break;
			}
			//�㲥��Ӧ
			case EEVENTID_BROADCAST_MSG_RSP:
			{
				LOG_DEBUG("���ڹ㲥�����ͻ���--id:EEVENTID_BROADCAST_MSG_RSP\n");
				sendPesponseMessage(ev, EEVENTID_BROADCAST_MSG_RSP, interface);
				break;
			}
			//������Ϣͬ����Ӧ
			case EEVENTID_LIST_BIKEINFO_RSP:
			{
				LOG_DEBUG("���ڹ㲥�����ͻ���--id:EEVENTID_BROADCAST_MSG_RSP\n");
				sendPesponseMessage(ev, EEVENTID_LIST_BIKEINFO_RSP, interface);
				break;
			}
			//�˻���¼������Ӧ
			case EEVENTID_ACCOUNT_RECORDS_RSP:
			{
				LOG_DEBUG("������Ӧ�û���ȡ�˻���¼����--id:EEVENTID_ACCOUNT_RECORDS_RSP\n");
				sendPesponseMessage(ev, EEVENTID_ACCOUNT_RECORDS_RSP, interface);
				break;
			}
			//��ӵ�����Ӧ
			case EEVENTID_ADDBIKE_RSP:
			{
				LOG_DEBUG("������Ӧ�û���ӵ�������--id:EEVENTID_ADDBIKE_RSP\n");
				//������Ӧ��Ϣ
				sendPesponseMessage(ev, EEVENTID_ADDBIKE_RSP, interface);
				break;
			}
			//ɾ��������Ӧ
			case EEVENTID_DELETEBIKE_RSP:
			{
				LOG_DEBUG("������Ӧ�û�ɾ����������--id:EEVENTID_DELETEBIKE_RSP\n");
				//������Ӧ��Ϣ
				sendPesponseMessage(ev, EEVENTID_DELETEBIKE_RSP, interface);
				break;
			}
			//�˳���Ӧ
			case EEVNETID_EXIT_RSP:
			{
				LOG_DEBUG("�˳���Ӧ--ev->get_eid() == EEVNETID_EXIT_RSP\n");
				ConnectSession* cs = (ConnectSession*)ev->get_args();
				cs->response = ev;
				interface->send_response_message(cs);
				break;
			}

			default:
				LOG_DEBUG("�յ�δ֪����Ӧ��EventId=%d\n", EventId);
				break;
			}
		}
	}
}

void DispatchMsgService::sendPesponseMessage(iEvent* ev, EventID Eid, NetworkInterface* interface)
{
	ConnectSession* cs = (ConnectSession*)ev->get_args();
	cs->response = ev;
	//ϵ�л���������
	cs->message_len = ev->Bytesize();
	cs->write_buf = new char[cs->message_len + MESSAGE_HEADER_LEN];
	cs->eid = Eid;

	//��װͷ��
	memcpy(cs->write_buf, MESSAGE_HEADER_ID, 4);
	*(u16*)(cs->write_buf + 4) = Eid;
	*(i32*)(cs->write_buf + 6) = cs->message_len;

	LOG_DEBUG("�������л�����......");//������ת���ֽ���
	ev->SerializeToArray(cs->write_buf + MESSAGE_HEADER_LEN, cs->message_len);

	//printf("!!!!!!!!!!!!sendPesponseMessage--cs->write_buf:%s\n", cs->write_buf);

	interface->send_response_message(cs);
}


