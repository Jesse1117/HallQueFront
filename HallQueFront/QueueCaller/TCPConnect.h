#pragma once
#ifndef _MY_MAX_IP_LENGTH
#define _MY_MAX_IP_LENGTH	64
#endif
#include "afxwin.h"
#include "WinSock2.h"


struct SocketPackage
{
	char PackageLen[8];   // ���ĳ���
	char Request[4];	   // ���״���
	char Response[4];	   // ��Ӧ����
	char Reserved[8];	   // ����ʹ��
	char XMLBuf[231];	// ����
};

class CTCPConnect
{
public:
	CTCPConnect(void);
	virtual ~CTCPConnect(void);

	int GetCustLevel(CString StrCardNum,CString strIP,CString strPort,UINT iOverTime);  //���뿨�� �Խӵķ�����IP�Ͷ˿ڳ�ʱʱ�� �õ��ͻ��ȼ�
	BOOL SendPackage(const CString strSendPackage,CString strIP,CString strPort,UINT iOverTime);
private:
	int GetLastError();
	void WriteErrLog(CString strErr);
	void WriteLogWithTime(CString strSockLog);			//д�������־
	BOOL AppendWriteFile(CString strText, CString strFileName);
	SocketPackage MakePackage(char c_CardNum[]);		//����socket��
	int  GetLevel(char c_Request[]);
	BOOL StartSocket(CString strPort,CString strIP);
	BOOL CloseSocket();
	BOOL SendTo(CHAR * pData, UINT nLen);
	BOOL RecvFrom(CHAR * buf, int iLen);

	BOOL Startup();
	BOOL CreateSocket();
	BOOL _Connect();
	BOOL _CloseSocket();
	BOOL Cleanup();
	USHORT m_uPort;
	CString m_strIP;
	SOCKET m_tSocket;
	BOOL m_bIsCreate;
	CString m_strLogFilePath;
};
