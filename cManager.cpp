#include "cManager.h"
#include"define.h"
#include<process.h>
using namespace std;
cManager cManager::m_man;
bool cManager::ConnectDb()
{
	auto conn = mysql_init(&m_sql);
	if (conn == NULL)
	{
		PrintError("mysql_init");
		return false;
	}
	//host参数可以用："localhost",".","192.168.0.88","127.0.0.1","teacher"
	if (mysql_real_connect(conn, "127.0.0.1", "root", "123456", "worker", 3306, NULL, 0) == NULL)
	{
		PrintError("mysql_real_connect");
		return false;
	}
	/*if (mysql_set_character_set(conn, "utf8mb4") != 0) {
		std::cerr << "设置字符集失败: " << mysql_error(conn) << std::endl;
	}*/
	mysql_query(conn, "set names gbk");//utf-8和ansi（gb2312，gbk）
	//mysql_query(conn, "use worker");
	return true;

}

void cManager::recvProc(void* p)
{
	CSockLx* pSocka = (CSockLx*)p;
	if (!pSocka)
		return;
	while (m_man.Receive(pSocka));
	cout << "有客户端断开，离开线程" << *pSocka << endl;
	delete pSocka;
}

BOOL cManager::Receive(CSockLx* pSocka)
{
	int nCmd = 0;
	if (pSocka->Receive(&nCmd, sizeof(nCmd)) < sizeof(nCmd))
		return FALSE;
	switch (nCmd)
	{
	case WK_MODIFY:
		return m_worker.Modify(pSocka);
	case WK_DELETE:
		return m_worker.Delete(pSocka);
	case WK_PRINT:
		return m_worker.Print(pSocka);
	
	case WK_INPUT:
			return m_worker.Input(pSocka);
	case WK_FIND_NUMB:
			return m_worker.FindNumb(pSocka);
	case WK_FIND_NAME:
		return m_worker.FindName(pSocka);
	case WK_FIND_SALARY:
		return m_worker.FindSalary(pSocka);
	case WK_FIND_DATE:
		return m_worker.FindDate(pSocka);
	case AD_LOGIN:
		return m_admin.Login(pSocka);
	case AD_CHHPASS:
		return m_admin.ChangePass(pSocka);
	case AD_PRINT:
		return m_admin.Print(pSocka);
	case AD_DELETE:
		return m_admin.Delete(pSocka);
	case AD_CHECK:
		return m_admin.Check(pSocka);
	case AD_INPUT:
		return  m_admin.Input(pSocka);
	}

	return FALSE;
}

void cManager::WriteLog(const char* sSQL, const char* sFile, int nLine)
{
}

int cManager::Main()
{
	if (!ConnectDb())
		return -1;
	if (!m_sock.Create(SEVER_PORT))
	{
		cout << "端口创建失败:" << m_sock.GetLastError() << endl;
		return -1;
	}
	m_sock.Listen();
	char sIP[20];
	UINT nPort;
	while (true)
	{
		auto pSock = new CSockLx;
		if (!m_sock.Accept(*pSock, sIP, &nPort))
			break;
		_beginthread(recvProc, 0, pSock);
		cout << "有客户端连接进来" << sIP << "-" <<nPort << endl;
	}
	
	return 0;
}
