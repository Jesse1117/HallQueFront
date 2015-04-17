#include "StdAfx.h"
#include "DealData.h"
#include "CallThread.h"
#include "CommonStrMethod.h"
CDealData* dealdata;
CDealData::CDealData(void)
{
	SetTimer(NULL,0,1000,MyDoOutTimerMsg);
	dealdata = this;
	pCallThread = new CCallThread();
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
}

CDealData::~CDealData(void)							
{
}

BOOL CDealData::Start()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Buss1"),NULL,wbuf,255,m_strCallPath);
	CString Buss1(wbuf);
	m_strBuss1 = Buss1;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Buss2"),NULL,wbuf,255,m_strCallPath);
	CString Buss2(wbuf);
	m_strBuss2 = Buss2;

		pCallThread->Start();
		return TRUE;
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
		CTime recvtime = 	data.GetRecvTime();
		if (currenttime-recvtime == CTimeSpan(0,0,data.GetWaitTime(),0))
		{
			if (data.GetBussName()==dealdata->m_strBuss1)
			{
				dealdata->m_DataList1.AddTail(data);
			}
			if (data.GetBussName()==dealdata->m_strBuss2)
			{
				dealdata->m_DataList2.AddTail(data);
			}
			dealdata->m_WaitList.RemoveAt(pos);
		}
	
	}
}
