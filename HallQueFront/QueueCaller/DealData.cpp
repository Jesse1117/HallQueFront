#include "StdAfx.h"
#include "DealData.h"
#include "CallThread.h"
#include "CommonStrMethod.h"
#include "SLZCCaller.h"
#include "QueueCaller.h"
#include "ComplSocketClient.h"
#include "ComplSocketClient.h"
#include "DealPacket.h"
#include "CommonConvert.h"

CDealData* dealdata;
extern void MyWriteConsole(CString str);

CDealData::CDealData(void):callerid1(0)
,callerid2(0)
,m_iWaitTime(0)
,m_pCallThread(NULL)
,m_hSendToNextHandle(NULL)
{
	SetTimer(NULL,0,1000,MyDoOutTimerMsg);
	dealdata = this;
	m_pCallThread = new CCallThread();
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
}

CDealData::~CDealData(void)							
{
	if(m_hSendToNextHandle)
	{
		TerminateThread(m_hSendToNextHandle,0);
		m_hSendToNextHandle = NULL;
	}
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

	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerAdd1"),NULL,wbuf,255,m_strCallPath);
	CString caller1(wbuf);
	callerid1 = CommonStrMethod::Str2Int(caller1);
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerAdd2"),NULL,wbuf,255,m_strCallPath);
	CString caller2(wbuf);
	callerid2 = CommonStrMethod::Str2Int(caller2);


	
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("IP"),NULL,wbuf,255,m_strCallPath);
	CString strIP(wbuf);
	m_strNextIP = strIP;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Port"),NULL,wbuf,255,m_strCallPath);
	CString strPort(wbuf);
	m_strNextPort = strPort;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("WaitTime"),NULL,wbuf,255,m_strCallPath);
	CString strWaitTime(wbuf);
	m_iWaitTime = CommonStrMethod::Str2Int(strWaitTime);

	///////呼叫线程开始
	m_pCallThread->Start();
	/////////发送到next线程
	m_hSendToNextHandle = CreateThread(NULL,0,SendToNext,this,0,NULL);
	if(!m_hSendToNextHandle)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDealData::HasData()
{
	m_mtDonLock.Lock();
	if (m_DoneList.GetCount()>0)
	{
		m_mtDonLock.Unlock();
		return TRUE;
	}
	m_mtDonLock.Unlock();	
	return FALSE;
}

void CDealData::AddData(SLZData& data)
{
	m_mtWaitLock.Lock();
	data.SetRecvTime(CTime::GetCurrentTime());
	m_WaitList.AddTail(data);
	m_mtWaitLock.Unlock();
}



void CALLBACK CDealData::MyDoOutTimerMsg(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	POSITION pos;
	int count = dealdata->m_WaitList.GetCount();
	for (int i=0;i<count;i++)
	{
		SLZData data;



		dealdata->m_mtWaitLock.Lock();
		pos = dealdata->m_WaitList.FindIndex(i);
		if (!pos)
		{
			dealdata->m_mtWaitLock.Unlock();
			break;
		}
		data = dealdata->m_WaitList.GetAt(pos);
		dealdata->m_mtWaitLock.Unlock();




		CTime currenttime = CTime::GetCurrentTime();
		CTime recvtime = 	data.GetRecvTime();
		CTimeSpan tWaitTime(0,0,data.GetWaitTime(),0);//几分钟
		if (currenttime - recvtime >= tWaitTime)//加入正在处理队列
		{
			
				dealdata->m_mtList1Lock.Lock();
				dealdata->m_DataList1.AddTail(data);
				dealdata->m_mtList1Lock.Unlock();

				///发声
				SLZCallerData callerdata;
				callerdata.SetCmdType(callerCmdShowAlarm);
				callerdata.SetCallerId(data.GetWindowId());
				SLZCCaller::GetInstance()->AddWriteCallerData(callerdata);

				CString strWaitNum = CommonStrMethod::Int2Str(dealdata->m_DataList1.GetCount());
				theApp.m_pMainWnd->SetWindowText(_T("呼叫终端--当前等待人数：")+strWaitNum);
	
				dealdata->m_mtWaitLock.Lock();
				dealdata->m_WaitList.RemoveAt(pos);
				dealdata->m_mtWaitLock.Unlock();
		}
	
	}
}

DWORD WINAPI CDealData::SendToNext(LPVOID pParam)
{
	CDealData* pThis = (CDealData*)pParam;
	while(TRUE)
	{
		if (pThis->HasData())
		{
			pThis->m_mtDonLock.Lock();
			SLZData data;
			if (!CDealData::GetInstance()->m_DoneList.IsEmpty())
			{
				data= CDealData::GetInstance()->m_DoneList.GetHead();
			}
			pThis->m_mtDonLock.Unlock();
			data.SetWaitTime(pThis->m_iWaitTime);
			
			string aSendPacket = CDealPacket::ProduceSendPacket(&data);
			CComplSocketClient client;
			client.SetRemoteIp(pThis->m_strNextIP);
			int nPort = 6000;
			CCommonConvert::CStringToint(nPort,pThis->m_strNextPort);
			client.SetRemotePort(nPort);
			CString wSendMsg(aSendPacket.c_str());
			string recvMsg;int actRecvSize = 0;
			if(client.SendData(wSendMsg,recvMsg,actRecvSize))
			{
				pThis->m_mtDonLock.Lock();
				pThis->m_DoneList.RemoveHead();
				pThis->m_mtDonLock.Unlock();
			}
#ifdef _DEBUG
			CString wRecvMsg(recvMsg.c_str());
			MyWriteConsole(_T("返回消息:") + wRecvMsg);
#endif
		}
	}
	Sleep(10);
}
