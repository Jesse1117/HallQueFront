#pragma once


#include "../HallQueFront/SocketBufDef.h"
#include <string>
#include <list>
using namespace std;
class CComplSocketClient
{
public:
	CComplSocketClient(void);
	~CComplSocketClient(void);

	void SetRemoteIp(const CString& ip);
	void SetRemotePort(UINT port);
	void SetTimeOut(UINT nMsec);

	BOOL InitClient();
	void StopClient();
	BOOL SendData(const CString& msg,std::string& recvMsg,int& actRecvSize);
	BOOL SendData(USHORT port,CString IP,const CString& msg,std::string& recvMsg,int& actRecvSize);
	BOOL SendData(USHORT port,CString IP,const std::string& msg,int size,std::string& recvMsg,int& actRecvSize);
private:
	CString m_romoteIP;
	UINT m_romotePort;
	SOCKET m_sClient;
	UINT m_nTimeOut;
	CString m_caChePath;
private:
	///¥¶¿Ìª∫¥Ê
	void DealCache(const CString& msg);
	void ReSendMsg();
	BOOL AppendListMsg();
};
