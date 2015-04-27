#pragma once
#include "SLZData.h"
#include <afxwin.h>
#include <afxmt.h>

class SLZCCaller;
class CCallerCmdData;
class CallerCmd;
class SLZCWndScreen;
class CDealData;

class CCallThread
{
public:
	CCallThread(void);
	~CCallThread(void);
public:
	BOOL Start(void);
private:
	static UINT GetHardCallerCmdProc(LPVOID pParam);
	CWinThread* m_pGetHardCallerCmd;

	SLZCCaller* m_pCaller;//������ָ�����
	SLZCWndScreen* m_pWndScreen;//��ָ�����

	CWinThread* m_pDealCallerCmd;
	static UINT DealCallerCmdProc(LPVOID pParam);

	CCallerCmdData* m_pCallerCmdData;

	void DispatchCallerCmd( CallerCmd* callerCmd);

	void ReturnToCaller(CallerCmd* callerCmd);

private:
	CDealData* m_pDealData;
//	CList<SLZData> m_CallingList; // ���ں��ж���
//	UINT m_CallerAdd1;
//	UINT m_CallerAdd2;
//	CString m_strBuss1;
//	CString m_strBuss2;
	int  m_CallWaitTime;   //���еȴ�ʱ��
	CMutex m_mtCallLock;  //������
	

//	CString m_strCallPath;
	BOOL m_bSendMsg;
	CString m_strSendMsg;   //���͵Ķ�������
	void OnLogin(CallerCmd* callerCmd);
	void OnQuit(CallerCmd* callerCmd);
	void OnCall(CallerCmd* callerCmd);
	void OnRecall(CallerCmd* callerCmd);
	void OnDiscard(CallerCmd* callerCmd);
	void OnWait(CallerCmd* callerCmd);
	void OnEvaReq(CallerCmd* callerCmd);
	void OnPause(CallerCmd* callerCmd);
	void OnResume(CallerCmd* callerCmd);
	void OnCallNum(CallerCmd* callerCmd);
	void OnCallSec(CallerCmd* callerCmd);
	void OnCallMana(CallerCmd* callerCmd);
	void OnCallBusc(CallerCmd* callerCmd);
	void OnExChange(CallerCmd* callerCmd);

	static void CALLBACK MyDoOutTimerMsg(
		HWND hwnd, // handle of window for timer messages
		UINT uMsg, // WM_TIMER message
		UINT idEvent, // timer identifier
		DWORD dwTime // current system time	
		);
};
