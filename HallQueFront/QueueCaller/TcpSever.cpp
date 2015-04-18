#include "StdAfx.h"
#include "TcpSever.h"
#include "CommonStrMethod.h"
#include "DealData.h"
#include "TCPConnect.h"
#include "CreatePacket.h"
#include "CommonConvert.h"
CTcpSever::CTcpSever(void):m_iWaitTime(0)
{
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
	m_strLogFilePath = CommonStrMethod::GetModuleDir() + _T("log/");
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
	LoadConfig();
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

void CTcpSever::LoadConfig()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("IP"),NULL,wbuf,255,m_strCallPath);
	CString strIP(wbuf);
	m_strIP = strIP;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Port"),NULL,wbuf,255,m_strCallPath);
	CString strPort(wbuf);
	m_strPort = strPort;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("WaitTime"),NULL,wbuf,255,m_strCallPath);
	CString strWaitTime(wbuf);
	m_iWaitTime = CommonStrMethod::Str2Int(strWaitTime);
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
	CTcpSever* Server = (CTcpSever*) lpParam;
	while(TRUE)
	{
		if (CDealData::GetInstance()->HasData())
		{
			Server->m_ServerLock.Lock();
			SLZData data = CDealData::GetInstance()->m_DoneList.GetHead();
			Server->m_ServerLock.Unlock();
			data.SetWaitTime(Server->m_iWaitTime);
			CCreatePacket packet;
			CString strSend = packet.ProducePacket(data);
			CTCPConnect connect;
			if (connect.SendPackage(strSend,Server->m_strIP,Server->m_strPort,0))
			{
				Server->m_ServerLock.Lock();
				SLZData data = CDealData::GetInstance()->m_DoneList.RemoveHead();
				Server->m_ServerLock.Unlock();
			}			
		}
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
					CString strRecv;
					CCommonConvert::CharToCstring(strRecv,buf);
					Server->WriteLogWithTime(_T("recv:")+strRecv);
					SLZData data = Server->Dodata(buf);
					Server->m_ServerLock.Lock();
					CDealData::GetInstance()->AddData(data);
					Server->m_ServerLock.Unlock();
					char returnbuf[2]={0};
					returnbuf[1] = '1';
					int iRet = ::send(NewConnection,returnbuf,sizeof(returnbuf),0);
					if (iRet==SOCKET_ERROR)
					{
						Server->WriteLogWithTime(_T("send error"));
					}
					else{Server->WriteLogWithTime(_T("send successs"));}
				}
			}
		}
	}
	return 0;
}

SLZData CTcpSever::Dodata(std::string buf)
{
	SLZData data;
	std::string::size_type firstIndex = buf.find("<SerialId>");
	std::string::size_type lastIndex = buf.find("</SerialId>");
	std::string SerialId = buf.substr(firstIndex+10,lastIndex-firstIndex-10);
	CString strSerialId(SerialId.c_str());
	data.SetSerialId(strSerialId);
	firstIndex = buf.find("<BussName>");
	lastIndex = buf.find("</BussName>");
	std::string BussName = buf.substr(firstIndex+10,lastIndex-firstIndex-10);
	CString strBussName(BussName.c_str());
	data.SetBussName(strBussName);
	firstIndex = buf.find("<QueNum>");
	lastIndex = buf.find("</QueNum>");
	std::string QueNum = buf.substr(firstIndex+8,lastIndex-firstIndex-8);
	CString strQueNumber(QueNum.c_str());
	data.SetQueueNumber(strQueNumber);
	firstIndex = buf.find("<CardNum>");
	lastIndex = buf.find("</CardNum>");
	std::string CardNum = buf.substr(firstIndex+9,lastIndex-firstIndex-9);
	CString strCardNum(CardNum.c_str());
	data.SetCardNumber(strCardNum);
	firstIndex = buf.find("<CustName>");
	lastIndex = buf.find("</CustName>");
	std::string CustName = buf.substr(firstIndex+10,lastIndex-firstIndex-10);
	CString strCustName(CustName.c_str());
	data.SetCustName(strCustName);
	firstIndex = buf.find("<WaitTime>");
	lastIndex = buf.find("</WaitTime>");
	std::string WaitTime = buf.substr(firstIndex+10,lastIndex-firstIndex-10);
	int waittime = ::atoi(WaitTime.c_str());
	data.SetWaitTime(waittime);
	firstIndex = buf.find("<CustPhoneNum>");
	lastIndex = buf.find("</CustPhoneNum>");
	std::string PhoneNum = buf.substr(firstIndex+14,lastIndex-firstIndex-14);
	CString strPhoneNum(PhoneNum.c_str());
	data.SetPhoneNum(strPhoneNum);
	firstIndex = buf.find("<ShortMsg>");
	lastIndex = buf.find("</ShortMsg>");
	std::string ShortMsg = buf.substr(firstIndex+10,lastIndex-firstIndex-10);
	CString strShortMsg(ShortMsg.c_str());
	data.SetSendMsg(strShortMsg);
	data.SetRecvTime(CTime::GetCurrentTime());
	return data;
}

void CTcpSever::WriteErrLog(CString strSockLog)
{
	CString str = _T("Error: ") + strSockLog;
	WriteLogWithTime(str);
}

void CTcpSever::WriteLogWithTime(CString strSockLog)
{
	CTime time = CTime::GetCurrentTime();
	CString str = time.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("  \t") + strSockLog + _T("\r\n\r\n");
	CString strLogFile = time.Format(_T("log_%Y%m%d.log"));
	if(!CommonStrMethod::PathFileExist(m_strLogFilePath))
	{
		if(!CommonStrMethod::CreatePath(m_strLogFilePath))
		{
			return;
		}
	}
	AppendWriteFile(str, m_strLogFilePath + strLogFile);
}

BOOL CTcpSever::AppendWriteFile(CString strText, CString strFileName)
{
	CFile file;
	if(!file.Open(strFileName, 
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		//CString str;
		//str.Format(_T("创建或打开日志文件失败,您所使用的计算机帐号没有相应的磁盘写权限:\r\n\t%s"), g_pControl->m_strLogFilePath);
		//AfxMessageBox(str);
		return FALSE;
	}
	CHAR* szBuf = (CHAR*)malloc(strText.GetLength()*2 + 1);
	memset(szBuf, 0, strText.GetLength()*2 + 1);
	CommonStrMethod::WChar2Char(szBuf, 
		strText.GetLength()*2 + 1, strText.GetBuffer());
	file.SeekToEnd();
	file.Write(szBuf, strlen(szBuf));
	file.Close();

	return TRUE;
}