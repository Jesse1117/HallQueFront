#include "StdAfx.h"
#include "../HallQueFront/ComplSocketClient.h"
#include "../HallQueFront/DoFile.h"
#include "../HallQueFront/CommonConvert.h"
#include "QueueClient.h"

extern void MyWriteConsole(CString str);

CComplSocketClient::CComplSocketClient(void) : m_romotePort(6000)
, m_nTimeOut(1000)
{
//	m_romoteIP = m_connect.GetRomoteIP();
//	m_romotePort = m_connect.GetRomotePort();
//	m_nTimeOut = m_connect.GetTimeOut();
	CDoFile doFile;
	CString exePath = doFile.GetExeFullFilePath();
	m_caChePath = exePath + _T("\\Cache");
	doFile.CreateMyDirectory(m_caChePath);
	m_caChePath += _T("\\Cache.dat");
	InitClient();
}

CComplSocketClient::~CComplSocketClient(void)
{
	StopClient();
}

void CComplSocketClient::SetRemoteIp(const CString& ip)
{
	m_romoteIP = ip;
}

void CComplSocketClient::SetRemotePort(UINT port)
{
	m_romotePort = port;
}

void CComplSocketClient::SetTimeOut(UINT nMsec)
{
	m_nTimeOut = nMsec;
}

BOOL CComplSocketClient::InitClient()
{
	WSADATA wsaData;
	//��ʼ��
	if(SOCKET_ERROR==WSAStartup(MAKEWORD(2,2),&wsaData))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("WSAStartup error"));
#endif
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

void CComplSocketClient::StopClient()
{
#ifdef _DEBUG
	MyWriteConsole(_T("WSACleanup"));
#endif
	closesocket(m_sClient);
	WSACleanup();
}

BOOL CComplSocketClient::SendData(const CString& msg,std::string& recvMsg,int& actRecvSize)
{

	if(msg.IsEmpty())
	{
		return FALSE;
	}
	BOOL flag = SendData(m_romotePort,m_romoteIP,msg,recvMsg,actRecvSize);

	if(!flag)
	{
		//�������
		DealCache(msg);
	}
	else
	{
		//�ط�����
		ReSendMsg();
	}

	return flag;
}

BOOL CComplSocketClient::SendData(USHORT port,CString IP,const CString& msg,std::string& recvMsg,int& actRecvSize)
{
	int len = CCommonConvert::CStringToChar(msg,NULL);
	char* data = new char[len+1];
	memset(data,0,0);
	CCommonConvert::CStringToChar(msg,data);
	std::string sendData(data);
	delete [] data;
	BOOL flag = SendData(port,IP,sendData,len+1,recvMsg,actRecvSize);
	return flag;
}

BOOL CComplSocketClient::SendData(USHORT port,CString IP,const std::string& msg,int size,std::string& recvMsg,int& actRecvSize)
{
#ifdef _DEBUG
	CString sendMsg = _T("Send Code:");
	CString temp(msg.c_str());
	sendMsg+=temp;
	MyWriteConsole(sendMsg);
#endif
	m_sClient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == m_sClient)
	{
		WSACleanup();
#ifdef _DEBUG
		MyWriteConsole(_T("socket error"));
#endif
		return FALSE;
	}
	SOCKADDR_IN ServerAddr;
	ServerAddr.sin_port = htons(port);
	ServerAddr.sin_family = AF_INET;
	///ת��
	int len = CCommonConvert::CStringToChar(IP,NULL);
	char* buf = new char[len+1]; 
	ZeroMemory(buf,len+1);
	CCommonConvert::CStringToChar(IP,buf);
	////////////////////////////////////////////
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(buf);
	delete []buf;

	if(SOCKET_ERROR==connect(m_sClient,(SOCKADDR*)&ServerAddr,
		sizeof(ServerAddr)))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("connect error"));
#endif
		closesocket(m_sClient);
		return FALSE;
	}

	//����
	setsockopt(m_sClient,SOL_SOCKET,SO_SNDTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
// 	if(SOCKET_ERROR == send(m_sClient,msg.c_str(),size,0))
// 	{
// #ifdef _DEBUG
// 		MyWriteConsole(_T("send failed"));
// #endif
// 		closesocket(m_sClient);
// 		return FALSE;
// 	}
	int actSendSize = 0;
	while(true)
	{
		int tempSize = send(m_sClient,msg.c_str(),size,0);
		if(tempSize == SOCKET_ERROR)
		{
#ifdef _DEBUG
		MyWriteConsole(_T("send failed"));
#endif
			closesocket(m_sClient);
			return FALSE;
		}
		else
		{
			actSendSize += tempSize;
			if(actSendSize == size)break;
		}
	}


	//����
	char recvBuf[MAXRECVBUF+1]={0};
	setsockopt(m_sClient,SOL_SOCKET,SO_RCVTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
	while(true)
	{
 		memset(recvBuf,0,MAXRECVBUF+1);
		actRecvSize = recv(m_sClient,recvBuf,MAXRECVBUF,0);
		std::string recvTemp(recvBuf);
#ifdef _DEBUG
		CString tremp(recvBuf);
		MyWriteConsole(tremp);
#endif
		if(recvTemp.find("</dataPacket>") != recvTemp.npos)//�ҽ�β
		{
			recvMsg += recvTemp;
			break;
		}
		if(actRecvSize==SOCKET_ERROR || actRecvSize==0)
		{
			break; 	
		}
		else
		{
			recvMsg += recvTemp;
		}
	}


	closesocket(m_sClient);
	return TRUE;
}

void CComplSocketClient::DealCache(const CString& msg)
{
	if(msg.IsEmpty() || IsTheSameMsg(msg))return;
	theApp.m_list_caCheMsg.push_back(msg);
#ifdef _DEBUG
	CString str=_T("cache size:");
	str.AppendFormat(_T("%d"),theApp.m_list_caCheMsg.size());
	MyWriteConsole(str);
#endif
	if(theApp.m_list_caCheMsg.size()>=10)
	{
		CFile file;
		if(file.Open(m_caChePath,CFile::modeCreate | CFile::modeNoTruncate |CFile::modeWrite))
		{
			std::list<CString>::const_iterator itera = theApp.m_list_caCheMsg.begin();
			for(itera;itera!=theApp.m_list_caCheMsg.end();itera++)
			{
				file.SeekToEnd();
				CString temp = *itera;
				char tempBuf[MAXRECVBUF] ={0};
				CCommonConvert::CStringToChar(temp,tempBuf);
				file.Write(tempBuf,MAXRECVBUF);
			}
			theApp.m_list_caCheMsg.clear();//�������
			file.Close();
		}
	}
}

void CComplSocketClient::ReSendMsg()
{
	AppendListMsg();//���ļ�����û���ͳɹ�������
	///�ļ����
	CFile clearFile;
	if(clearFile.Open(m_caChePath,CFile::modeWrite))
	{
		clearFile.SetLength(0);
		clearFile.Close();
	}
	/////////////////
	if(theApp.m_list_caCheMsg.size()==0)return;
	std::list<CString>::const_iterator itera = theApp.m_list_caCheMsg.begin();
	for(itera;itera!=theApp.m_list_caCheMsg.end();itera++)
	{
		CString msg = *itera;
		std::string recvMsg;
		int actRecvSize = 0;
		BOOL flag = SendData(m_romotePort,m_romoteIP,msg,recvMsg,actRecvSize);
		if(!flag)//�ط�ʱ���Ͳ��ɹ�
		{
			//ֱ��д�ļ�
			CFile file;
			if(file.Open(m_caChePath,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
			{
				file.SeekToEnd();
				CString temp = *itera;
				char tempBuf[MAXRECVBUF] ={0};
				CCommonConvert::CStringToChar(temp,tempBuf);
				file.Write(tempBuf,MAXRECVBUF);
				file.Close();
			}
		}
	}
	theApp.m_list_caCheMsg.clear();
}

BOOL CComplSocketClient::AppendListMsg()
{
	CFile file;
	if(file.Open(m_caChePath,CFile::modeRead))
	{
		DWORD length = (DWORD)file.GetLength()/MAXRECVBUF;
		for(DWORD i=0;i<length;i++)
		{
			char msg[MAXRECVBUF] = {0};
			if(file.Read(msg,MAXRECVBUF))
			{
				CString wStrMsg;
				CCommonConvert::CharToCstring(wStrMsg,msg);
				if(!IsTheSameMsg(wStrMsg))
					theApp.m_list_caCheMsg.push_back(wStrMsg);
			}
		}
		file.Close();
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CComplSocketClient::IsTheSameMsg(const CString& msg)
{
	list<CString>::const_iterator itera = theApp.m_list_caCheMsg.begin();
	for(itera;itera != theApp.m_list_caCheMsg.end(); ++itera)
	{
		if(*itera == msg)
		{
			return TRUE;
		}
	}
	return FALSE;
}