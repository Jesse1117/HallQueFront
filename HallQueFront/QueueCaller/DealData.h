#pragma once
#include "SLZData.h"
#include "afxmt.h"

class CCallThread;
class CDealData
{
private:
	CDealData(void);
public:
	static CDealData* GetInstance()
	{
		static CDealData Instance;
		return &Instance;
	}
	~CDealData(void);
	void AddData(SLZData& data);  // ����ȴ�����
	BOOL HasData();   //�ж���ɶ����Ƿ�������
	BOOL Start();


	CList<SLZData,SLZData&> m_DataList1;   //ҵ��1�ŶӶ���
	CList<SLZData,SLZData&> m_DoneList;   //��ɶ���
private:
	CList<SLZData> m_WaitList;  //��ʱ�ȴ�����
	CList<SLZData> m_DataList2;  // ҵ��2 �ŶӶ���	
	

	
private:
	static void CALLBACK MyDoOutTimerMsg(
		HWND hwnd, // handle of window for timer messages
		UINT uMsg, // WM_TIMER message
		UINT idEvent, // timer identifier
		DWORD dwTime // current system time	
		);

	HANDLE m_hSendToNextHandle;
	static DWORD WINAPI SendToNext(LPVOID pParam);
private:
	CCallThread* m_pCallThread;

	CString m_strBuss1;
	CString m_strBuss2;
	CString m_strCallPath;
	int callerid1;
	int callerid2;
private:
	CMutex m_mtWaitLock;
	CMutex m_mtList1Lock;
	CMutex m_mtList2Lock;
	CMutex m_mtDonLock;
private:
	CString m_strNextIP;
	CString m_strNextPort;
	int m_iWaitTime;
};
