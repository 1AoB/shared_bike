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
    ueh_.reset();//���ú󣬸�����ָ�벻�ٹ����κζ���ע�⣬������ͷŲ�����������ȥ�ͷţ�ֻ�����ü����� 1�����ͷ������ü���Ϊ 0 ʱ���н��еġ�
}
