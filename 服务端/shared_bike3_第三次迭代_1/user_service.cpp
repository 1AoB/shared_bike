#include "user_service.h"
UserService::UserService(std::shared_ptr<MysqlConnection> sql_conn)
    :sql_conn_(sql_conn)//<-
{
}

bool UserService::exist(const std::string& mobile)
{
    char sql_connect[1024] = { 0 };
    sprintf(sql_connect, \
        "select * from userinfo where mobile = %s",\
        mobile.c_str());//将"select * from userinfo where mobile = %s"放入到sql_connect中
    SqlRecordSet record_set;
    if (!sql_conn_->Execute(sql_connect, record_set))//查到了就放入到record_set中
    {
        LOG_DEBUG("<user_service.cpp>-exit Execute failed\n");
        return false;
    }
    return (record_set.GetRowCount() != 0);
}

bool UserService::insert(const std::string& mobile)
{
    char sql_content[1024] = { 0 };
    sprintf(sql_content, \
        "insert into userinfo (mobile) values (%s)",
        mobile.c_str());
    return sql_conn_->Execute(sql_content);
}

UserService::~UserService()
{
}
