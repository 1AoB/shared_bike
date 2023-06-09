#ifndef BRKS_COMMON_EVENTS_DEF_H_
#define BRKS_COMMON_EVENTS_DEF_H_

#include "ievent.h"
#include <string>
#include "bike.pb.h"

class MobileCodeReqEv : public iEvent
{
public:
	MobileCodeReqEv(const std::string& mobile) :iEvent(EEVENTID_GET_MOBLIE_CODE_REQ, iEvent::generateSeqNo())
	{
		msg_.set_mobile(mobile);
	};

	const std::string& get_mobile() { return msg_.mobile(); };
	virtual std::ostream& dump(std::ostream& out) const;
	virtual i32 ByteSize() { return msg_.ByteSize(); };
	virtual bool SerializeToArray(char* buf, int len) { return msg_.SerializeToArray(buf, len); };

private:
	tutorial::mobile_request msg_;

};

class MobileCodeRspEv : public iEvent
{
public:
	MobileCodeRspEv(i32 code, i32 icode) :
		iEvent(EEVENTID_GET_MOBLIE_CODE_RSP, iEvent::generateSeqNo()) {
		printf("code: %d , icode:%d\n", code, icode);
		msg_.set_code(code);
		msg_.set_icode(icode);
		const char* tmp = getReasonByErrorCode(code);
		printf("tmp:%s\n", tmp);
		msg_.set_data(tmp);
	}


	const i32 get_code() { return msg_.code(); };
	const i32 get_icode() { return msg_.icode(); };
	const std::string& get_data() { return msg_.data(); };

	virtual std::ostream& dump(std::ostream& out) const;
	virtual i32 ByteSize() { return msg_.ByteSize(); };
	virtual bool SerializeToArray(char* buf, int len) { return msg_.SerializeToArray(buf, len); };


private:
	//std::string mobile_;
	tutorial::mobile_response msg_;
};


class  ExitRspEv :public iEvent
{
public:
	ExitRspEv() :
		iEvent(EEVENTID_EXIT_RSP, iEvent::generateSeqNo())
	{
	}
};

#endif