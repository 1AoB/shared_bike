#ifndef DATASTORE_MYSQL_CONNECTION_H_
#define DATASTORE_MYSQL_CONNECTION_H_

#include <mysql/mysql.h>
#include <string>
#include <mysql/errmsg.h>
#include <assert.h>

#include "glo_def.h"

class SqlRecordSet {
public:
	SqlRecordSet() :m_pRes(NULL)
	{

	}

	explicit SqlRecordSet(MYSQL_RES* pRes)// ������ʽ����
	{
		m_pRes = pRes;
	}
	MYSQL_RES* MysqlRes()
	{
		return m_pRes;
	}
	~SqlRecordSet() {
		if (m_pRes)
		{
			mysql_free_result(m_pRes);
		}
	}

	/*
	* ���Ѿ������˽����,��ʱ��Ҫ�ٴ����ý����,��ô֮ǰ�Ľ�����ͷ��ʲ�����.(��֮ǰ�Ľ�������ʲ�����,֮��Ľ������û���ļ��ͷ�,�����ڴ�й©)
	* ����,��Ҫ���ý������ǰ����:������ǿյ�.
	* ���ǿյ�,���ǲ���������
	*/
	//���ý����
	inline void SetResult(MYSQL_RES* pRes)
	{
		//�����ʱ�Ѿ������˽����,��ô��Ӧ���ó��򱨴�,��ֹ�ڴ�й©
		assert(m_pRes == NULL);
		if (m_pRes)
		{
			LOG_WARN("the MYSQL_RES has already stored result , maybe will case memory leak\n");
		}
		m_pRes = pRes;
	}
	//��ȡ�����
	inline MYSQL_RES* GetResult()
	{
		return m_pRes;
	}

	//��ȡ�������
	void FetchRow(MYSQL_ROW& row)
	{
		row = mysql_fetch_row(m_pRes);
	}

	//���ؾ����е�����
	inline i32 GetRowCount()
	{
		return m_pRes->row_count;
	}
private:
	MYSQL_RES* m_pRes;
};

class MysqlConnection {
public:
	MysqlConnection();
	~MysqlConnection();

	MYSQL* Mysql()
	{
		return mysql_;
	}

	bool Init(const char* szHost, int nPort, const char* szUser, const char* szPasswd, const char* szDb);
	bool Execute(const char* szSql);
	//MYSQL_RES *
	bool Execute(const char* szSql, SqlRecordSet& recordSet);
	
	//��pSrc�����ַ�����ת��,һЩ�����ַ������ת��,sql��ѯ�ͻᱨ��
	int EscapeString(const char* pSrc, int nSrcLen, char* pDest);

	void Close();
	//�õ�������Ϣ�ķ���
	const char* GetErrInfo();

	//����ϵ���,����
	void Reconnect();
private:
	MYSQL* mysql_;//mysql�ľ��,���ڲ������ݿ�
};

#endif


