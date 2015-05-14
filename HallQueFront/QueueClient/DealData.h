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
	void AddWaitListData(SLZData& data);  // 加入等待队列
	void AddDoingListData(SLZData& data);
	void AddDoneListData(SLZData& data);
	void AddCallingListData(SLZData& data);

	BOOL HasDoneData();   //判断完成队列是否有数据
	BOOL Start();


	CList<SLZData,SLZData&> m_doingDataList;   //业务1排队队列
	CList<SLZData,SLZData&> m_DoneList;   //完成队列
	list<SLZData> m_callingList;//正在呼叫队列
public:
	CMutex m_mtDoingLock;
	CMutex m_mtCallingLock;
private:
	CList<SLZData> m_WaitList;  //计时等待队列
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
	BOOL ReadWndData();//读callerWnd

	BOOL GetDoingFirstData(CallerCmd* callerCmd,SLZData& data);
	int GetDoingDataCount();
//	BOOL ModifyDoingListData( SLZData& data );
	BOOL IsEmptyCallingListData();
//	BOOL IsEmptyDoingListDat();
//	void RemoveHeadDoingListData();
//	BOOL RemoveCallerDoingListData(CallerCmd* callerCmd,SLZData& data);
	BOOL RemoveCallingListData(CallerCmd* callerCmd,SLZData& data);
	BOOL ReStartQueue(CallerCmd* callerCmd,SLZData& data);//重排，将呼叫的第一个数据排队到队尾
public:
	CString m_doingSaveDataPath;
	CString m_waitSaveDataPath;
	BOOL SaveDoingData();
	BOOL ReadDoingData();
	BOOL SaveWaitData();
	BOOL ReadWaitData();

private:
		int  m_CallWaitTime;   //呼叫等待时间
};
