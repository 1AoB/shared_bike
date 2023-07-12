#include "BusProcessor.h"
#include "SqlTables.h"

BusinessProcessor::BusinessProcessor(std::shared_ptr<MysqlConnection> conn)
    :mysqlconn_(conn),ueh_(new UserEventHandler())
{

}

bool BusinessProcessor::init()
{
    SqlTables tables(mysqlconn_);
    if (tables.CreateUserInfo() == false)
    {
        printf("<BusProcesspr.cpp> init-CreateUserInfo failed.\n");
        return false;
    }
    if (tables.CreateBikeTable() == false)
    {
        printf("<BusProcesspr.cpp> init-CreateBikeTable failed.\n");
        return false;
    }
    
    return true;
}

BusinessProcessor::~BusinessProcessor()
{
    ueh_.reset();//调用后，该智能指针不再管理任何对象。注意，这里的释放并不是真正地去释放，只是引用计数减 1，而释放是引用计数为 0 时自行进行的。
}
