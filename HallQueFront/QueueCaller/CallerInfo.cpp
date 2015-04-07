#include "StdAfx.h"
#include "CallerInfo.h"
IMPLEMENT_SERIAL(CallerInfo,CObject,1);
CallerInfo::CallerInfo(void)
{
}

CallerInfo::~CallerInfo(void)
{
}

CallerInfo::CallerInfo(const CallerInfo& obj)
{
	m_CallerUserName=obj.m_CallerUserName;
	m_CallerCallName=obj.m_CallerCallName;
	m_iWndScreenId=obj.m_iWndScreenId;
	m_strCallMsg=obj.m_strCallMsg;
	m_strShowMsg=obj.m_strShowMsg;
	m_iMsgShowTime=obj.m_iMsgShowTime;
	m_strAdMsg=obj.m_strAdMsg;
}

CallerInfo& CallerInfo::operator=(CallerInfo& obj)
{
	m_CallerUserName=obj.m_CallerUserName;
	m_CallerCallName=obj.m_CallerCallName;
	m_iWndScreenId=obj.m_iWndScreenId;
	m_strCallMsg=obj.m_strCallMsg;
	m_strShowMsg=obj.m_strShowMsg;
	m_iMsgShowTime=obj.m_iMsgShowTime;
	m_strAdMsg=obj.m_strAdMsg;
	return *this;
}
 
void CallerInfo::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_CallerUserName<<m_CallerCallName<<m_iWndScreenId<<m_strCallMsg<<m_strShowMsg<<m_iMsgShowTime
			<<m_strAdMsg;
	}
	else
	{
		ar>>m_CallerUserName>>m_CallerCallName>>m_iWndScreenId>>m_strCallMsg>>m_strShowMsg>>m_iMsgShowTime
			>>m_strAdMsg;
	}
}