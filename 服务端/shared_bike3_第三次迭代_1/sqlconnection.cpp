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

    //��ʼ��
    if (mysql_init(mysql_) == NULL) {
        LOG_ERROR("init mysql failed %s , %d",this->GetErrInfo(),errno);
        return false;
    }
    //��Ϊ�����ԭ��,�Ͽ����Զ�����
    char cAuto = 1;
    if (mysql_options(mysql_, MYSQL_OPT_RECONNECT, &cAuto) != 0)//�ɹ�����0,���ɹ����ط�0
    {
        LOG_ERROR("mysql_options MYSQL_OPT_RECONNEC failed.");
    }
    //����
    if (mysql_real_connect(mysql_, szHost, szUser, szPasswd, szDb, nPort, NULL, 0) == NULL)
    {
        LOG_ERROR("connect mysql failed : %s ", this->GetErrInfo());
        return false;
    }
    return true;

}
/*
* ִ��sql���,���ǲ���Ҫ����(��insert,select,update��)
* �����ѯִ��ʧ�ܣ��� mysql_real_query() ��������ֵ��Ϊ 0����
* �������� mysql_errno() ���صĴ������Ƿ��� CR_SERVER_GONE_ERROR��
* �ô������ʾ�� MySQL �������������Ѿ��Ͽ�������ǣ������ Reconnect() �������������������ݿ⡣
* �����Ƿ�ɹ��������������᷵�� false�������ѯִ�гɹ����������� true��
*/
bool MysqlConnection::Execute(const char* szSql)//szSql ��һ�� C �����ַ�������ʾҪִ�е� SQL ��ѯ���
{
    if ( mysql_real_query(mysql_, szSql, strlen(szSql)) != 0 )
    {
        if (mysql_errno(mysql_) == CR_SERVER_GONE_ERROR )//�Ͽ����Ӿ�����
        {
            Reconnect();
        }
        return false;
    }
    return true;
}

bool MysqlConnection::Execute(const char* szSql, SqlRecordSet& recordSet)
{
    //�Ƚ���sql��ѯ,���Ƿ��ܹ�ִ�гɹ�?
    if (mysql_real_query(mysql_, szSql, strlen(szSql)) != 0)
    {
        if (mysql_errno(mysql_) == CR_SERVER_GONE_ERROR)
        {
            Reconnect();
        }
        return false;
    }
    //ִ�гɹ���,�ͽ��鵽�Ľ�����õ��������
    MYSQL_RES* pRes = mysql_store_result(mysql_);
    if (!pRes) {
        return NULL;//����ʧ��,���ؿ�,˵��mysql_store_resultʧ����
    }
    recordSet.SetResult(pRes);//�������������
    return false;
}

/*
* pSrc  ת��ǰ���ַ�
* pDest ת�����ַ�
*/
int MysqlConnection::EscapeString(const char* pSrc, int nSrcLen, char* pDest)
{
    if (!mysql_)
    {
        return 0;
    }
    return mysql_real_escape_string(mysql_,pDest,pSrc,nSrcLen);//��Դsrcת�嵽Ŀ���Ӵ�dest
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
