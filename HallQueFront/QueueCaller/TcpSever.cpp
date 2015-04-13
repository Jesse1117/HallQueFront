#include "StdAfx.h"
#include "TcpSever.h"
#include "CommonStrMethod.h"
#include "DealData.h"

CTcpSever::CTcpSever(void)
{
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
}

CTcpSever::~CTcpSever(void)
{
	if(m_hAcceptThreadHandle!=INVALID_HANDLE_VALUE)
	{
		TerminateThread(m_hAcceptThreadHandle,0);
		CloseHandle(m_hAcceptThreadHandle);
	}
	if(m_hWorhThreadHandle!=INVALID_HANDLE_VALUE)
	{
		TerminateThread(m_hWorhThreadHandle,0);
		CloseHandle(m_hWorhThreadHandle);
	}
	if(INVALID_SOCKET != m_ListeningSocket)  
	{  
		closesocket(m_ListeningSocket);  
		m_ListeningSocket = INVALID_SOCKET;  
	}  
}

BOOL CTcpSever::Start()
{
	if (!InitSocket())
	{
		return FALSE;
	}
	m_hWorhThreadHandle = CreateThread(NULL,0,WorkerThread,this,0,NULL);
	m_hAcceptThreadHandle = CreateThread(NULL,0,AcceptThread,this,0,NULL);
	if (m_hAcceptThreadHandle!=NULL&&m_hAcceptThreadHandle!=NULL)
	{
		return TRUE;
	}
	else return FALSE;
}

BOOL CTcpSever::InitSocket()
{
	WSADATA wsaData;
	SOCKADDR_IN addr_Server;//地址家族
	CString errcode;
	//winsock初始化
	int iError = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(iError != 0)
	{
		errcode.Format(_T("WSAStartup failed with error: %d"),GetLastError);
		AfxMessageBox(errcode);
		return FALSE;
	}
	//创建一个套接字绑定到一个特定的协议 
	//	m_SockBound = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	m_ListeningSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_ListeningSocket == INVALID_SOCKET)  
	{  
		errcode.Format(_T("WSAStartup failed with error: %d"),GetLastError);
		AfxMessageBox(errcode);
		WSACleanup();  //terminates use of the Winsock 2 DLL 
		return FALSE;  
	}  
	//构建本地地址信息  
	addr_Server.sin_family = AF_INET; //地址家族  
	/*USHORT port = GetListenPort();*/
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("SelfPort"),NULL,wbuf,255,m_strCallPath);
	int port = CommonStrMethod::Str2Int(wbuf);
	if(port > 0)
	{
		m_uListenPort = port;
	}
	//注意转化为网络字节序
	addr_Server.sin_port = htons(m_uListenPort);  
	//使用INADDR_ANY 指示任意地址 
	addr_Server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//绑定
	iError = bind(m_ListeningSocket,(LPSOCKADDR)&addr_Server,
		sizeof(addr_Server));
	if(iError == SOCKET_ERROR)
	{
		errcode.Format(_T("socket failed with error code: %d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_ListeningSocket); //关闭套接字  
		WSACleanup();  
		return FALSE;
	}
	//侦听连接请求  
	iError = listen(m_ListeningSocket, 5);  
	if (iError == SOCKET_ERROR)  
	{    
		errcode.Format(_T("socket failed with error code:%d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_ListeningSocket); //关闭套接字  
		return FALSE; 
	} 
	return TRUE;
}

DWORD WINAPI CTcpSever::WorkerThread(LPVOID lpParam)
{
	while(TRUE)
	{
		
	}
	return 0;
}

DWORD WINAPI CTcpSever::AcceptThread(LPVOID lpParam)
{
	CTcpSever* Server = (CTcpSever*) lpParam;
	while(TRUE)
	{
		SOCKADDR_IN clientAddr;
		int len = sizeof(clientAddr);
		SOCKET NewConnection = accept(Server->m_ListeningSocket,(SOCKADDR*)&clientAddr,&len);
		if (NewConnection!=SOCKET_ERROR)
		{
			char buf[512];
			memset(buf,0,512);
			if (SOCKET_ERROR!=::recv(NewConnection,buf,512,0))
			{
				if (strlen(buf)>0)
				{
					SLZData data = Server->Dodata(buf);
					CDealData::GetInstance()->AddData(data);
				}
			}
		}
	}
	return 0;
}

SLZData CTcpSever::Dodata(std::string buf)
{
	SLZData data;
	std::string::size_type firstIndex = buf.find("<dataType>");
	std::string::size_type lastIndex = buf.find("</dataType>");
	std::string SerialId = buf.substr(firstIndex+10,lastIndex-firstIndex-10);
	data.SetWaitTime(SerialId);
	return data;
}