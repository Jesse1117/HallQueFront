#pragma once
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")      // Socket������õĶ�̬���ӿ�
//#include "DealData.h"
//#include "ProducePacket.h"
#include <string>
#include "..\HallQueFront\CommonConvert.h"
#include "afxmt.h"
using namespace std;
#define MAX_BUFFER  4*1024

class CSelectSocketServer
{
public:
	CSelectSocketServer(void);
	~CSelectSocketServer(void);
private:
	static DWORD WINAPI WorkerThread(LPVOID lpParam);
	static DWORD WINAPI AcceptThread(LPVOID lpParam);
private:
	int    m_iTotalConn;//������
	SOCKET m_CliSocketArr[FD_SETSIZE];//Socket����
	SOCKET m_ListeningSocket;//����socket
	UINT m_uListenPort;
	HANDLE m_hAcceptThreadHandle;
	HANDLE m_hWorhThreadHandle;
	/*CDealData* m_pDealData;*/
	//CProducePacket m_producePacket;
	CCommonConvert m_convert;
public:
	BOOL InitServer();
	//USHORT GetListenPort();
	//BOOL SaveListenPort();
private:
	//std::string DealMsg(const std::string recvPacket);
	CMutex m_muServerLock;
	void DeleteClient(int i);
	static int CALLBACK ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData);  
};
