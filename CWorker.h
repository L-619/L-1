#pragma once
#include"define.h"
#include<mysql.h>
class CWorker
{
	void MakeInfo(SInfo& d, MYSQL_ROW row);
	MYSQL& m_sql;
	bool Select(const char* sSQL,CSockLx *pSocka);
public:

	CWorker(MYSQL& sql) :m_sql(sql)
	{

	}
	bool FindNumb(CSockLx* pSocka); 
	bool FindName(CSockLx* pSocka);
	bool FindSalary(CSockLx* pSocka);
	bool FindDate(CSockLx* pSocka);
	bool Input(CSockLx*pSocka);
	bool Delete(CSockLx* pSocka);
	bool Print(CSockLx* pSocka);
	bool Modify(CSockLx* pSocka);
};

