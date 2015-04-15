#pragma once


class SLZCCaller;
class CCallerCmdData;
class CallerCmd;
class SLZCWndScreen;

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

	SLZCCaller* m_pCaller;//呼叫器指针变量
	SLZCWndScreen* m_pWndScreen;//屏指针变量

	CWinThread* m_pDealCallerCmd;
	static UINT DealCallerCmdProc(LPVOID pParam);

	CCallerCmdData* m_pCallerCmdData;

	void DispatchCallerCmd( CallerCmd* callerCmd);

	void ReturnToCaller(CallerCmd* callerCmd);

private:
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
};
