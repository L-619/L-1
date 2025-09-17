#include "CWorker.h"
#include"cManager.h" 
#include<string>
#include<sstream>
#include<iomanip>
using namespace std;
void CWorker::MakeInfo(SInfo& d, MYSQL_ROW row)
{
	if(*row)
	d.nNumb = atoi(*row);
	++row;
	if (*row)
	strcpy(d.sName, *row);
	++row;
	if (*row)
		d.fsala = (float)atof(*row);
	++row;
	if (*row)
	strcpy(d.sDate, *row);
}
bool CWorker::Select(const char* sSQL,CSockLx*pSocka)
{
	int n = mysql_query(&m_sql, sSQL);
	if (n)
	{
		cManager::PrintError(sSQL);
		return false;
	}
	auto res = mysql_store_result(&m_sql);
	if (!res)
	{
		cManager::PrintError(sSQL);
		return false;
	}
	MYSQL_ROW row = nullptr;
	//先发nCount 

	while (row = mysql_fetch_row(res))
	{
		SInfo d = SInfo();
		MakeInfo(d, row);

		pSocka->Send(&d, sizeof(d));
	}
	pSocka->Send(&n, sizeof(n));
	return true;

}
bool CWorker::FindNumb(CSockLx* pSocka)
{
	int nNumb;
	if (pSocka->Receive(&nNumb, sizeof(nNumb)) < sizeof(nNumb))
		return false;
	string  sSQL = "SELECT * FROM t_worker WHERE f_numb=" + to_string(nNumb);
	int n = mysql_query(&m_sql, sSQL.c_str());
	if (n)
	{
		cManager::PrintError(sSQL.c_str());//WriteLog(
		return false;
	}
	auto res = mysql_store_result(&m_sql);
	if (!res)
	{
		cManager::PrintError(sSQL.c_str());//WriteLog(
		return false;
	}
	int nCount = (int)mysql_num_rows(res);//bool b 
	pSocka->Send(&nCount, sizeof(nCount));
	if (nCount > 0)
	{
		SInfo d{};
		auto row = mysql_fetch_row(res);
		if (row)
			MakeInfo(d, row);
		pSocka->Send(&d, sizeof(d));
	}
	return true;
}

bool CWorker::FindName(CSockLx* pSocka)
{
	char sName[20];
	int n = pSocka->Receive(sName, sizeof(sName));
	if (n <= 0 || n > sizeof(sName))
		return false;
	sName[n] = '\0';
	string sSQL = "SELECT * FROM t_worker WHERE f_name LIKE '%";
	sSQL += sName;
	sSQL += "%'";
	return Select(sSQL.c_str(), pSocka);
}

bool CWorker::FindSalary(CSockLx* pSocka)
{
	float fMin{}, fMax{};
	if (pSocka->Receive(&fMin, sizeof(fMin)) < sizeof(fMin))
		return false;
	if (pSocka->Receive(&fMax, sizeof(fMax)) < sizeof(fMax))
		return false;
	char sSQL[256];
	sprintf(sSQL, "SELECT * FROM t_worker WHERE f_sala>=%0.2f AND f_sala<=%0.2f", fMin, fMax);
	return Select(sSQL,pSocka);
}

bool CWorker::FindDate(CSockLx* pSocka)
{
	char s1[20],s2[20];
	int n = pSocka->Receive(&s1, sizeof(s1));
	if ( n<=0)
		return false;
	s1[n] = '\0';
	pSocka->Send(&n, sizeof(n));
	n = pSocka->Receive(&s2, sizeof(s2));
	if (n<=0)
		return false;
	s2[n] = '\0';
	char sSQL[256];
	sprintf(sSQL, "SELECT * FROM t_worker WHERE f_data>='%s' AND f_data<='%s'", s1, s2);
	return Select(sSQL, pSocka);
}

bool CWorker::Input(CSockLx* pSocka)
{
	SInfo d;
	if(pSocka->Receive(&d,sizeof(d))<sizeof(d))
	return false;
	char sSQL[256];
	sprintf(sSQL, "INSERT INTO t_worker(f_numb,f_name,f_sala,f_data) VALUES(%d,'%s',%0.2f,'%s')",
		d.nNumb, d.sName, d.fsala, d.sDate);
	int n = mysql_query(&m_sql, sSQL);
	if (n)
	{
		cManager::PrintError(sSQL);
		return false;
	}
	return true;
}

bool CWorker::Delete(CSockLx* pSocka)
{
	int nNumb;
	if (pSocka->Receive(&nNumb, sizeof(nNumb)) < sizeof(nNumb))
		return false;
	auto sSQL = "DELETE FROM t_worker WHERE f_numb=" + to_string(nNumb);
	int n = mysql_query(&m_sql, sSQL.c_str());
	if (n)
	{
		cManager::PrintError(sSQL.c_str());
		return false;
	}
	//执行结果发送回客户端
	n = (int)mysql_affected_rows(&m_sql);
	pSocka->Send(&n, sizeof(n));
	return true;
}

bool CWorker::Print(CSockLx* pSocka)
{
	int nOrder = -1;
	if(pSocka->Receive(&nOrder,sizeof(nOrder))!=sizeof(nOrder))
	return false;
	if (nOrder > 4 || nOrder < -1)
		return false;
	string ss[] = { "f_ctime","f_numb","f_name","f_sala","f_data" };
	string sSQL = "SELECT * FROM t_worker ORDER BY " + ss[nOrder + 1];
	return Select(sSQL.c_str(), pSocka);//后发送结束包，n是不完整的结构体

}


bool CWorker::Modify(CSockLx* pSocka)
{
	SInfo d;
	if (pSocka->Receive(&d, sizeof(d)) != sizeof(d))
		return false;
	stringstream ss;
	ss << "UPDATE t_worker SET f_name='" 
		<< d.sName <<"',f_sala=" 
		<< setiosflags(ios::fixed) << setprecision(2) << d.fsala << ",f_data='"
		<< d.sDate << "' WHERE f_numb=" << d.nNumb;
	string  sSQL = ss.str();
	int n = mysql_query(&m_sql, sSQL.c_str());
	if (n)
	{
		cManager::PrintError(sSQL.c_str());
		return false;
	}
	return true;
}
