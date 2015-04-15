#include "StdAfx.h"
#include "DealData.h"
CDealData* dealdata;
CDealData::CDealData(void)
{
}

CDealData::~CDealData(void)
{
	if(m_CallThreadHandle!=INVALID_HANDLE_VALUE)
	{
		TerminateThread(m_CallThreadHandle,0);
		CloseHandle(m_CallThreadHandle);
	}
}

BOOL CDealData::Start()
{
	m_CallThreadHandle = CreateThread(NULL,0,CallThread,this,0,NULL);
	if (m_CallThreadHandle!=NULL)
	{
		SetTimer(NULL,0,1000,MyDoOutTimerMsg);
		return TRUE;
	}
	else return FALSE;
}

BOOL CDealData::HasData()
{
	if (m_DoneList.GetCount()>0)
	{
		return TRUE;
	}
	else return FALSE;
}

void CDealData::AddData(SLZData& data)
{
	m_WaitList.AddTail(data);
}

DWORD WINAPI CDealData::CallThread(LPVOID lpParam)
{
	while (TRUE)
	{
			
	}
	return 0;
}

void CALLBACK CDealData::MyDoOutTimerMsg(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	POSITION pos;
	int count = dealdata->m_WaitList.GetCount();
	for (int i=0;i<count;i++)
	{
		SLZData data;
		pos = dealdata->m_WaitList.FindIndex(i);
		if (!pos)break;
		data = dealdata->m_WaitList.GetAt(pos);
		CTime currenttime = CTime::GetCurrentTime();
		data.GetRecvTime();
	}
}
