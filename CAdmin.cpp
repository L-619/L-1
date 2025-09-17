#include "CAdmin.h"
#include<string>
#include"cManager.h"
using namespace std;
void CAdmin::MakeInfo(sAdmin& a, MYSQL_ROW row)
{
	if (*row)
		strcpy(a.sName, *row);
	++row;
	if (*row)
		strcpy(a.sPass, *row);
	++row;
	if (*row)
		a.nPrior =atoi( *row);
}
//��������ֹʹ��pause
bool CAdmin::Login(CSockLx* pSocka)
{// ����������-1�����˺�������󣬷���0��1�����˺Ŷ�ӦȨ��
	char sName[20], sPass[20];
	int n = 0;
	if (pSocka->Receive(&n, sizeof(n)) < sizeof(n))
		return false;
	if (n <= 0 || n >=sizeof(sName))
		return false;
	if (pSocka->Receive(sName, n) < n)
		return false;
	sName[n] = 0;
	if (pSocka->Receive(&n, sizeof(n)) < sizeof(n))
		return false;
	if (n <= 0 || n >= sizeof(sPass))
		return false;
	if (pSocka->Receive(sPass, n) < n)
		return false;
	sPass[n] = 0;
		string sSQL = "SELECT f_prior FROM t_admin WHERE f_name=\'" + (string)sName
		+ "\' AND BINARY f_pass=\'" + (string)sPass + '\'';
	 n = mysql_query(&m_sql, sSQL.c_str());
	if (n)
	{
		cManager::PrintError(sSQL.c_str());
		return false;
	}
	auto res = mysql_store_result(&m_sql);//��ʹ�ǲ�ƥ�䣬���طǿ�ֻ��row_count��0
	if (!res)
	{
		cout << "��ȡ��ѯ�������" << sSQL << endl;
		return false;
	}
	int nPrior = -1;
	MYSQL_ROW row = mysql_fetch_row(res);//���row_count��0��һ��fetch����NULL
	if (row)
	{
		if (*row)
			nPrior = atoi(row[0]);
	}
	pSocka->Send(&nPrior, sizeof(nPrior));	
    return true;
}

bool CAdmin::ChangePass(CSockLx* pSocka)
{
	sAdmin a;
	if(pSocka->Receive(&a,sizeof(a))<sizeof(a))
	return false;
	string sSQL = "UPDATE t_admin SET f_pass='" + (string)a.sPass +
		"' WHERE f_name='" + (string)a.sName + "'";
	int n = mysql_query(&m_sql, sSQL.c_str());
	if (n)
	{
		cManager::PrintError(sSQL.c_str());
		return false;
	}
	n = (int)mysql_affected_rows(&m_sql);
	pSocka->Send(&n, sizeof(n));

	return true;
}
bool CAdmin::Check(CSockLx* pSocka)
{
	char sName[20];
	int n = pSocka->Receive(sName, sizeof(sName));
	if (n <= 0 || n >= sizeof(sName))
	 return false;
	sName[n] = '\0';
	auto sSQL = "SELECT * FROM t_admin WHERE f_name='" + string(sName) + "'";
	return Select(sSQL.c_str(), pSocka);
}
bool CAdmin::Input(CSockLx* pSocka)
{
	sAdmin a;
	if (pSocka->Receive(&a, sizeof(a)) < sizeof(a))
		return false;
	char sSQL[256];
	sprintf(sSQL, "INSERT INTO t_admin(f_name,f_pass,f_prioR) VALUES('%s','%s',%d)",
		a.sName,a.sPass,a.nPrior);
		int n = mysql_query(&m_sql, sSQL);
		if (n)
		{
			cManager::PrintError(sSQL);
			return false;
		}
	return true;
}
bool CAdmin::Delete(CSockLx* pSocka)
{
	char sName[20];
	int n = pSocka->Receive(sName, sizeof(sName));
	if (n <= 0 || n >= sizeof(sName))
		return false;
	sName[n] = 0;
	if(!_stricmp(sName,"admin"))
	return true;
	string sSQL = "DELETE FROM t_admin WHERE f_name='" + string(sName) + "'";
	n = mysql_query(&m_sql, sSQL.c_str());
	if (n)
	{
		cManager::PrintError(sSQL.c_str());
		return false;
	}
	return true;
}
bool CAdmin::Select(const char* sSQL, CSockLx* pSocka)
{
	int n = mysql_query(&m_sql, sSQL);
	if (n)
	{
		cManager::PrintError(sSQL);
		return false;
	}
	auto res = mysql_store_result(&m_sql);//��ȡ�����
	if (!res)
	{
		cManager::PrintError(sSQL);
		return false;
	}
	MYSQL_ROW row = nullptr;
	//�ȷ�nCount 
	int nCount = (int)mysql_num_rows(res);//��ȡ�����������
	pSocka->Send(&nCount, sizeof(nCount));
	while (row = mysql_fetch_row(res))//����ȡ�������
	{
		sAdmin a{};
		MakeInfo(a, row);//������м�¼�ڽṹ��a��

		pSocka->Send(&a, sizeof(a));//���ͻؿͻ���
	}
	return true;
}
bool CAdmin::Print(CSockLx* pSocka)
{
	auto sSQL = "SELECT * FROM t_admin";
	return Select(sSQL, pSocka);
	
}