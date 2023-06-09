#include "events_def.h"
#include <iostream>
#include <sstream>

std::ostream& MobileCodeReqEv::dump(std::ostream& out) const
{
	out << "MobileCodeReq sn=" << get_sn() << ",";
	out << "mobile=" << msg_.mobile() << std::endl;
	return out;
}

std::ostream& MobileCodeRspEv::dump(std::ostream& out) const
{
	out << "MobileCodeReqEv sn =" << get_sn() << ",";

	out << "code=" <<msg_.code() << std::endl;//出现问题,一旦调用,code_与icode就会变为随机值

	out << "icode=" << msg_.icode() << std::endl;

	out << "describle = " << msg_.data() << std::endl;

	return out;
}