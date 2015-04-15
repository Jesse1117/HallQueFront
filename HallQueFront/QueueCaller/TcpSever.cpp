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
	SOCKADDR_IN addr_Server;//��ַ����
	CString errcode;
	//winsock��ʼ��
	int iError = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(iError != 0)
	{
		errcode.Format(_T("WSAStartup failed with error: %d"),GetLastError);
		AfxMessageBox(errcode);
		return FALSE;
	}
	//����һ���׽��ְ󶨵�һ���ض���Э�� 
	//	m_SockBound = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	m_ListeningSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_ListeningSocket == INVALID_SOCKET)  
	{  
		errcode.Format(_T("WSAStartup failed with error: %d"),GetLastError);
		AfxMessageBox(errcode);
		WSACleanup();  //terminates use of the Winsock 2 DLL 
		return FALSE;  
	}  
	//�������ص�ַ��Ϣ  
	addr_Server.sin_family = AF_INET; //��ַ����  
	/*USHORT port = GetListenPort();*/
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("SelfPort"),NULL,wbuf,255,m_strCallPath);
	int port = CommonStrMethod::Str2Int(wbuf);
	if(port > 0)
	{
		m_uListenPort = port;
	}
	//ע��ת��Ϊ�����ֽ���
	addr_Server.sin_port = htons(m_uListenPort);  
	//ʹ��INADDR_ANY ָʾ�����ַ 
	addr_Server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//��
	iError = bind(m_ListeningSocket,(LPSOCKADDR)&addr_Server,
		sizeof(addr_Server));
	if(iError == SOCKET_ERROR)
	{
		errcode.Format(_T("socket failed with error code: %d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_ListeningSocket); //�ر��׽���  
		WSACleanup();  
		return FALSE;
	}
	//������������  
	iError = listen(m_ListeningSocket, 5);  
	if (iError == SOCKET_ERROR)  
	{    
		errcode.Format(_T("socket failed with error code:%d"),GetLastError());
		AfxMessageBox(errcode,MB_OK | MB_ICONERROR);
		closesocket(m_ListeningSocket); //�ر��׽���  
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
	data.SetPhoneNum(strShortMsg);
	data.SetRecvTime(CTime::GetCurrentTime());
	return data;
}