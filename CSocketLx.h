#pragma once
#include<WinSock2.h>
class CSockLx
{
	SOCKET m_hSocket;
public:
	CSockLx() :m_hSocket(INVALID_SOCKET)
	{
		WSAData wd;
		WSAStartup(0x0202, &wd);
	}
	~CSockLx()
	{
		if (m_hSocket != INVALID_SOCKET)
			closesocket(m_hSocket);
	}
	static int GetLastError()
	{
		return WSAGetLastError();
	}
	int Receive(void* lpBuf, int nBufLen, int nFlags = 0)
	{
		return recv(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
	}
	BOOL Listen(int backlog = 5)
	{//TCPServer
		return 0 == listen(m_hSocket, backlog);
	}
	int Send(const void* lpBuf, int nBufLen, int nFlags = 0)
	{
		return send(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
	}

	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCSTR sIP, int nFlags = 0);
	int ReceiveFrom(void* lpBuf, int nBufLen, char* sIP = nullptr, UINT* pPort = nullptr, int nFlags = 0);

	BOOL Accept(CSockLx& socka, LPSTR sIP = nullptr, PUINT pPort = nullptr); //SERVER
	BOOL Connect(LPCSTR lpszHostAddress, UINT nHostPort);//CLIENT

	BOOL Create(UINT nSocketPort=0, int nSocketType = SOCK_STREAM, LPCSTR sIP = nullptr);
	BOOL GetSokcetName(LPSTR sIP = nullptr, PUINT pPort = nullptr);
		BOOL GetPeerName(LPSTR sIP = nullptr, PUINT pPort = nullptr);
		void close()
		{
			closesocket(m_hSocket);
			m_hSocket = INVALID_SOCKET;
		}
		operator SOCKET()const
		{
			return m_hSocket;
		}
};
#pragma comment(lib,"ws2_32.lib")