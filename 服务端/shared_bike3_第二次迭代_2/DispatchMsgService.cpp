#include "DispatchMsgService.h"
#include "NetworkInterface.h"
#include <algorithm>

#include "bike.pb.h"
#include "events_def.h"

DispatchMsgService* DispatchMsgService::DMS_ = nullptr;
pthread_mutex_t DispatchMsgService::queue_mutex;
std::queue<iEvent*> DispatchMsgService::response_events;


DispatchMsgService::DispatchMsgService() :tp(nullptr)
{
    //tp = NULL;
    //NTIF_ = NULL;

}

DispatchMsgService::~DispatchMsgService()
{
}

BOOL DispatchMsgService::open()
{
    svr_exit_ = FALSE;

    thread_mutex_create(&queue_mutex);
    tp = thread_pool_init();

    return tp ? TRUE : FALSE;
}

void DispatchMsgService::close()
{
    svr_exit_ = TRUE;

    thread_pool_destroy(tp);
    thread_mutex_destroy(&queue_mutex);
    subscribers_.clear();

    tp = NULL;
}

DispatchMsgService* DispatchMsgService::getInstance() {
    if (DMS_ == nullptr) {
        DMS_ = new DispatchMsgService();
    }

    return DMS_;
}

void DispatchMsgService::svc(void* argv)
{
    DispatchMsgService* dms = DispatchMsgService::getInstance();
    iEvent* ev = (iEvent*)argv;
    if (!dms->svr_exit_)
    {
        LOG_DEBUG("DispatchMsgService::svc ...\n");
        iEvent* rsp = dms->process(ev);
        if (rsp)
        {
            rsp->dump(std::cout);
            rsp->set_args(ev->get_args());
        }
        else
        {
            //生成终止响应事件
            rsp = new ExitRspEv();
            rsp->set_args(ev->get_args());
        }

        thread_mutex_lock(&queue_mutex);
        response_events.push(rsp);
        thread_mutex_unlock(&queue_mutex);

    }
}

i32 DispatchMsgService::enqueue(iEvent* ev)
{
    if (NULL == ev)
    {
        return -1;
    }

    thread_task_t* task = thread_task_alloc(0);

    task->handler = DispatchMsgService::svc;
    task->ctx = ev;

    return thread_task_post(tp, task);

    //return msg_queue_.enqueue(ev, 0);
}

void DispatchMsgService::subscribe(u32 eid, iEventHandler* handler)
{
    LOG_DEBUG("DispatchMsgService::subscribe eid: %u\n", eid);
    T_EventHandlersMap::iterator iter = subscribers_.find(eid);
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
            iter->second.erase(hdl_iter);
        }
    }
}

iEvent* DispatchMsgService::process(const iEvent* ev)
{
    LOG_DEBUG("DispatchMsgService::process -ev: %p\n", ev);
    if (NULL == ev)
    {
        return NULL;
    }

    u32 eid = ev->get_eid();

    LOG_DEBUG("DispatchMsgService::process - eid: %u\n", eid);

    if (eid == EEVENTID_UNKOWN)
    {
        LOG_WARN("DispatchMsgService : unknow evend id %d", eid);
        return NULL;
    }

    //LOG_DEBUG("dispatch ev : %d\n", ev->get_eid());

    T_EventHandlersMap::iterator handlers = subscribers_.find(eid);
    if (handlers == subscribers_.end())
    {
        LOG_WARN("DispatchMsgService : no any event handler subscribed %d", eid);
        return NULL;
    }

    iEvent* rsp = NULL;
    for (auto iter = handlers->second.begin(); iter != handlers->second.end(); iter++)
    {
        iEventHandler* handler = *iter;
        LOG_DEBUG("DispatchMsgService : get handler: %s\n", handler->get_name().c_str());

        rsp = handler->handle(ev);//推荐使用vector 或list 返回多个rsp 
    }

    return rsp;
}

iEvent* DispatchMsgService::parseEvent(const char* message, u32 len, u32 eid)
{
    if (!message)
    {
        LOG_ERROR("DispatchMsgService::parseEvent - message is null[eid: %d].\n", eid);
        return nullptr;
    }

    if (eid == EEVENTID_GET_MOBLIE_CODE_REQ)
    {
        tutorial::mobile_request  mr;
        if (mr.ParseFromArray(message, len))
        {
            MobileCodeReqEv* ev = new MobileCodeReqEv(mr.mobile());
            return ev;
        }
    }
    else if (eid == EEVENTID_LOGIN_REQ)
    {
        tutorial::login_request lr;
        if (lr.ParseFromArray(message, len))
        {
            LoginReqEv* ev = new LoginReqEv(lr.mobile(),lr.icode());
            return ev;
        }
        //.........未完待续..............
    }

    return nullptr;
}


void DispatchMsgService::handleAllResponseEvent(NetworkInterface* interface)
{
    bool done = false;

    while (!done)
    {
        iEvent* ev = nullptr;
        thread_mutex_lock(&queue_mutex);
        if (!response_events.empty())
        {
            ev = response_events.front();
            response_events.pop();
        }
        else
        {
            done = true;
        }
        thread_mutex_unlock(&queue_mutex);//勘误 lock 应为 unlock

        if (!done)
        {
            if (ev->get_eid() == EEVENTID_GET_MOBLIE_CODE_RSP)
            {
                MobileCodeRspEv* mcre = static_cast<MobileCodeRspEv*>(ev);
                LOG_DEBUG("DispatchMsgService::handleAllResponseEvent - id：EEVENTID_GET_MOBLIE_CODE_RSP\n");

                ConnectSession* cs = (ConnectSession*)ev->get_args();
                cs->response = ev;

                //系列化请求数据
                cs->message_len = mcre->ByteSize();
                cs->write_buf = new char[cs->message_len + MESSAGE_HEADER_LEN];


                //组装头部
                memcpy(cs->write_buf, MESSAGE_HEADER_ID, 4);
                *(u16*)(cs->write_buf + 4) = EEVENTID_GET_MOBLIE_CODE_RSP;
                *(i32*)(cs->write_buf + 6) = cs->message_len;

                mcre->SerializeToArray(cs->write_buf + MESSAGE_HEADER_LEN, cs->message_len);

                interface->send_response_message(cs);
            }
            else if (ev->get_eid() == EEVENTID_EXIT_RSP)
            {
                ConnectSession* cs = (ConnectSession*)ev->get_args();
                cs->response = ev;
                interface->send_response_message(cs);
            }
        }


    }


}


