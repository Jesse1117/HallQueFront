#include "StdAfx.h"
#include "CallThread.h"
#include "com/SLZCCaller.h"
#include "com/callercmd.h"
#include "com/CallerCmdData.h"
#include "com/SLZCWndScreen.h"

CCallThread::CCallThread(void) : m_pCaller(NULL)
,m_pGetHardCallerCmd(NULL)
,m_pDealCallerCmd(NULL)
,m_pCallerCmdData(NULL)
,m_pWndScreen(NULL)
{
	m_pCaller = SLZCCaller::GetInstance();
	m_pCallerCmdData = new CCallerCmdData;
	m_pWndScreen = SLZCWndScreen::GetInstance();
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
	if(pThis->m_pCallerCmdData->GetHead(cmd))
	{
		pThis->DispatchCallerCmd(&cmd);
	}
	else
	{
		Sleep(10);
	}
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

}
///重呼///
/////////
void CCallThread::OnRecall(CallerCmd* callerCmd)
{

}
///过好///
/////////
void CCallThread::OnDiscard(CallerCmd* callerCmd)
{

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
	return TRUE;
}