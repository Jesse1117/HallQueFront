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
	void AddData(SLZData& data);  // ����ȴ�����
	BOOL HasData();   //�ж���ɶ����Ƿ�������
	BOOL Start();

	CList<SLZData> m_WaitList;  //��ʱ�ȴ�����
	CList<SLZData,SLZData&> m_DataList1;   //ҵ��1�ŶӶ���
	CList<SLZData> m_DataList2;  // ҵ��2 �ŶӶ���	
	CList<SLZData,SLZData&> m_DoneList;   //��ɶ���

	
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
