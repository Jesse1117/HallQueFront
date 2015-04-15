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
	void AddData(SLZData& data);  // �����ŶӶ���
	BOOL HasData();   //�ж���ɶ����Ƿ�������
	BOOL Start();
private:
	CList<SLZData> m_WaitList;  //��ʱ�ȴ�����
	CList<SLZData,SLZData&> m_DataList;   //�ŶӶ���
	CList<SLZData,SLZData&> m_DoneList;   //��ɶ���
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
