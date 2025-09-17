#include "CSocketLx.h"
#include<WinSock2.h>
#include<WS2tcpip.h>
int CSockLx::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort,
    LPCSTR sIP, int nFlags)
{
    sockaddr_in sa = { AF_INET,htons(nHostPort) };
    inet_pton(AF_INET, sIP, &sa.sin_addr);
    return sendto(m_hSocket, (const char*)lpBuf, nBufLen, nFlags,
        (sockaddr*)&sa, sizeof(sa));

}

int CSockLx::ReceiveFrom(void* lpBuf, int nBufLen, char* sIP, UINT* pPort, int nFlags)
{
    sockaddr_in sa;
    int nLen = sizeof(sa);
    int n = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, (sockaddr*)&sa, &nLen);
    if (n <= 0)
        return n;
    if (sIP)
    {
        inet_ntop(AF_INET, &sa.sin_addr, sIP, 20);
    }
    if (pPort)
        *pPort = htons(sa.sin_port);
    return n;
}
BOOL CSockLx::Accept(CSockLx& socka, LPSTR sIP, PUINT pPort)
{
    sockaddr_in sa;
    int nLen = sizeof(sa);
    socka.m_hSocket=accept(m_hSocket, (sockaddr*)&sa, &nLen);
     if (INVALID_SOCKET == socka.m_hSocket)
        return FALSE;
    if (sIP)
    {
        inet_ntop(AF_INET, &sa.sin_addr, sIP, 20);
    }
    if (pPort)
        *pPort = htons(sa.sin_port);
  
    return TRUE;
}
BOOL CSockLx::Connect(LPCSTR lpszHostAddress, UINT nHostPort)
{
    sockaddr_in sa = { AF_INET,htons(nHostPort) };
    inet_pton(AF_INET, lpszHostAddress, &sa.sin_addr);
    return !connect(m_hSocket, (sockaddr*)&sa, sizeof(sa));
}
//´´½¨¾ä±ú->m_hsocket  bind(m_hSocket,->nSocketPort
BOOL CSockLx::Create(UINT nSocketPort, int nSocketType, LPCSTR sIP)
{
    m_hSocket = socket(AF_INET, nSocketType, 0);
    if (INVALID_SOCKET == m_hSocket)
        return false;
    sockaddr_in sa = { AF_INET,htons(nSocketPort) };
    if (sIP)
        inet_pton(AF_INET, sIP, &sa.sin_addr);
    int n = bind(m_hSocket, (sockaddr*)&sa, sizeof(sa));
    if (n)
        closesocket(m_hSocket);
    return n == 0;
}

BOOL CSockLx::GetSokcetName(LPSTR sIP, PUINT pPort)
{

    sockaddr_in sa;
    int nLen = sizeof(sa);
    int n = getsockname(m_hSocket, (sockaddr*)&sa, &nLen);
    if (n)
        return FALSE;
    if (sIP)
    {
        inet_ntop(AF_INET, &sa.sin_addr, sIP, 20);
    }
    if (pPort)
        *pPort = htons(sa.sin_port);

    return TRUE;
}

BOOL CSockLx::GetPeerName(LPSTR sIP, PUINT pPort)
{
    sockaddr_in sa;
    int nLen = sizeof(sa);
    int n=getpeername(m_hSocket, (sockaddr*)&sa, &nLen);
    if (n)
        return FALSE;
    if (sIP)
    {
        inet_ntop(AF_INET, &sa.sin_addr, sIP, 20);
    }
    if (pPort)
        *pPort = htons(sa.sin_port);

    return TRUE;
}

