#include "StdAfx.h"
#include "TCPConnect.h"
#include "CommonConvert.h"
#include "CommonStrMethod.h"
#include <stdio.h>

CTCPConnect::CTCPConnect(void)
: m_uPort(0)
, m_bIsCreate(FALSE)
{
	m_strLogFilePath = CommonStrMethod::GetModuleDir() + _T("log/");
}

CTCPConnect::~CTCPConnect(void)
{
}

int CTCPConnect::GetCustLevel(CString StrCardNum,CString strIP,CString strPort,UINT iOverTime)
{
	if (StartSocket(strPort,strIP))
	{
		char cCardNum[30];
		CCommonConvert::CStringToChar(StrCardNum,cCardNum);
		SocketPackage package = MakePackage(cCardNum);
		int size = sizeof(package);
		SendTo((char*)&package,sizeof(package));
		CString CardNum = _T("cardnum:")+StrCardNum;
		WriteLogWithTime(CardNum);
		char CardRecv[255];
		BOOL RecvTemp = FALSE;
		setsockopt(m_tSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&iOverTime,sizeof(UINT));
		if (!RecvFrom(CardRecv,255))
		{
			return -1;
		}
		int iLevel = GetLevel(CardRecv);
		
		//RecvFrom()
		CloseSocket();
		return iLevel;
	}
	else	
	{
		CString strErr = _T("socket����ʧ��");
		return -1;
	}
	
}

BOOL CTCPConnect::SendPackage(const CString strSendPackage,CString strIP,CString strPort,UINT iOverTime)
{
	if (StartSocket(strPort,strIP))
	{
		int size = strSendPackage.GetLength();
		char* cSendPackage = new char[size+1];
		CCommonConvert::CStringToChar(strSendPackage,cSendPackage);
		size = sizeof(cSendPackage);
		SendTo(cSendPackage,size);
		char CardRecv[255];
		BOOL RecvTemp = FALSE;
		if (iOverTime!=0)
		{
			setsockopt(m_tSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&iOverTime,sizeof(UINT));
		}
		if (!RecvFrom(CardRecv,255))
		{
			CloseSocket();
			return FALSE;
		}
		CloseSocket();
	}
	return TRUE;
}

BOOL CTCPConnect::StartSocket(CString strPort,CString strIP)
{
	int port;
	CCommonConvert::CStringToint(port,strPort);
	m_uPort = port;
	m_strIP = strIP;
	CString strErr;
	if(m_bIsCreate)
	{
		return FALSE;
	}
	if(!Startup())
	{
		strErr = _T("��ʼ��Socketʧ��");
		WriteErrLog(strErr);
		//ShowSocketError();
		return FALSE;
	}

	if(!CreateSocket())
	{
		strErr = _T("����Socket�׽���ʧ��");
		WriteErrLog(strErr);
		return FALSE;
	}

	if(!_Connect())
	{
		CString str;
		str = (_T("���ӷ�����ʧ��,ȷ��IP��ַ�˿�����"));
		WriteErrLog(str);
		CloseSocket();
		return FALSE;
	}

	m_bIsCreate = TRUE;

	return TRUE;
}

BOOL CTCPConnect::Startup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1,1);
	err = WSAStartup(wVersionRequested,&wsaData);
	if (err!=0)
	{
		return FALSE;
	}
	if (LOBYTE(wsaData.wVersion)!=1||HIBYTE(wsaData.wVersion)!=1)
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL CTCPConnect::CreateSocket()
{
	m_tSocket = socket(AF_INET,SOCK_STREAM,0);
	if(m_tSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CTCPConnect::_Connect()
{
	if (!m_strIP.IsEmpty())
	{
		CHAR szIP[_MY_MAX_IP_LENGTH] = {0};
		CCommonConvert::WChar2Char(szIP,_MY_MAX_IP_LENGTH,m_strIP);
		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr = inet_addr(szIP);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(m_uPort);
		int iresult = connect(m_tSocket,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
		if (iresult!=0)
		{
			int ierr = GetLastError();
			CString strErr;
			CCommonConvert::intToCString(ierr,strErr);
			WriteErrLog(strErr);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CTCPConnect::CloseSocket()
{
	if(!m_bIsCreate)
	{
		return FALSE;
	}

	if(!_CloseSocket())
	{
		return FALSE;
	}
	if(!Cleanup())
	{
		return FALSE;
	}
	m_bIsCreate = FALSE;

	return TRUE;
}

BOOL CTCPConnect::SendTo( CHAR * pData, UINT nLen)
{
	int iRet = ::send(m_tSocket,pData,nLen,0);
	if(iRet == SOCKET_ERROR)
	{
		int ierr = GetLastError();
		CString strErr;
		CCommonConvert::intToCString(ierr,strErr);
		WriteErrLog(strErr);
		return FALSE;
	}
	return TRUE;
}

BOOL CTCPConnect::RecvFrom(CHAR * buf, int iLen)
{
	if (SOCKET_ERROR == ::recv(m_tSocket,buf,iLen,0))
	{
		int ierr = GetLastError();
		CString strErr;
		CCommonConvert::intToCString(ierr,strErr);
		WriteErrLog(strErr);
		return FALSE;
	}
	return TRUE;
}

BOOL CTCPConnect::_CloseSocket()
{
	if(::closesocket(m_tSocket) != SOCKET_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CTCPConnect::Cleanup()
{
	if(::WSACleanup() != SOCKET_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

SocketPackage CTCPConnect::MakePackage(char c_CardNum[])
{
	SocketPackage package ={0};
	int length = sprintf_s(package.XMLBuf,223,"<?xml version=\"1.0\" encoding=\"UTF-8\"?><vip><cutomerinfo>%s</cutomerinfo></vip>",c_CardNum);
	length+=24;
	char digitwithnull[9];
	sprintf_s(digitwithnull,9,"%.8d",length);
	memcpy(package.PackageLen,digitwithnull,8);
	package.Request[0] = '0';
	package.Request[1] = '0';
	package.Request[2] = '0';
	package.Request[3] = '4';
	memset(package.Response,'0',4);
	memset(package.Reserved,' ',8);
	return package;
}

int CTCPConnect::GetLevel(char c_Request[])
{
	if (c_Request[15]=='0')
	{
		int iLevel = c_Request[74]-48;
		return iLevel;
	}
	else
	{
		CString strErr ;
		switch (c_Request[15])
		{
		case '1':
			strErr = _T("��ͷ��ʽ����");
			WriteErrLog(strErr);
			break;
		case '2':
			strErr = _T("����XML��ʽ����");
			WriteErrLog(strErr);
			break;
		case '3':
			strErr = _T("����ʶ���벻����");
			WriteErrLog(strErr);
			break;
		case '4':
			strErr = _T("�û��������벻����");
			WriteErrLog(strErr);
			break;
		case '5':
			strErr = _T("������");
			WriteErrLog(strErr);
			break;
		case '6':
			strErr = _T("����Զ�̷���ʧ��");
			WriteErrLog(strErr);
			break;
		case '7':
			strErr = _T("Զ�̷�����Ӧ��ʱ");
			WriteErrLog(strErr);
			break;
		case '8':
			strErr = _T("��������");
			WriteErrLog(strErr);
			break;
		}
		/*CString strErr = _T("���ر��ĸ�ʽ����");
		WriteErrLog(strErr);*/
		return -1;
	}
}

int CTCPConnect::GetLastError()
{
	return ::WSAGetLastError();
}

void CTCPConnect::WriteErrLog(CString strSockLog)
{
	CString str = _T("Error: ") + strSockLog;
	WriteLogWithTime(str);
}

void CTCPConnect::WriteLogWithTime(CString strSockLog)
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

BOOL CTCPConnect::AppendWriteFile(CString strText, CString strFileName)
{
	CFile file;
	if(!file.Open(strFileName, 
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		//CString str;
		//str.Format(_T("���������־�ļ�ʧ��,����ʹ�õļ�����ʺ�û����Ӧ�Ĵ���дȨ��:\r\n\t%s"), g_pControl->m_strLogFilePath);
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