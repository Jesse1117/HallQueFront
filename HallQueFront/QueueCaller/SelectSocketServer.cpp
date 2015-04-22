#include "StdAfx.h"
#include "SelectSocketServer.h"
#include "..\HallQueFront\DoFile.h"
#include "../HallQueFront/WriteLogError.h"
#include "DealPacket.h"
#include "DealData.h"
#include "SLZData.h"
#include "QueueCaller.h"

CSelectSocketServer* pServer;
extern void MyWriteConsole(CString str);
CSelectSocketServer::CSelectSocketServer(void)
: m_iTotalConn(0)
,m_hAcceptThreadHandle(INVALID_HANDLE_VALUE)
,m_hWorhThreadHandle(INVALID_HANDLE_VALUE)
,m_uListenPort(6000)
{
	m_pDealData = CDealData::GetInstance();
	pServer = this;
}

CSelectSocketServer::~CSelectSocketServer(void)
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
	WSACleanup(); 
//	SaveListenPort();
}

BOOL CSelectSocketServer::InitServer()
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
	//�����������߳�
	m_hWorhThreadHandle = CreateThread(NULL,0,WorkerThread,this,0,NULL);
	// �����µ�����
	m_hAcceptThreadHandle = CreateThread(NULL,0,AcceptThread,this,0,NULL);
	return TRUE;
}

// USHORT CSelectSocketServer::GetListenPort()
// {
// 	CDbaConfig m_dbaConfig;
// 	CString port = m_dbaConfig.GetServerPort();
// 	int  iPort = 0;
// 	m_convert.CStringToint(iPort,port);
// 	return iPort;
// }

// BOOL CSelectSocketServer::SaveListenPort()
// {
// 	CString strPath=m_convert.GetExeFullFilePath();
// 	CDoFile dofile;
// 	CString port;
// 	m_convert.intToCString(m_uListenPort,port);
// 	strPath+=_T("\\Port");
// 	dofile.CreateMyDirectory(strPath);
// 	strPath+=_T("\\ServerPort.ini");
// 	return WritePrivateProfileString(_T("port"),_T("PORT"),port,strPath);
// }

DWORD WINAPI CSelectSocketServer::AcceptThread(LPVOID lpParam)
{
	CSelectSocketServer* pThis = (CSelectSocketServer*)lpParam;
	while(1)
	{
		SOCKET NewConnection;
		SOCKADDR_IN clientAddr;
		int len = sizeof(clientAddr);
		NewConnection = WSAAccept(pThis->m_ListeningSocket, (sockaddr*)&clientAddr, &len, ConditionFunc, 0);
		if(NewConnection != SOCKET_ERROR)
		{
			pThis->m_muServerLock.Lock();
			pThis->m_CliSocketArr[pThis->m_iTotalConn++] = NewConnection;
#ifdef _DEBUG
			CString ipAddress;
			pThis->m_convert.CharToCstring(ipAddress,
				inet_ntoa(clientAddr.sin_addr));
			CString clientInfo;
			clientInfo.Format(_T("Client <%s : %d> come in..."),ipAddress,
				ntohs(clientAddr.sin_port));
			MyWriteConsole(clientInfo); 
#endif
// 			CWriteLogError log;
// 			CString str;
// 			str.Format(_T("accept succ client num:%d"),pThis->m_iTotalConn);
// 			log.WriteErrLog(str);
			pThis->m_muServerLock.Unlock();
		}
		else
		{
#ifdef _DEBUG
			MyWriteConsole(_T("accpet error"));
#endif
			CWriteLogError log;
			CString str;
			str.Format(_T("accept error client num:%d"),pThis->m_iTotalConn);
			log.WriteErrLog(str);
		}
	}
	return 0;
}

DWORD WINAPI CSelectSocketServer::WorkerThread(LPVOID lpParam)
{
	CSelectSocketServer* pThis = (CSelectSocketServer*)lpParam;
	fd_set fdread;
	int ret;
	struct timeval tv = {1, 0};
	char szMessage[MAX_BUFFER+1]={0};
	while(TRUE)
	{
		memset(szMessage,0,MAX_BUFFER+1);
		FD_ZERO(&fdread);//��ʼ��
		if(pThis->m_iTotalConn==0)
		{
			Sleep(20);
		}
		else
		{
			for(int i=0;i<pThis->m_iTotalConn;i++)
			{
				FD_SET(pThis->m_CliSocketArr[i], &fdread);//���µ����Ӽ��뼯��fdread
			}
		//ȷ��һ�������׽��ֵ�״̬���ȴ������Ҫ�Ļ���ִ��ͬ��I/O
			ret = select(0,&fdread,NULL,NULL,&tv);
			if (ret == 0)
			{
				// Time expired
				continue;
			}

			for(int i=0;i<pThis->m_iTotalConn;i++)
			{
				/*�ڵ���select()��������FD_ISSET�����fd��fdset����
				�е�״̬�Ƿ�仯�������ͣ�����⵽fd״̬�����仯ʱ�����棬
				���򣬷��ؼ٣�0��*/
				if(FD_ISSET(pThis->m_CliSocketArr[i],&fdread))//���
				{
// #ifdef _DEBUG
// 					CComputeFuncationTime time;
// 					time.SetStartTime(clock());
// #endif
					//����
					ret = recv(pThis->m_CliSocketArr[i], szMessage, MAX_BUFFER, 0);
					if (ret == 0 || (ret == SOCKET_ERROR))
					{
						pThis->DeleteClient(i);
					}
					else
					{
						//���յ��Ժ���
#ifdef _DEBUG
						CString msg;
						pThis->m_convert.CharToCstring(msg,szMessage);
						MyWriteConsole(_T("���ϼ����յ���Ϣ:") + msg);
#endif
						std::string recvPacket(szMessage);
						std::string retPacket = pThis->DealMsg(recvPacket);
#ifdef _DEBUG
						CString wRetPacket(retPacket.c_str());
						MyWriteConsole(_T("������Ϣ:") + wRetPacket);
#endif
						int size = retPacket.size();
						int nTimeOut=1000;
						int actSendSize = 0;
						setsockopt(pThis->m_CliSocketArr[i],SOL_SOCKET,SO_RCVTIMEO,(char *)&nTimeOut,sizeof(UINT));
						while(true)
						{
							int tempSize = send(pThis->m_CliSocketArr[i], retPacket.c_str(), size, 0);
							if(tempSize == SOCKET_ERROR)
							{
								pThis->DeleteClient(i);
								CWriteLogError log;
								CString str;
								str.Format(_T("send errcode:%d client num:%d"),WSAGetLastError(),i);
								log.WriteErrLog(str);
								break;
							}
							else
							{
								actSendSize += tempSize;
								if(actSendSize == size)break;
							}
						}
					}
// #ifdef _DEBUG
// 					time.SetFinshTime(clock());
// 					double dura = time.GetDuration();
// 					CString strDuration;
// 					strDuration.Format(_T("dealtime:%f"),dura);
// 					MyWriteConsole(strDuration);
// #endif
				}
				else
				{
					pThis->DeleteClient(i);
					CWriteLogError log;
					CString str;
					str.Format(_T("FISSET errcode:%d client num:%d"),WSAGetLastError(),i);
					log.WriteErrLog(str);
				}
			}
		}
	}	
	return 0;
}

std::string CSelectSocketServer::DealMsg(const std::string recvPacket)
{
	SLZData data;
	BOOL flag = CDealPacket::AnaSendPacket(recvPacket,&data);//�������͹���������
	CDealData* pDealData = CDealData::GetInstance();
	pDealData->AddData(data);//���뵽�ȴ�����
	///���ɷ�������
	return CDealPacket::ProduceRetPacket(flag);
}

void CSelectSocketServer::DeleteClient(int i)
{
	closesocket(m_CliSocketArr[i]);
#ifdef _DEBUG
	CString str;
	str.Format(_T("%d client"),m_iTotalConn);
	MyWriteConsole(str);
#endif
	if(i == m_iTotalConn-1)//ɾ�����һ��
	{
		m_iTotalConn--;
		m_CliSocketArr[i]=0;
	}
	else if(i < m_iTotalConn-1)      
	{     
		m_CliSocketArr[i--] = m_CliSocketArr[--m_iTotalConn];     
	}
}

int CALLBACK CSelectSocketServer::ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData) 
{  
	if (pServer->m_iTotalConn < FD_SETSIZE)  
		return CF_ACCEPT;  
	else  
		return CF_REJECT;  
}  