#include "StdAfx.h"
#include "CallerInfo.h"
IMPLEMENT_SERIAL(CallerInfo,CObject,1);
CallerInfo::CallerInfo(void):m_iMsgShowTime(0)
,m_iWndScreenId(0)
,m_iWaitTime(5)
,m_iPlayTimes(3)
{
	m_strShowMsg = _T("#0#��#2#[�ŶӺ���]#0#��#2#[�ͻ�����]#0#��#2#[��������]#0#����ҵ��");
	m_strCallMsg = _T("#��#[�ŶӺ���]#�ŵ�#[��������]#����ҵ��");
	/*m_WaitShowMsg = _T("#0#��#2#[�ŶӺ���]#0#��#2#[�ͻ�����]#0#����׼��#0#   Please No.#2#[�ŶӺ���]#0#  get ready.");
	m_WaitCalledMsg = _T("#��#[�ŶӺ���]#��#����׼��#please#number#[�ŶӺ���(Ӣ)]#get#ready");*/
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
	m_strIP = obj.m_strIP;
	m_strPort = obj.m_strPort;
	m_iWaitTime = obj.m_iWaitTime;
	m_iPlayTimes = obj.m_iPlayTimes;
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
	m_strIP = obj.m_strIP;
	m_strPort = obj.m_strPort;
	m_iWaitTime = obj.m_iWaitTime;
	m_iPlayTimes = obj.m_iPlayTimes;
	return *this;
}
 
void CallerInfo::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_CallerUserName<<m_CallerCallName<<m_iWndScreenId<<m_strCallMsg<<m_strShowMsg<<m_iMsgShowTime
			<<m_strAdMsg<<m_strIP<<m_strPort<<m_iWaitTime<<m_iPlayTimes;
	}
	else
	{
		ar>>m_CallerUserName>>m_CallerCallName>>m_iWndScreenId>>m_strCallMsg>>m_strShowMsg>>m_iMsgShowTime
			>>m_strAdMsg>>m_strIP>>m_strPort>>m_iWaitTime>>m_iPlayTimes;
	}
}