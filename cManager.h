#pragma once
#include"CWorker.h"
#include<iostream>
#include"CAdmin.h"
class cManager
{
	CAdmin m_admin{ m_sql };
	MYSQL m_sql;
	CSockLx m_sock;
	CWorker m_worker{ m_sql };
	bool ConnectDb();
	static void recvProc(void* p);
	BOOL Receive(CSockLx* pSocka);

public:
	static cManager m_man;
	static void PrintError(const char* sSQL)
	{
		printf("SQLÓï¾ä´íÎó£º%s\n %u: %s\n", sSQL, mysql_errno(&m_man.m_sql),
			mysql_error(&m_man.m_sql));
	}

	static void WriteLog(const char* sSQL,const char* sFile,int nLine );
	int Main();
};

