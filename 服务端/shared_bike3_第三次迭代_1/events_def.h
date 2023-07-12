#ifndef BRKS_COMMON_EVENTS_DEF_H_
#define BRKS_COMMON_EVENTS_DEF_H_

#include "ievent.h"
#include <string>
#include "bike.pb.h"

class MobileCodeReqEv : public iEvent//request
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

class MobileCodeRspEv : public iEvent//response
{
public:
	MobileCodeRspEv(i32 code, i32 icode) :
		iEvent(EEVENTID_GET_MOBLIE_CODE_RSP, iEvent::generateSeqNo()) {
		printf("code: %d , icode:%d\n", code, icode);
		const char* tmp = getReasonByErrorCode(code);
		printf("tmp:%s\n", tmp);

		msg_.set_code(code);
		msg_.set_icode(icode);
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


class  ExitRspEv :public iEvent//response
{
public:
	ExitRspEv() :
		iEvent(EEVENTID_EXIT_RSP, iEvent::generateSeqNo())
	{
	}
};

class LoginReqEv :public iEvent{//request
public:
	LoginReqEv(const std::string& mobile, i32 icode)
		:iEvent(EEVENTID_LOGIN_REQ, iEvent::generateSeqNo()) 
	{
		msg_.set_mobile(mobile);
		msg_.set_icode(icode);
	}

	const std::string& get_mobile() { return msg_.mobile(); }
	const i32 get_icode() { return msg_.icode(); }

	virtual std::ostream& dump(std::ostream& out)const;
	virtual i32 ByteSize() {return msg_.ByteSize(); }
	virtual bool SerializeToArray(char* buf, int len) { return msg_.SerializeToArray(buf, len); }
private:
	tutorial::login_request msg_;
};

class LoginResEv :public iEvent {//response rsp res
public:
	LoginResEv(i32 code):
		iEvent(EEVENTID_LOGIN_RSP,iEvent::generateSeqNo())
	{
		msg_.set_code(code);
		msg_.set_desc(getReasonByErrorCode(code));
	}
	const i32 get_code() { return msg_.code(); }
	const std::string& get_desc() { return msg_.desc(); }

	virtual std::ostream& dump(std::ostream& out) const;
	virtual i32 ByteSize() { return msg_.ByteSize(); }
	virtual bool SerializeToArray(char* buf, int len) { return msg_.SerializeToArray(buf, len); }

	~LoginResEv() {

	}
private:
	tutorial::login_response msg_;
};
#endif