#include "StdAfx.h"
#include "UserInfo.h"

IMPLEMENT_SERIAL(CUserInfo,CObject,1);
CUserInfo::CUserInfo(void)
{
}

CUserInfo::~CUserInfo(void)
{
}

CUserInfo::CUserInfo(const CUserInfo& obj)
{
	m_UserName=obj.m_UserName;
	m_PassWord=obj.m_PassWord;
	m_isAutoLogin=obj.m_isAutoLogin;
	m_isRemember=obj.m_isRemember;
}

CUserInfo& CUserInfo::operator=(CUserInfo& obj)
{
	m_UserName=obj.m_UserName;
	m_PassWord=obj.m_PassWord;
	m_isAutoLogin=obj.m_isAutoLogin;
	m_isRemember=obj.m_isRemember;
	return *this;
}

void CUserInfo::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_UserName<<m_PassWord<<m_isAutoLogin<<m_isRemember;
	}
	else
	{
		ar>>m_UserName>>m_PassWord>>m_isAutoLogin>>m_isRemember;
	}
}
