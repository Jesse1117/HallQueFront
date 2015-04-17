#include "StdAfx.h"
#include "CallThread.h"
#include "com/SLZCCaller.h"
#include "com/callercmd.h"
#include "com/CallerCmdData.h"
#include "com/SLZCWndScreen.h"
#include "DealData.h"
#include "SoundPlay.h"
#include "CommonStrMethod.h"
#include "ShortMsgModem.h"
CCallThread* pCallThread;
CCallThread::CCallThread(void) : m_pCaller(NULL)
,m_pGetHardCallerCmd(NULL)
,m_pDealCallerCmd(NULL)
,m_pCallerCmdData(NULL)
,m_pWndScreen(NULL)
,m_bSendMsg(FALSE)
{
	m_pCaller = SLZCCaller::GetInstance();
	m_pCallerCmdData = new CCallerCmdData;
	m_pWndScreen = SLZCWndScreen::GetInstance();
	m_pDealData = CDealData::GetInstance();
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
}

CCallThread::~CCallThread(void)
{
	if(m_pGetHardCallerCmd)
	{
		TerminateThread(m_pGetHardCallerCmd->m_hThread,0);
		delete m_pGetHardCallerCmd;
		m_pGetHardCallerCmd=NULL;
	}
	if(m_pCallerCmdData)
	{
		delete m_pCallerCmdData;
		m_pCallerCmdData = NULL;
	}
}


UINT CCallThread::GetHardCallerCmdProc(LPVOID pParam)
{
	CCallThread* pThis = (CCallThread*)pParam;

	while(TRUE)
	{
		if(pThis->m_pCaller->HasData())
		{
			SLZCallerData callerData = pThis->m_pCaller->GetData();
			if(callerData.GetCmdType() == callerCmdShowAdd)
			{
				pThis->m_pCaller->AddWriteCallerData(callerData);
			}
			else
			{
				
				CallerCmd cmd;
				cmd.SetCallerAdd(callerData.GetCallerId());
				cmd.SetCmdType(callerData.GetCmdType());
				cmd.SetCarriedData(callerData.GetAttchMsg());
				pThis->m_pCallerCmdData->AddTail(cmd);
			}
		}
		else
		{
			Sleep(10);
		}
	}
	::ExitThread(0);
	return 0;
}

UINT CCallThread::DealCallerCmdProc(LPVOID pParam)
{
	CCallThread* pThis = (CCallThread*)pParam;

	CallerCmd cmd;
	while(TRUE)
	{
		if(pThis->m_pCallerCmdData->GetHead(cmd))
		{
			pThis->DispatchCallerCmd(&cmd);
		}
		else
		{
			Sleep(10);
		}
	}
	Sleep(10);
	return 0;
}

void CCallThread::DispatchCallerCmd(CallerCmd* callerCmd)
{
	switch(callerCmd->GetCmdType())
	{
	case cmdLogin:
		{
			OnLogin(callerCmd);
		}
		break;
	case cmdQuit:
		{
			OnQuit(callerCmd);
		}
		break;
	case cmdCall:
		{
			OnCall(callerCmd);
		}
		break;
	case cmdRecall:
		{
			OnRecall(callerCmd);
		}
		break;
	case cmdDiscard:
		{
			OnDiscard(callerCmd);
		}
		break;
	case cmdWait:
		{
			OnWait(callerCmd);
		}
		break;
	case cmdEvaReq:
		{
			OnEvaReq(callerCmd);
		}
		break;
	case cmdPause:
		{
			OnPause(callerCmd);
		}
		break;
	case cmdResume:
		{
			OnResume(callerCmd);
		}
		break;
	case cmdCallNum:
		{
			OnCallNum(callerCmd);
		}
		break;
	case cmdCallSec:
		{
			OnCallSec(callerCmd);
		}
		break;
	case cmdCallMana:
		{
			OnCallMana(callerCmd);
		}
		break;
	case cmdCallBusc:
		{
			OnCallBusc(callerCmd);
		}
		break;
	case cmdExChange:
		{
			OnExChange(callerCmd);
		}
		break;
	case callerCmdShowAdd:
		break;
	default:
		{
			return;
		}
		break;
	}
	///处理完后返回
	ReturnToCaller(callerCmd);
}

////登录////
///////////
void CCallThread::OnLogin(CallerCmd* callerCmd)
{

}
////退出////
///////////
void CCallThread::OnQuit(CallerCmd* callerCmd)
{

}
////呼叫////
////////////
void CCallThread::OnCall(CallerCmd* callerCmd)
{	
	SLZData data;
	if (callerCmd->GetCallerAdd()==m_CallerAdd1)
	{
		if (m_pDealData->m_DataList1.GetCount()>0)
		{
		
			m_mtCallLock.Lock();
			data = m_pDealData->m_DataList1.GetHead();
			m_mtCallLock.Unlock();
			CSoundPlay::GetInstance()->DataPlay(data);
			data.SetCallTime(CTime::GetCurrentTime());
			m_mtCallLock.Lock();
				for (int i=0;i<m_CallingList.GetCount();i++)
				{
					POSITION pos;
					pos  = m_CallingList.FindIndex(i);
					if (pos)
					{
						SLZData data = m_CallingList.GetAt(pos);
						if (data.GetBussName()==m_strBuss1)
						{
							m_CallingList.RemoveAt(pos);
						}
					}
				}
				m_CallingList.AddTail(data);
				m_mtCallLock.Unlock();
			}
	}
	if (callerCmd->GetCallerAdd()==m_CallerAdd2)
	{
		if (m_pDealData->m_DataList2.GetCount()>0)
		{
			m_mtCallLock.Lock();
			data = m_pDealData->m_DataList2.GetHead();
			m_mtCallLock.Unlock();
			CSoundPlay::GetInstance()->DataPlay(data);
			data.SetCallTime(CTime::GetCurrentTime());
			m_mtCallLock.Lock();
			for (int i=0;i<m_CallingList.GetCount();i++)
			{
				POSITION pos;
				pos  = m_CallingList.FindIndex(i);
				if (pos)
				{
					SLZData data = m_CallingList.GetAt(pos);
					if (data.GetBussName()==m_strBuss2)
					{
						m_CallingList.RemoveAt(pos);
					}
				}
			}
			m_CallingList.AddTail(data);
			m_mtCallLock.Unlock();
		}
	}
	if (m_bSendMsg)
	{
		if (!data.GetPhoneNum().IsEmpty())
		{
			CShortMsgModem::GetInstance()->SendMsg(data.GetPhoneNum(),m_strSendMsg);
		}

	}
}
///重呼///
/////////
void CCallThread::OnRecall(CallerCmd* callerCmd)
{
	if (m_CallingList.GetCount()>0)
	{
		SLZData data;
		POSITION pos;
		for (int i=0;i<m_CallingList.GetCount();i++)
		{
			m_mtCallLock.Lock();
			pos = m_CallingList.FindIndex(i);
			data = m_CallingList.GetAt(pos);
			m_mtCallLock.Unlock();
			if (((data.GetBussName()==m_strBuss1)&& callerCmd->GetCallerAdd()==m_CallerAdd1)||((data.GetBussName()==m_strBuss2)&& callerCmd->GetCallerAdd() == m_CallerAdd2))
			{
				CSoundPlay::GetInstance()->DataPlay(data);
			}
		}
	}
}
///过好///
/////////
void CCallThread::OnDiscard(CallerCmd* callerCmd)
{
	if (callerCmd->GetCallerAdd()==m_CallerAdd1)
	{
		m_mtCallLock.Lock();
		SLZData data ;
		data = m_pDealData->m_DataList1.GetHead();
		m_pDealData->m_DataList1.RemoveHead();
		m_pDealData->m_DoneList.AddTail(data);
		m_mtCallLock.Unlock();
	}
	if (callerCmd->GetCallerAdd()==m_CallerAdd2)
	{
		m_mtCallLock.Lock();
		SLZData data ;
		data = m_pDealData->m_DataList2.GetHead();
		m_pDealData->m_DataList2.RemoveHead();
		m_pDealData->m_DoneList.AddTail(data);
		m_mtCallLock.Unlock();
	}
}
///等待///
/////////
void CCallThread::OnWait(CallerCmd* callerCmd)
{

}
///接收到开启评价///
/////////
void CCallThread::OnEvaReq(CallerCmd* callerCmd)
{

}
///暂停///
/////////
void CCallThread::OnPause(CallerCmd* callerCmd)
{

}
///恢复///
//////////
void CCallThread::OnResume(CallerCmd* callerCmd)
{

}
//呼叫特定号码///
void CCallThread::OnCallNum(CallerCmd* callerCmd)
{
	CString callNum = callerCmd->GetCarriedData();//获取附加数据，就是呼叫的号码
	callerCmd->SetSuccess(TRUE);
	//playsound,display
	SLZData data;
	data.SetQueueNumber(callNum);
	CSoundPlay::GetInstance()->DataPlay(data);
}
//呼叫保安///
void CCallThread::OnCallSec(CallerCmd* callerCmd)
{

}
/*
呼叫大堂经理
*/
void CCallThread::OnCallMana(CallerCmd* callerCmd)
{

}
/*
呼叫业务顾问
*/
void CCallThread::OnCallBusc(CallerCmd* callerCmd)
{

}
/*
转移队列/窗口
*/
void CCallThread::OnExChange(CallerCmd* callerCmd)
{

}
/*返回给呼叫器*/
void CCallThread::ReturnToCaller(CallerCmd* callerCmd)
{
	SLZCCaller* pSLZCCaller = SLZCCaller::GetInstance();
	SLZCallerData data;
	data.SetCmdType(callerCmd->GetCmdType());//命令类型
	data.SetAttchMsg(callerCmd->GetCarriedData());//附加信息

	switch(data.GetCmdType())
	{
	case cmdLogin:
		data.SetCmdType(callerCmd->GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case	cmdQuit:
		data.SetCmdType(callerCmd->GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCall:
		data.SetCmdType(callerCmdShowNum);
		break;
	case cmdRecall:
		data.SetCmdType(callerCmdShowNum);
		break;
	case cmdDiscard:
		data.SetCmdType(callerCmdShowNum);
		break;
	case cmdWait:
		data.SetCmdType(callerCmdShowNum);
		break;
	case cmdEvaReq:
		//		data.SetCmdType(callerCmd.GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case 	cmdPause:
		data.SetCmdType(callerCmd->GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCallNum:
		data.SetCmdType(callerCmd->GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCallSec:
		data.SetCmdType(callerCmd->GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCallMana:
		data.SetCmdType(callerCmd->GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdCallBusc:
		data.SetCmdType(callerCmd->GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case cmdExChange:
		data.SetCmdType(callerCmd->GetSuccess() ? callerCmdShowSuc : callerCmdShowFail);
		break;
	case callerCmdShowAdd:
		data.SetCmdType(callerCmdShowAdd);
		break;
	default:
		break;
	}
	data.SetCallerId(callerCmd->GetCallerAdd());//呼叫器地址
	pSLZCCaller->AddWriteCallerData(data);
}


BOOL CCallThread::Start()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerAdd1"),NULL,wbuf,255,m_strCallPath);
	CString strCallerAdd1(wbuf);
	m_CallerAdd1 = CommonStrMethod::Str2Int(strCallerAdd1);
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerAdd2"),NULL,wbuf,255,m_strCallPath);
	CString strCallerAdd2(wbuf);
	m_CallerAdd2 = CommonStrMethod::Str2Int(strCallerAdd2);
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Buss1"),NULL,wbuf,255,m_strCallPath);
	CString Buss1(wbuf);
	m_strBuss1 = Buss1;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Buss2"),NULL,wbuf,255,m_strCallPath);
	CString Buss2(wbuf);
	m_strBuss2 = Buss2;

	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallWaitTime"),NULL,wbuf,255,m_strCallPath);
	CString callwait(wbuf);
	m_CallWaitTime = CommonStrMethod::Str2Int(callwait);
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("IsSendMsg"),NULL,wbuf,255,m_strCallPath);
	CString strSendMsg(wbuf);
	m_bSendMsg = CommonStrMethod::Str2Int(strSendMsg);
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("ShortMsg"),NULL,wbuf,255,m_strCallPath);
	CString strShortMsg(wbuf);
	m_strSendMsg = strShortMsg;
	m_pGetHardCallerCmd = AfxBeginThread(GetHardCallerCmdProc,this,0,0,0,NULL);//读呼叫线程
	if(!m_pGetHardCallerCmd)
	{
		return FALSE;
	}

	m_pDealCallerCmd = AfxBeginThread(DealCallerCmdProc,this,0,0,0,NULL);
	if(!m_pDealCallerCmd)
	{
		return FALSE;
	}
	SetTimer(NULL,0,1000,MyDoOutTimerMsg);
	return TRUE;
}

void CALLBACK CCallThread::MyDoOutTimerMsg(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	POSITION pos;
	int count = pCallThread->m_CallingList.GetCount();
	for (int i=0;i<count;i++)
	{
		SLZData data;
		pos = pCallThread->m_CallingList.FindIndex(i);
		if (!pos)break;
		data = pCallThread->m_CallingList.GetAt(pos);
		CTime currenttime = CTime::GetCurrentTime();
		CTime calltime = 	data.GetCallTime();
		if (currenttime-calltime == CTimeSpan(0,0,pCallThread->m_CallWaitTime,0))
		{	
			pCallThread->m_mtCallBackLock.Lock();
			if (data.GetBussName()==pCallThread->m_strBuss1)
			{
				
				pCallThread->m_pDealData->m_DataList1.RemoveHead();
				pCallThread->m_pDealData->m_DataList1.AddTail(data);
				
			}
			if (data.GetBussName()==pCallThread->m_strBuss2)
			{
				pCallThread->m_pDealData->m_DataList2.RemoveHead();
				pCallThread->m_pDealData->m_DataList2.AddTail(data);
			}	
			pCallThread->m_CallingList.RemoveAt(pos);
			pCallThread->m_mtCallBackLock.Unlock();
		}

	}
}
