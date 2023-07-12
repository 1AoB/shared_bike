#include "events_def.h"
#include <iostream>
#include <sstream>

std::ostream& MobileCodeReqEv::dump(std::ostream& out) const
{
	out << "MobileCodeReq sn" << get_sn() << ",";
	out << "mobile=" << msg_.username() << std::endl;
	return out;
}

std::ostream& MobileCodeRspEv::dump(std::ostream& out) const
{
	out << "MobileCodeRspEv sn" << get_sn() << ",";
	out << "rescode=" << msg_.rescode() << std::endl;
	out << "vercode=" << msg_.vercode() << std::endl;
	out << "describel=" << msg_.data() << std::endl;
	return out;
}

std::ostream& LoginReqEv::dump(std::ostream& out) const
{
	out << "LoginReqEv sn=" << get_sn() << ",";
	out << "username:" << msg_.username() << std::endl;
	//out << "vercode:" << msg_.() << std::endl;
	return out;
}

std::ostream& LoginResEv::dump(std::ostream& out) const
{
	out << "LoginResEv sn=" << get_sn() << ",";
	out << "rescode:" << msg_.rescode() << std::endl;
	//out << "describle:" << msg_.desc() << std::endl;
	return out;
}

std::ostream& ListAccountRecordsReqEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& ListAccountRecordsReqEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& ListAccountRecordsResonseEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& ListAccountRecordsResonseEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeScanQRStartReqEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeScanQRStartReqEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeScanQRStartResponseEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeScanQRStartResponseEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeScanQREndReqEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeScanQREndReqEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeScanQREndResponseEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeScanQREndResponseEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& broadcastMsgReqEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& broadcastMsgReqEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& broadcastMsgResponseEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& broadcastMsgResponseEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeScanQRFaultReqEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeScanQRFaultReqEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeScanQRFaultResponseEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeScanQRFaultResponseEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeScanQRRepairFinishReqEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeScanQRRepairFinishReqEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeScanQRRepairFinishResponseEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeScanQRRepairFinishResponseEv::dump(std::ostream& out) const");
	return out;
}

void bikeInfoResponseEv::set_bikeResponse_msg(i32 bikeId, i32 bikeStatus, r64 longitude, r64 latitude)
{
	tutorial::list_all_bikeinfo_response_bikeinfo* bikeinfo = msg_.add_records();
	bikeinfo->set_bikeid(bikeId);
	bikeinfo->set_bikestatus(bikeStatus);
	bikeinfo->set_longitude(longitude);
	bikeinfo->set_latitude(latitude);
}

std::ostream& bikeInfoResponseEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeInfoResponseEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& bikeInfoReqEv::dump(std::ostream& out) const
{
	LOG_DEBUG("std::ostream& bikeInfoReqEv::dump(std::ostream& out) const");
	return out;
}

std::ostream& RegisterResEv::dump(std::ostream& out) const
{
	// TODO: 在此处插入 return 语句
}

std::ostream& RegisterReqEv::dump(std::ostream& out) const
{
	// TODO: 在此处插入 return 语句
}

std::ostream& AddBikeReqEv::dump(std::ostream& out) const
{
	// TODO: 在此处插入 return 语句
}

std::ostream& DeleteBikeReqEv::dump(std::ostream& out) const
{
	// TODO: 在此处插入 return 语句
}

std::ostream& DeleteBikeResEv::dump(std::ostream& out) const
{
	// TODO: 在此处插入 return 语句
}

std::ostream& AddBikeResEv::dump(std::ostream& out) const
{
	// TODO: 在此处插入 return 语句
}
