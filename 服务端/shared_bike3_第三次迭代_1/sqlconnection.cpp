#include "sqlconnection.h"
#include <string.h>
MysqlConnection::MysqlConnection()
{
    mysql_ = (MYSQL*)malloc(sizeof(MYSQL));
}

MysqlConnection::~MysqlConnection()
{
    if (mysql_ != NULL)
    {
        mysql_close(mysql_);
        free(mysql_);
        mysql_ = NULL;
    }
    return;
}

bool MysqlConnection::Init(const char* szHost, int nPort, const char* szUser, const char* szPasswd, const char* szDb)
{
    LOG_INFO("enter Init.\n");

    //初始化
    if (mysql_init(mysql_) == NULL) {
        LOG_ERROR("init mysql failed %s , %d",this->GetErrInfo(),errno);
        return false;
    }
    //因为网络等原因,断开后自动重连
    char cAuto = 1;
    if (mysql_options(mysql_, MYSQL_OPT_RECONNECT, &cAuto) != 0)//成功返回0,不成功返回非0
    {
        LOG_ERROR("mysql_options MYSQL_OPT_RECONNEC failed.");
    }
    //连接
    if (mysql_real_connect(mysql_, szHost, szUser, szPasswd, szDb, nPort, NULL, 0) == NULL)
    {
        LOG_ERROR("connect mysql failed : %s ", this->GetErrInfo());
        return false;
    }
    return true;

}
/*
* 执行sql语句,但是不需要数据(如insert,select,update等)
* 如果查询执行失败（即 mysql_real_query() 函数返回值不为 0），
* 函数会检查 mysql_errno() 返回的错误码是否是 CR_SERVER_GONE_ERROR，
* 该错误码表示与 MySQL 服务器的连接已经断开。如果是，则调用 Reconnect() 函数尝试重新连接数据库。
* 无论是否成功重连，函数都会返回 false。如果查询执行成功，则函数返回 true。
*/
bool MysqlConnection::Execute(const char* szSql)//szSql 是一个 C 风格的字符串，表示要执行的 SQL 查询语句
{
    if ( mysql_real_query(mysql_, szSql, strlen(szSql)) != 0 )
    {
        if (mysql_errno(mysql_) == CR_SERVER_GONE_ERROR )//断开连接就重连
        {
            Reconnect();
        }
        return false;
    }
    return true;
}

bool MysqlConnection::Execute(const char* szSql, SqlRecordSet& recordSet)
{
    //先进行sql查询,看是否能够执行成功?
    if (mysql_real_query(mysql_, szSql, strlen(szSql)) != 0)
    {
        if (mysql_errno(mysql_) == CR_SERVER_GONE_ERROR)
        {
            Reconnect();
        }
        return false;
    }
    //执行成功了,就将查到的结果设置到结果集中
    MYSQL_RES* pRes = mysql_store_result(mysql_);
    if (!pRes) {
        return NULL;//设置失败,返回空,说明mysql_store_result失败了
    }
    recordSet.SetResult(pRes);//将结果放入结果集
    return false;
}

/*
* pSrc  转义前的字符
* pDest 转义后的字符
*/
int MysqlConnection::EscapeString(const char* pSrc, int nSrcLen, char* pDest)
{
    if (!mysql_)
    {
        return 0;
    }
    return mysql_real_escape_string(mysql_,pDest,pSrc,nSrcLen);//将源src转义到目标子串dest
}

void MysqlConnection::Close()
{

}

const char* MysqlConnection::GetErrInfo()
{

    return mysql_error(mysql_);
}

void MysqlConnection::Reconnect()
{
    mysql_ping(mysql_);
}
