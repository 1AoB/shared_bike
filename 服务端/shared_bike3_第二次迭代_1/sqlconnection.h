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

	explicit SqlRecordSet(MYSQL_RES* pRes)// 不能隐式构造
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
	* 你已经设置了结果集,此时若要再次设置结果集,那么之前的结果集就访问不到了.(那之前的结果集访问不到了,之间的结果集还没来的及释放,就是内存泄漏)
	* 所以,你要设置结果集的前提是:结果集是空的.
	* 不是空的,咱们不让他设置
	*/
	//设置结果集
	inline void SetResult(MYSQL_RES* pRes)
	{
		//如果此时已经保存了结果集,那么就应该让程序报错,防止内存泄漏
		assert(m_pRes == NULL);
		if (m_pRes)
		{
			LOG_WARN("the MYSQL_RES has already stored result , maybe will case memory leak\n");
		}
		m_pRes = pRes;
	}
	//获取结果集
	inline MYSQL_RES* GetResult()
	{
		return m_pRes;
	}

	//获取里面的行
	void FetchRow(MYSQL_ROW& row)
	{
		row = mysql_fetch_row(m_pRes);
	}

	//返回具体行的数量
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
	
	//将pSrc特殊字符进行转义,一些特殊字符如果不转义,sql查询就会报错
	int EscapeString(const char* pSrc, int nSrcLen, char* pDest);

	void Close();
	//得到错误信息的方法
	const char* GetErrInfo();

	//服务断掉了,重连
	void Reconnect();
private:
	MYSQL* mysql_;//mysql的句柄,用于操作数据库
};

#endif


