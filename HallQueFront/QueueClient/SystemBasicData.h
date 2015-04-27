#pragma once

class CSystemBasicData
{
public:
	CSystemBasicData(void);
	~CSystemBasicData(void);

	CString GetNextIP()const{return m_strNextIP;}
	void SetNextIP(const CString& IP){m_strNextIP = IP;}

	CString GetCallWaitTime()const{return m_strCallWaitTime;}
	void SetCallWaitTime(const CString& callWaitTime){m_strCallWaitTime = callWaitTime;}

	CString GetSendWaitTime()const{return m_strSendWaitTime;}
	void SetSendWaitTime(const CString& sendWaitTime){m_strSendWaitTime = sendWaitTime;}

	int GetReCallNum()const {return m_nReCallNum;}
	void SetReCallNum(int nNum){m_nReCallNum = nNum;}

	BOOL GetIsSendShortMsg()const {return m_isSendShortMsg;}
	void SetIsSendShortMsg(BOOL flag){m_isSendShortMsg = flag;}

	CString GetShortMsg()const {return m_strShortMsg;}
	void SetShortMsg(const CString& shortMsg){m_strShortMsg = shortMsg;}

	BOOL LoadBasicMsg();
	void SaveBasicMsg();

	CString GetCallMsg()const {return m_strCallMsg;}
	void SetCallMsg(const CString& msg){m_strCallMsg = msg;}

	CString GetShowMsg()const {return m_strShowMsg;}
	void SetShowMsg(const CString& msg){m_strShowMsg = msg;}
	
private:
	CString m_strNextIP;
	CString m_strCallWaitTime;
	CString m_strSendWaitTime;
	int m_nReCallNum;//呼叫次数
	BOOL m_isSendShortMsg;
	CString m_strShortMsg;//短信内容
	CString m_strCallMsg;//呼叫内容
	CString m_strShowMsg;//	LED显示内容
private:
	CString m_strPath;
};
