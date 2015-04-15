#pragma once

#include "CallerCmd.h"
#include <afxmt.h>

class CCallerCmdData
{
public:
	CCallerCmdData(void);
	~CCallerCmdData(void);
	BOOL GetHead(CallerCmd& callerCmd);
	void AddTail(CallerCmd& callerCmd);
	UINT GetCount();

private:
	CList<CallerCmd> m_lstCallerCmd;//�����б�
	CMutex m_mtCallerCmd;
};
