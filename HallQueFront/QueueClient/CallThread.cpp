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
#include "QueueClient.h"
#include "SystemBasicData.h"
#include "../HallQueFront/CommonConvert.h"

extern void MyWriteConsole(CString str);

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
}

CCallThread::~CCallThread(void)
{
	if(m_pGetHardCallerCmd)
	{
		TerminateThread(m_pGetHardCallerCmd->m_hThread,0);
		delete m_pGetHardCallerCmd;
		m_pGetHardCallerCmd=NULL;
	}

	if(m_pDealCallerCmd)
	{
		TerminateThread(m_pDealCallerCmd->m_hThread,0);
		delete m_pDealCallerCmd;
		m_pDealCallerCmd = NULL;
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
	///������󷵻�
	ReturnToCaller(callerCmd);
}

////��¼////
///////////
void CCallThread::OnLogin(CallerCmd* callerCmd)
{

}
////�˳�////
///////////
void CCallThread::OnQuit(CallerCmd* callerCmd)
{

}
////����////
////////////
void CCallThread::OnCall(CallerCmd* callerCmd)
{	
	SLZData callingData;
	if (!m_pDealData->IsEmptyCallingListData())
	{
		if(m_pDealData->RemoveCallingListData(callerCmd,callingData))
			m_pDealData->AddDoneListData(callingData);
	}
	

	SLZData data;

	if (m_pDealData->GetDoingDataCount() > 0)
	{
		if(!m_pDealData->GetDoingFirstData(callerCmd,data))
			return;
	
		CSoundPlay::GetInstance()->DataPlay(data);
		data.SetCallTime(CTime::GetCurrentTime());
	}
	
	if (m_bSendMsg)
	{
		if (!data.GetPhoneNum().IsEmpty() && !data.GetIsHaveSendToPhone())
		{
			CShortMsgModem::GetInstance()->SendMsg(data.GetPhoneNum(),m_strSendMsg);
			///��Ϊ�Ѿ�����
//			data.SetIsHaveSendToPhone(TRUE);

			//////////////////////////////////////////�޸�list
//			m_pDealData->ModifyDoingListData(data);

#ifdef _DEBUG
			CString strMsg;
			strMsg.Format(_T("�Ѿ������˶���,data IDΪ:%s"),data.GetSerialId());
			MyWriteConsole(strMsg);
#endif
		}
	}
	
	////////////////////////////////����Ϊ���ں���
//	data.SetIsCalling(TRUE);
//	data.SetCallingAdd(callerCmd->GetCallerAdd());
//	m_pDealData->ModifyDoingListData(data);

	SLZData* pData = new SLZData;
	*pData = data;
	::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_SHOWCALLMSG,(WPARAM)pData,NULL);
	
	///��������
	int cout = m_pDealData->GetDoingDataCount();
	CString carriedData = data.GetQueueNumber() + _T(" ");
	carriedData.AppendFormat(_T("%d"),cout);
	callerCmd->SetCarriedData(carriedData);
}
///�غ�///
/////////
void CCallThread::OnRecall(CallerCmd* callerCmd)
{
	
}
///����///
/////////
void CCallThread::OnDiscard(CallerCmd* callerCmd)
{
	//if (callerCmd->GetCallerAdd()==m_CallerAdd1)
	//{
	//	m_mtCallLock.Lock();
	//	SLZData data ;
	//	if (!m_pDealData->m_DataList1.IsEmpty())
	//	{
	//		data = m_pDealData->m_DataList1.GetHead();
	//		m_pDealData->m_DataList1.RemoveHead();
	//		m_pDealData->m_DoneList.AddTail(data);
	//	}
	//	m_mtCallLock.Unlock();
	//}
	//else if (callerCmd->GetCallerAdd()==m_CallerAdd2)
	//{
	//	m_mtCallLock.Lock();
	//	SLZData data ;
	//	data = m_pDealData->m_DataList1.GetHead();
	//	m_pDealData->m_DataList1.RemoveHead();
	//	m_pDealData->m_DoneList.AddTail(data);
	//	m_mtCallLock.Unlock();
	//}
}
///�ȴ�///
/////////
void CCallThread::OnWait(CallerCmd* callerCmd)
{
	
}
///���յ���������///
/////////
void CCallThread::OnEvaReq(CallerCmd* callerCmd)
{
	
	SLZData data ;
	if (!m_pDealData->IsEmptyCallingListData())
	{
		if(!m_pDealData->RemoveCallingListData(callerCmd,data))
			return;
		
		m_pDealData->AddDoneListData(data);
#ifdef _DEBUG
		CString writeMsg;
		writeMsg.Format(_T("1�ŵ�ַ���������IDΪ:%s"),data.GetSerialId());
		MyWriteConsole(writeMsg);
#endif
	}
	///��������
	int cout = m_pDealData->GetDoingDataCount();
	CString carriedData = data.GetQueueNumber() + _T(" ");
	carriedData.AppendFormat(_T("%d"),cout);
	callerCmd->SetCarriedData(carriedData);

	CString* pWaitText = new CString;
	pWaitText->Format(_T("�ȴ�����:%d"),cout);
	::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_WAITNUMMSG,(WPARAM)pWaitText,NULL);
	//////////////////////////
	SLZData* pData = new SLZData;
	*pData = data;
	::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_SHOWDONEMSG,(WPARAM)pData,NULL);
}
///��ͣ///
/////////
void CCallThread::OnPause(CallerCmd* callerCmd)
{
// 	SLZCWndScreen* pWndScreen = SLZCWndScreen::GetInstance();
// 	
// 	pWndScreen->AddScreenMsg(_T("��ͣ����"),callerCmd->
}
///�ָ�///
//////////
void CCallThread::OnResume(CallerCmd* callerCmd)
{

}
//�����ض�����///
void CCallThread::OnCallNum(CallerCmd* callerCmd)
{
	CString callNum = callerCmd->GetCarriedData();//��ȡ�������ݣ����Ǻ��еĺ���
	callerCmd->SetSuccess(TRUE);
	//playsound,display
	SLZData data;
	data.SetQueueNumber(callNum);
	CSoundPlay::GetInstance()->DataPlay(data);
}
//���б���///
void CCallThread::OnCallSec(CallerCmd* callerCmd)
{

}
/*
���д��þ���
*/
void CCallThread::OnCallMana(CallerCmd* callerCmd)
{

}
/*
����ҵ�����
*/
void CCallThread::OnCallBusc(CallerCmd* callerCmd)
{

}
/*
ת�ƶ���/����
*/
void CCallThread::OnExChange(CallerCmd* callerCmd)
{
	SLZData data;
	m_pDealData->ReStartQueue(callerCmd,data);

	SLZData* pData = new SLZData;
	*pData = data;
	::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_SHOWNORMALMSG,(WPARAM)pData,NULL);
}
/*���ظ�������*/
void CCallThread::ReturnToCaller(CallerCmd* callerCmd)
{
	SLZCCaller* pSLZCCaller = SLZCCaller::GetInstance();
	SLZCallerData data;
	data.SetCmdType(callerCmd->GetCmdType());//��������
	data.SetAttchMsg(callerCmd->GetCarriedData());//������Ϣ

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
		data.SetCmdType(callerCmdShowNum);
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
	data.SetCallerId(callerCmd->GetCallerAdd());//��������ַ
	pSLZCCaller->AddWriteCallerData(data);
}


BOOL CCallThread::Start()
{
	/*
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
	GetPrivateProfileString(_T("CompSet"),_T("IfSendMsg"),NULL,wbuf,255,m_strCallPath);
	CString strSendMsg(wbuf);
	m_bSendMsg = CommonStrMethod::Str2Int(strSendMsg);
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("ShortMsg"),NULL,wbuf,255,m_strCallPath);
	CString strShortMsg(wbuf);
	m_strSendMsg = strShortMsg;
	*/
	CSystemBasicData basicData;
	m_bSendMsg = basicData.GetIsSendShortMsg();
	m_strSendMsg = basicData.GetShortMsg();
	CCommonConvert::CStringToint(m_CallWaitTime,basicData.GetCallWaitTime());


	m_pGetHardCallerCmd = AfxBeginThread(GetHardCallerCmdProc,this,0,0,0,NULL);//�������߳�
	if(!m_pGetHardCallerCmd)
	{
		return FALSE;
	}

	m_pDealCallerCmd = AfxBeginThread(DealCallerCmdProc,this,0,0,0,NULL);
	if(!m_pDealCallerCmd)
	{
		return FALSE;
	}
//	SetTimer(NULL,0,1500,MyDoOutTimerMsg);

	m_pDealData->Start();
	return TRUE;
}

void CALLBACK CCallThread::MyDoOutTimerMsg(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
//	pCallThread->m_pDealData->m_mtDoingLock.Lock();
	
//	pCallThread->m_pDealData->m_mtDoingLock.Unlock();
}
