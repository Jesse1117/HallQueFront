#pragma once
#include <WinSock2.h>
#include "afxmt.h"
#include "SLZData.h"
#include <string>
#pragma comment(lib, "Ws2_32.lib")  

class CTcpSever
{
public:
	CTcpSever(void);
	~CTcpSever(void);	
	
	BOOL Start();
private:
	BOOL InitSocket();
	static DWORD WINAPI WorkerThread(LPVOID lpParam);
	static DWORD WINAPI AcceptThread(LPVOID lpParam);

	void WriteErrLog(CString strErr);
	void WriteLogWithTime(CString strSockLog);			//–¥»Î¥ÌŒÛ»’÷æ
	BOOL AppendWriteFile(CString strText, CString strFileName);
	CString m_strCallPath;
	CString m_strLogFilePath;
	HANDLE m_hAcceptThreadHandle;
	HANDLE m_hWorhThreadHandle;
	SOCKET m_ListeningSocket;
	CMutex m_ServerLock;
	UINT m_uListenPort;
	SLZData Dodata(std::string	buf);
	void LoadConfig();
	CString m_strIP;
	CString m_strPort;
	UINT m_iWaitTime;
};
