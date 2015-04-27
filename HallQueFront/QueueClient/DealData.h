#pragma once
#include "SLZData.h"
#include "afxmt.h"
#include <list>
#include "com/callercmd.h"
using namespace std;

class CCallerWnd;

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
	void AddWaitListData(SLZData& data);  // ����ȴ�����
	void AddDoingListData(SLZData& data);
	void AddDoneListData(SLZData& data);
	BOOL HasDoneData();   //�ж���ɶ����Ƿ�������
	BOOL Start();


	CList<SLZData,SLZData&> m_doingDataList;   //ҵ��1�ŶӶ���
	CList<SLZData,SLZData&> m_DoneList;   //��ɶ���
public:
	CMutex m_mtDoingLock;
private:
	CList<SLZData> m_WaitList;  //��ʱ�ȴ�����
private:
	static void CALLBACK MyDoOutTimerMsg(
		HWND hwnd, // handle of window for timer messages
		UINT uMsg, // WM_TIMER message
		UINT idEvent, // timer identifier
		DWORD dwTime // current system time	
		);

	HANDLE m_hSendToNextHandle;
	static DWORD WINAPI SendToNext(LPVOID pParam);

	void FreeWndData();

	void AlarmToCaller(const SLZData* pData);
private:
	CMutex m_mtWaitLock;
	
	CMutex m_mtDonLock;
private:
	CString m_strNextIP;
	int m_iWaitTime;
	list<CCallerWnd*> m_list_callerWnd;
public:
	BOOL ReadWndData();//��callerWnd

	BOOL GetDoingFirstData(CallerCmd* callerCmd,SLZData& data);
	int GetDoingDataCount();
	BOOL ModifyDoingListData( SLZData& data );
	BOOL IsEmptyDoingListDat();
	void RemoveHeadDoingListData();
	BOOL RemoveCallerDoingListData(CallerCmd* callerCmd,SLZData& data);
public:
	CString m_doingSaveDataPath;
	CString m_waitSaveDataPath;
	BOOL SaveDoingData();
	BOOL ReadDoingData();
	BOOL SaveWaitData();
	BOOL ReadWaitData();

private:
		int  m_CallWaitTime;   //���еȴ�ʱ��
};
