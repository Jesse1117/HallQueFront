#include "StdAfx.h"
#include "SystemBasicData.h"
#include "../HallQueFront/DoFile.h"
#include "../HallQueFront/CommonConvert.h"

CSystemBasicData::CSystemBasicData(void) : m_nReCallNum(1)
,m_isSendShortMsg(TRUE)
{
	CDoFile doFile;
	m_strPath = doFile.GetExeFullFilePath();
	m_strPath += _T("\\BasiceData");
	doFile.CreateMyDirectory(m_strPath);
	m_strPath += _T("\\basic.ini");
	
	LoadBasicMsg();
	if(m_strCallMsg.IsEmpty())
	{
		m_strCallMsg = _T("#请#[排队号码]#号#办理业务");
	}
	if(m_strShowMsg.IsEmpty())
	{
		m_strShowMsg = _T("请[排队号码]号[客户姓名]办理业务");
	}
}

CSystemBasicData::~CSystemBasicData(void)
{
}


BOOL CSystemBasicData::LoadBasicMsg()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("IP"),NULL,wbuf,255,m_strPath);
	CString strIp(wbuf);
	m_strNextIP = strIp;

	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("SendWaitTime"),NULL,wbuf,255,m_strPath);
	CString strSendWaitTime(wbuf);
	m_strSendWaitTime = strSendWaitTime;


	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallWaitTime"),NULL,wbuf,255,m_strPath);
	CString strCallWaitTime(wbuf);
	m_strCallWaitTime = strCallWaitTime;
	
	wchar_t wShortMsg[1024]={0};
	GetPrivateProfileString(_T("CompSet"),_T("ShortMsg"),NULL,wShortMsg,1024,m_strPath);
	CString strShortMsg(wShortMsg);
	m_strShortMsg = strShortMsg;

	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("ReCallNum"),NULL,wbuf,255,m_strPath);
	CString strReCallNum(wbuf);
	CCommonConvert::CStringToint(m_nReCallNum,strReCallNum);

	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("IsSendShortMsg"),NULL,wbuf,255,m_strPath);
	CString strIsSendShortMsg(wbuf);
	CCommonConvert::CStringToint(m_isSendShortMsg,strIsSendShortMsg);
	
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallMsg"),NULL,wbuf,255,m_strPath);
	CString strCallMsg(wbuf);
	m_strCallMsg = strCallMsg;
	
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("ShowMsg"),NULL,wbuf,255,m_strPath);
	CString strShowMsg(wbuf);
	m_strShowMsg = strShowMsg;

	return TRUE;
}

void CSystemBasicData::SaveBasicMsg()
{
	WritePrivateProfileString(_T("CompSet"),_T("IP"),m_strNextIP,m_strPath);
	WritePrivateProfileString(_T("CompSet"),_T("SendWaitTime"),m_strSendWaitTime,m_strPath);
	WritePrivateProfileString(_T("CompSet"),_T("CallWaitTime"),m_strCallWaitTime,m_strPath);
	WritePrivateProfileString(_T("CompSet"),_T("ShortMsg"),m_strShortMsg,m_strPath);
	WritePrivateProfileString(_T("CompSet"),_T("CallMsg"),m_strCallMsg,m_strPath);
	WritePrivateProfileString(_T("CompSet"),_T("ShowMsg"),m_strShowMsg,m_strPath);

	CString strReCallNum,strIsSendShortMsg;
	strReCallNum.Format(_T("%d"),m_nReCallNum);
	strIsSendShortMsg.Format(_T("%d"),m_isSendShortMsg);

	WritePrivateProfileString(_T("CompSet"),_T("ReCallNum"),strReCallNum,m_strPath);
	WritePrivateProfileString(_T("CompSet"),_T("IsSendShortMsg"),strIsSendShortMsg,m_strPath);
}