#include "StdAfx.h"
#include "CallerWnd.h"

IMPLEMENT_SERIAL(CCallerWnd,CObject,1);
UINT g_CallerWmdID;
CCallerWnd::CCallerWnd(void) : m_nCallerID(0)
,m_nWndID(0)
{
	m_nSerialID = ++g_CallerWmdID;
	for(int i=0;i<3;i++)
	{
		m_strCanDoBuss[i] = new WCHAR [LENGTH];
		ZeroMemory(m_strCanDoBuss[i],LENGTH);
	}
}

CCallerWnd::~CCallerWnd(void)
{
	for(int i=0;i<3;i++)
	{
		delete [] m_strCanDoBuss[i];
		m_strCanDoBuss[i] = NULL;
	}
}


CCallerWnd::CCallerWnd(const CCallerWnd& obj)
{
	m_nSerialID = obj.m_nSerialID;
	m_nCallerID = obj.m_nCallerID;
	m_nWndID = obj.m_nWndID;
	for(int i=0;i<3;i++)
	{
		wcscpy_s(m_strCanDoBuss[i],LENGTH-1,obj.m_strCanDoBuss[i]);
	}
}

CCallerWnd& CCallerWnd::operator=(const CCallerWnd& obj)
{
	if(this==&obj)return *this;//×Ô¸³Öµ

	m_nSerialID = obj.m_nSerialID;
	m_nCallerID = obj.m_nCallerID;
	m_nWndID = obj.m_nWndID;
	for(int i=0;i<3;i++)
	{
		wcscpy_s(m_strCanDoBuss[i],LENGTH-1,obj.m_strCanDoBuss[i]);
	}
	return *this;
}

void CCallerWnd::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_nSerialID<<m_nCallerID<<m_nWndID<<m_strWndName;
		for(int i=0;i<3;i++)
		{
			WCHAR* temp = m_strCanDoBuss[i];
			int size = wcslen(temp);
			ar << size;
			for(int j=0; j<size; j++)
			{
				ar << temp[j];
			}
		}
	}
	else
	{
		ar>>m_nSerialID>>m_nCallerID>>m_nWndID>>m_strWndName;
		WCHAR tempBuffer[LENGTH] = {0};
		int size = 0;
		for(int i=0;i<3;i++)
		{
			ZeroMemory(tempBuffer,LENGTH);
			ar >> size;
			for(int j=0;j<size;j++)
			{
				ar >> tempBuffer[j];
			}
			wcscpy_s(m_strCanDoBuss[i],LENGTH,tempBuffer);
		}
	}
}