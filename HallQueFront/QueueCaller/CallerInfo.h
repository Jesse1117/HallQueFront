#pragma once

class CallerInfo:public CObject
{
public:
	CallerInfo(void);
	~CallerInfo(void);
	CallerInfo(const CallerInfo& obj);
	CallerInfo& operator=(CallerInfo& obj);

	CString GetUserName()const
	{
		return m_CallerUserName;
	}
	void SetUserName(const CString strUserName)
	{
		m_CallerUserName = strUserName;
	}

	CString GetCallName()const
	{
		return m_CallerCallName;
	}
	void SetCallName(const CString strCallName)
	{
		m_CallerCallName = strCallName;
	}

	int GetWndScreenId()const
	{
		return m_iWndScreenId;
	}
	void SetWndScreenId(const int WndId)
	{
		m_iWndScreenId = WndId;
	}

	CString GetCallMsg()const
	{
		return m_strCallMsg;
	}
	void SetCallMsg(const CString strCallMsg)
	{
		m_strCallMsg = strCallMsg;
	}

	CString GetShowMsg()const
	{
		return m_strShowMsg;
	}
	void SetShowMsg(const CString strShowMsg)
	{
		m_strShowMsg = strShowMsg;
	}

	UINT GetMsgShowTime()
	{
		return m_iMsgShowTime;
	}

	void SetMsgShowTime(const UINT& MsgShowTime)
	{
		m_iMsgShowTime=MsgShowTime;
	}

	CString GetAdMsg()
	{
		return m_strAdMsg;
	}

	void SetAdMsg(const CString& AdMsg)
	{
		m_strAdMsg=AdMsg;
	}

	CString GetStrIP()
	{
		return m_strIP;
	}
	void SetStrIP(const CString strIP)
	{
		m_strIP = strIP;
	}

	CString GetStrPort()
	{
		return m_strPort;
	}
	void SetStrPort(const CString strPort)
	{
		m_strPort = strPort;
	}

	UINT GetWatiTime()
	{
		return m_iWaitTime;
	}
	void SetWaitTime(const UINT iWatiTime)
	{
		m_iWaitTime = iWatiTime;
	}

	UINT GetPlayTimes(){return m_iPlayTimes;}
	void SetPlayTimes(const UINT iPlayTimes){m_iPlayTimes = iPlayTimes;}

	virtual void Serialize( CArchive& ar );//支持序列化
	DECLARE_SERIAL(CallerInfo)

private:
	CString m_CallerUserName;
	CString m_CallerCallName;
	int m_iWndScreenId;
	CString m_strCallMsg;
	CString m_strShowMsg;
	unsigned int m_iMsgShowTime;		//信息显示时间
	CString m_strAdMsg;				//广告信息
	CString m_strIP;
	CString m_strPort;
	UINT m_iWaitTime;
	UINT m_iPlayTimes;
};
