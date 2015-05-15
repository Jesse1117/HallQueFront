// InlineQueData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "InlineQueData.h"
#include "CommonConvert.h"


// CInlineQueData
extern void MyWriteConsole(CString str);

CInlineQueData::CInlineQueData(SLZWindowQueryView& rWindowTable)
: m_rWindowTable(rWindowTable)
{
}

CInlineQueData::~CInlineQueData()
{
}

/*
��ȡ�Ŷ� �����׸�����
*/
BOOL CInlineQueData::GetInlineQueData(const UINT iWinId, 
									  SLZData& rdata)
{
	if(m_lstInlineQue.GetCount() < 1)
	{
		return FALSE;
	}
	CStringArray arrStrQueId;//�ɴ�������Լ����ȼ�
	SLZWindow Window;
	BOOL flag = m_rWindowTable.QueryWindowById(iWinId,Window);
	if(!flag)return FALSE;
	Window.GetArrayQueId(arrStrQueId);//��ȡ�ɴ�����м����ȼ�
	CString staffID = theApp.m_Controller.m_mapLoginList[Window.GetWindowId()];//��ȡ��¼STAFFID
	if(staffID.IsEmpty())return FALSE;
	if(arrStrQueId.GetCount() < 1)
	{
		return FALSE;
	}
	BOOL bFind = FALSE;
	///////////////////////
	m_mtInlineQue.Lock();
	for(int i = 0; i < arrStrQueId.GetCount(); i++)
	{
		POSITION pos = m_lstInlineQue.GetHeadPosition();
		while(pos)
		{
			SLZData data;
			POSITION posLast = pos;
			data = m_lstInlineQue.GetNext(pos);
//			if(data.GetWindowId()==0)//���û����ָ������
//			{
				if(arrStrQueId[i].Compare(data.GetBussinessType()) == 0)
				{
					rdata = data;
					m_lstInlineQue.RemoveAt(posLast);
					bFind = TRUE;
					break;
				}
//			}
// 			else//������ָ���������Ⱥ���
// 			{
// 				if(iWinId == data.GetWindowId())
// 				{
// 					rdata = data;
// 					m_lstInlineQue.RemoveAt(posLast);
// 					bFind = TRUE;
// 					break;
// 				}
// 			}
		}
		if(bFind)
		{
			break;
		}
	}
	CTime currTime = CTime::GetCurrentTime();
	rdata.SetCallTime(currTime);//���ú���ʱ��
	rdata.SetStaffId(staffID);//����Ա��ID,�ĸ�Ա������
	rdata.SetWindowId(iWinId);//�����ĸ����ں���
	rdata.SetWindowShowId(Window.GetShowWndId());//���ô��ڱ��
	m_mtInlineQue.Unlock();
	return bFind;
}

void CInlineQueData::Add(SLZData& data)
{
	m_mtInlineQue.Lock();
	m_lstInlineQue.AddTail(data);
	m_mtInlineQue.Unlock();
}

void CInlineQueData::AddHeadData(const SLZData& data)
{
	m_mtInlineQue.Lock();
	m_lstInlineQue.AddHead(data);
	m_mtInlineQue.Unlock();
}

UINT CInlineQueData::GetCount()
{
	return m_lstInlineQue.GetCount();
}

UINT CInlineQueData::GetBussCount(const CString& strBussId)
{
	m_mtInlineQue.Lock();
	UINT iCount = 0;
	POSITION pos = m_lstInlineQue.GetHeadPosition();
	SLZData data;
	for(; pos; )
	{
		data = m_lstInlineQue.GetNext(pos);
		CString queID = data.GetBussinessType();
		if(queID == strBussId)
		{
			iCount++;
		}
	}
	m_mtInlineQue.Unlock();
	return iCount;
}

// CInlineQueData ��Ա����

BOOL CInlineQueData::GetInlineQueData(SLZData& rdata)
{
	m_mtInlineQue.Lock();
	if(!m_lstInlineQue.GetCount())
	{
		m_mtInlineQue.Unlock();
		return FALSE;
	}
	rdata = m_lstInlineQue.GetHead();
	m_lstInlineQue.RemoveHead();
	m_mtInlineQue.Unlock();
	return TRUE;
}

void CInlineQueData::RemoveAllData()
{
	m_mtInlineQue.Lock();
	m_lstInlineQue.RemoveAll();
	m_mtInlineQue.Unlock();
}

int CInlineQueData::GetMaxQueNum(const CString queID)
{
	m_mtInlineQue.Lock();
	CString maxNum;
	POSITION pos = m_lstInlineQue.GetHeadPosition();
	while(pos)
	{
		SLZData data = m_lstInlineQue.GetNext(pos);
		if(data.GetBussinessType() == queID)
		{
			CString queNum = data.GetQueueNumber();
			maxNum = maxNum > queNum ? maxNum : queNum;
		}
	}
	CString num;
	if(!maxNum.IsEmpty())
	{
		for(int i=0;i<maxNum.GetLength();i++)
		{
			WCHAR w = maxNum.GetAt(i);
			if(w>'0'&&w<'9')
			{
				num+=w;
			}
		}
	}
	m_mtInlineQue.Unlock();
	int iNum = 0;
	CCommonConvert convert;
	convert.CStringToint(iNum,num);
	return iNum;
}

UINT CInlineQueData::GetCandoQueCount(UINT iWinID)
{
	UINT uWaitCount = 0;
	SLZWindow Window;
	BOOL flag = m_rWindowTable.QueryWindowById(iWinID,Window);
	ASSERT(flag != FALSE);
	CStringArray arrayQueID;
	Window.GetArrayQueId(arrayQueID);
	for(int i=0;i<arrayQueID.GetCount();i++)
	{
		CString queID = arrayQueID.GetAt(i);
		ASSERT(!queID.IsEmpty());
		uWaitCount += GetBussCount(queID);
	}
	return uWaitCount;
}

BOOL CInlineQueData::AlterData(const CString QueNum,const CString strPhoneNum,const CString strMsg)
{
	m_mtInlineQue.Lock();
	int iCount = m_lstInlineQue.GetCount();
	for (int i = 0;i<iCount;i++)
	{
		SLZData data;
		POSITION pos = m_lstInlineQue.FindIndex(i);
		if (pos)
		{
			data = m_lstInlineQue.GetAt(pos);
			if (data.GetQueueNumber()==QueNum)
			{
				data.SetPhoneNum(strPhoneNum);
				data.SetSendMsg(strMsg);
				m_lstInlineQue.SetAt(pos,data);
				m_mtInlineQue.Unlock();
				return TRUE;
			}
		}
	}
	m_mtInlineQue.Unlock();
	return FALSE;
}

BOOL CInlineQueData::GetInlineQueData(int i,SLZData& data)
{
	if(!GetCount() || i<0)return FALSE;
	m_mtInlineQue.Lock();
	POSITION pos = m_lstInlineQue.FindIndex(i);
	if (pos)
	{
		data = m_lstInlineQue.GetAt(pos);
		m_mtInlineQue.Unlock();
		return TRUE;
	}
	m_mtInlineQue.Unlock();
	return FALSE;
}