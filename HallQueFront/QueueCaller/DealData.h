#pragma once
#include "SLZData.h"
class CDealData
{
private:
	CDealData(void);
public:
	static CDealData* GetInstance()
	{
		CDealData* data = new CDealData();
		return data;
	}
	~CDealData(void);
	void AddData(SLZData& data);  // 加入排队队列
	BOOL HasData();   //判断完成队列是否有数据
	BOOL Start();
private:
	CList<SLZData> m_WaitList;  //计时等待队列
	CList<SLZData,SLZData&> m_DataList;   //排队队列
	CList<SLZData,SLZData&> m_DoneList;   //完成队列
	HANDLE m_hCallerHandle;
	static DWORD WINAPI CallThread(LPVOID lpParam);
	HANDLE m_CallThreadHandle;
	static void CALLBACK MyDoOutTimerMsg(
		HWND hwnd, // handle of window for timer messages
		UINT uMsg, // WM_TIMER message
		UINT idEvent, // timer identifier
		DWORD dwTime // current system time	
		);
};
