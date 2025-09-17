#pragma once
#include "define.h"
#include<mysql.h>
class CAdmin
{
	MYSQL &m_sql;
	void MakeInfo(sAdmin& a, MYSQL_ROW row);
	bool Select(const char* sSQL,CSockLx*psocka );
public:
	CAdmin(MYSQL& sql) :m_sql(sql)
	{

	}
	bool Login(CSockLx* pSocka);
	bool ChangePass(CSockLx* pSocka);
	bool Input(CSockLx* pSocka);
	bool Delete(CSockLx* pSocka);
	bool Print(CSockLx* pSocka);
	bool Check(CSockLx* pSocka);
};

