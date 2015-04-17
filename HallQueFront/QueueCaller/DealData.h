#pragma once
#include "SLZData.h"
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
	void AddData(SLZData& data);  // 加入等待队列
	BOOL HasData();   //判断完成队列是否有数据
	BOOL Start();

	CList<SLZData> m_WaitList;  //计时等待队列
	CList<SLZData,SLZData&> m_DataList1;   //业务1排队队列
	CList<SLZData> m_DataList2;  // 业务2 排队队列	
	CList<SLZData,SLZData&> m_DoneList;   //完成队列

	
private:
	HANDLE m_CallThreadHandle;
	static void CALLBACK MyDoOutTimerMsg(
		HWND hwnd, // handle of window for timer messages
		UINT uMsg, // WM_TIMER message
		UINT idEvent, // timer identifier
		DWORD dwTime // current system time	
		);

	CCallThread* pCallThread;

	CString m_strBuss1;
	CString m_strBuss2;
	CString m_strCallPath;

};
