#include "StdAfx.h"
#include "DealData.h"
#include "CommonStrMethod.h"
#include "com/SLZCCaller.h"
#include "QueueClient.h"
#include "ComplSocketClient.h"
#include "DealPacket.h"
#include "../HallQueFront/CommonConvert.h"
#include "SystemBasicData.h"
#include "CallerWnd.h"
#include "../HallQueFront/DoFile.h"

CDealData* dealdata;
extern void MyWriteConsole(CString str);

CDealData::CDealData(void) : m_iWaitTime(0)
,m_hSendToNextHandle(NULL)
,m_CallWaitTime(0)
{
	ReadWndData();

	
	dealdata = this;
	

	CDoFile doFile;
	m_doingSaveDataPath = doFile.GetExeFullFilePath();
	m_doingSaveDataPath += _T("\\Data");
	m_waitSaveDataPath = m_doingSaveDataPath;
	doFile.CreateMyDirectory(m_doingSaveDataPath);
	m_doingSaveDataPath += _T("\\Data.dat");

	m_waitSaveDataPath += _T("\\Wait.dat");
}

CDealData::~CDealData(void)							
{
	if(m_hSendToNextHandle)
	{
		TerminateThread(m_hSendToNextHandle,0);
		m_hSendToNextHandle = NULL;
	}
	FreeWndData();
}

BOOL CDealData::Start()
{
	/*
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Buss1"),NULL,wbuf,255,m_strCallPath);
	CString Buss1(wbuf);
	m_strBuss1 = Buss1;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Buss2"),NULL,wbuf,255,m_strCallPath);
	CString Buss2(wbuf);
	m_strBuss2 = Buss2;

	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerAdd1"),NULL,wbuf,255,m_strCallPath);
	CString caller1(wbuf);
	callerid1 = CommonStrMethod::Str2Int(caller1);
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerAdd2"),NULL,wbuf,255,m_strCallPath);
	CString caller2(wbuf);
	callerid2 = CommonStrMethod::Str2Int(caller2);


	
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("IP"),NULL,wbuf,255,m_strCallPath);
	CString strIP(wbuf);
	m_strNextIP = strIP;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Port"),NULL,wbuf,255,m_strCallPath);
	CString strPort(wbuf);
	m_strNextPort = strPort;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("WaitTime"),NULL,wbuf,255,m_strCallPath);
	CString strWaitTime(wbuf);
	m_iWaitTime = CommonStrMethod::Str2Int(strWaitTime);
	*/
	
	ReadDoingData();
	ReadWaitData();


	CSystemBasicData basicData;
	m_strNextIP = basicData.GetNextIP();
	m_iWaitTime = CommonStrMethod::Str2Int(basicData.GetSendWaitTime());
	m_CallWaitTime = CommonStrMethod::Str2Int(basicData.GetCallWaitTime());
// 	m_pCallThread = new CCallThread();
// 	///////呼叫线程开始
// 	m_pCallThread->Start();
	/////////发送到next线程
	m_hSendToNextHandle = CreateThread(NULL,0,SendToNext,this,0,NULL);
	if(!m_hSendToNextHandle)
	{
		return FALSE;
	}

	SetTimer(NULL,0,1000,MyDoOutTimerMsg);

	return TRUE;
}

BOOL CDealData::HasDoneData()
{
	m_mtDonLock.Lock();
	if (m_DoneList.GetCount()>0)
	{
		m_mtDonLock.Unlock();
		return TRUE;
	}
	m_mtDonLock.Unlock();	
	return FALSE;
}

void CDealData::AddWaitListData(SLZData& data)
{
	m_mtWaitLock.Lock();
	data.SetRecvTime(CTime::GetCurrentTime());
	m_WaitList.AddTail(data);
	m_mtWaitLock.Unlock();
}

void CDealData::AddDoingListData(SLZData& data)
{
	m_mtDoingLock.Lock();
	m_doingDataList.AddTail(data);
	m_mtDoingLock.Unlock();
}

void CDealData::AddDoneListData(SLZData& data)
{
	m_mtDonLock.Lock();
	m_DoneList.AddTail(data);
	m_mtDonLock.Unlock();
}


void CALLBACK CDealData::MyDoOutTimerMsg(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	POSITION pos;
	int count = dealdata->m_WaitList.GetCount();
	for (int i=0;i<count;i++)
	{
		SLZData data;

//		dealdata->m_mtWaitLock.Lock();
		pos = dealdata->m_WaitList.FindIndex(i);
		if (!pos)
		{
//			dealdata->m_mtWaitLock.Unlock();
			break;
		}
		data = dealdata->m_WaitList.GetAt(pos);
//		dealdata->m_mtWaitLock.Unlock();

		CTime currenttime = CTime::GetCurrentTime();
		CTime recvtime = 	data.GetRecvTime();
		CTimeSpan tWaitTime(0,0,data.GetWaitTime(),0);//几分钟
		if (currenttime - recvtime >= tWaitTime)//加入正在处理队列
		{
#ifdef _DEBUG
			CString strMsg = _T("MyDoOutTimerMsg: 时间到加入处理队列");
			MyWriteConsole(strMsg);
			CString strData;
			strData.Format(_T("接收到的data为:%s\n"),data.GetSerialId());
			MyWriteConsole(strData);
#endif
//			dealdata->m_mtDoingLock.Lock();
			dealdata->m_doingDataList.AddTail(data);
//			dealdata->m_mtDoingLock.Unlock();

			///呼叫器发声
			dealdata->AlarmToCaller(&data);
			

			int nWaitNum = dealdata->GetDoingDataCount();
	//		theApp.m_pMainWnd->SetWindowText(_T("呼叫终端--当前等待人数：")+strWaitNum);
			
			SLZData* pData = new SLZData;
			*pData = data;
			::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_SHOWMSG,(WPARAM)pData,NULL);

			CString* pWaitText = new CString;
			pWaitText->Format(_T("等待人数:%d"),nWaitNum);
			::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_WAITNUMMSG,(WPARAM)pWaitText,NULL);
	
//			dealdata->m_mtWaitLock.Lock();
			dealdata->m_WaitList.RemoveAt(pos);
//			dealdata->m_mtWaitLock.Unlock();
		}
	
	}


	pos = dealdata->m_doingDataList.GetHeadPosition();
	//	int count = pCallThread->m_pDealData->GetDoingDataCount();
	POSITION poslast;
	SLZData data;
// #ifdef _DEBUG
// 	int num = 0;
// #endif
	while(pos)
	{

		poslast = pos;
		data = dealdata->m_doingDataList.GetNext(pos);
// #ifdef _DEBUG
// 		CString str = data.GetQueueNumber();
// 		str.AppendFormat(_T("          %d"),++num);
// 		MyWriteConsole(str);
// #endif

		CTime currenttime = CTime::GetCurrentTime();
		CTime calltime = 	data.GetCallTime();
		if (currenttime-calltime >= CTimeSpan(0,0,dealdata->m_CallWaitTime,0)  && data.GetIsCalling())
		{
			data.SetIsCalling(FALSE);
			dealdata->m_doingDataList.RemoveAt(poslast);
			dealdata->AddDoingListData(data);
#ifdef _DEBUG
			CString strmsg = _T("呼叫次数到了没有应答,等候时间到，将自动排队到末尾\n");
			MyWriteConsole(strmsg);
			strmsg.Format(_T("data ID为:%s\n"),data.GetSerialId());
			MyWriteConsole(strmsg);
#endif
			break;
		}
	}
}

DWORD WINAPI CDealData::SendToNext(LPVOID pParam)
{
	CDealData* pThis = (CDealData*)pParam;
	while(TRUE)
	{
		if (pThis->HasDoneData())
		{
			pThis->m_mtDonLock.Lock();
			SLZData data;
			if (!CDealData::GetInstance()->m_DoneList.IsEmpty())
			{
				data= CDealData::GetInstance()->m_DoneList.GetHead();
			}
			pThis->m_mtDonLock.Unlock();
			data.SetWaitTime(pThis->m_iWaitTime);
			
			string aSendPacket = CDealPacket::ProduceSendPacket(&data);

#ifdef _DEBUG
			CString wSendPacket(aSendPacket.c_str());
			MyWriteConsole(_T("发往第三级消息:") + wSendPacket);
			MyWriteConsole(_T("\n"));
#endif
		
			if(!pThis->m_strNextIP.IsEmpty())
			{
				

			CComplSocketClient client;
			client.SetRemoteIp(pThis->m_strNextIP);
			CString wSendMsg(aSendPacket.c_str());
			string recvMsg;int actRecvSize = 0;
			if(client.SendData(wSendMsg,recvMsg,actRecvSize))
			{
				pThis->m_mtDonLock.Lock();
				pThis->m_DoneList.RemoveHead();
				pThis->m_mtDonLock.Unlock();
			}
#ifdef _DEBUG
			CString wRecvMsg(recvMsg.c_str());
			MyWriteConsole(_T("发往第三级返回消息:") + wRecvMsg);
#endif
			}
		}
	}
	Sleep(10);
}


BOOL CDealData::ReadWndData()
{
	
	FreeWndData();

	CDoFile doFile;
	CString savePath;
	savePath = doFile.GetExeFullFilePath();
	savePath += _T("\\BasiceSet");
	savePath += _T("\\BasiceSet.dat");


	CFile file;
	if(file.Open(savePath,CFile::modeRead))
	{
		CCallerWnd* pCallerWnd = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pCallerWnd;
				if (pCallerWnd)
				{
					m_list_callerWnd.push_back(pCallerWnd);
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

void CDealData::FreeWndData()
{
	list<CCallerWnd*>::const_iterator itera = m_list_callerWnd.begin();
	for(itera;itera != m_list_callerWnd.end();++itera)
	{
		CCallerWnd* pWnd = *itera;
		if(pWnd)
		{
			delete pWnd;
		}
	}
	m_list_callerWnd.clear();
}
void CDealData::AlarmToCaller(const SLZData* pData)
{
	if(!pData) return;

	SLZCallerData callerdata;
	callerdata.SetCmdType(callerCmdShowAlarm);
	//////加入地址
	list<CCallerWnd*>::const_iterator itera = m_list_callerWnd.begin();
	for(itera; itera != m_list_callerWnd.end(); ++itera)
	{
		CCallerWnd* pWnd = *itera;
		for(int i=0;i<3;i++)
		{
			if(pWnd->m_strCanDoBuss[i] == pData->GetBussName())
			{
				callerdata.SetCallerId(pWnd->GetCallerID());
				SLZCCaller::GetInstance()->AddWriteCallerData(callerdata);
				break;
			}
		}
	}
}

BOOL CDealData::GetDoingFirstData(CallerCmd* callerCmd,SLZData& data)
{
	BOOL flag = FALSE;
	if(!callerCmd)	return flag;

	UINT callerAdd = callerCmd->GetCallerAdd();
	
	CCallerWnd* pCallerWnd = NULL;
	list<CCallerWnd*>::const_iterator itera = m_list_callerWnd.begin();
	for(itera;itera != m_list_callerWnd.end();++itera)
	{
		CCallerWnd* pTempWnd = *itera;
		if(pTempWnd->GetCallerID() == callerAdd)
		{
			pCallerWnd = pTempWnd;
			break;
		}
	}

	if(pCallerWnd == NULL)
	{
		return FALSE;
	}

	POSITION pos = m_doingDataList.GetHeadPosition();
	SLZData temp;
	while(pos)
	{
		temp = m_doingDataList.GetNext(pos);
		for(int i=0;i<3;i++)
		{
			if(temp.GetBussName() == pCallerWnd->m_strCanDoBuss[i])
			{
				flag = TRUE;
				data = temp;
				break;
			}
		}
		if(flag)
		{
			break;
		}
	}
	return flag;
/*
	BOOL isHaveSecBuss = FALSE;
	if(pCallerWnd->m_strCanDoBuss[1]  !=  '\0' || pCallerWnd->m_strCanDoBuss[2] != '\0')
		isHaveSecBuss = TRUE;

//	m_mtDoingLock.Lock();
	if(!isHaveSecBuss)
	{
	POSITION pos = m_doingDataList.GetHeadPosition();
	SLZData temp;
	while(pos)
	{
		 temp = m_doingDataList.GetNext(pos);
		for(int i=0;i<3;i++)
		{
			if(temp.GetBussName() == pCallerWnd->m_strCanDoBuss[i])
			{
				flag = TRUE;
				data = temp;
				break;
			}
		}
		if(flag)
		{
			break;
		}
	}
	}
	else
	{
		POSITION pos = m_doingDataList.GetHeadPosition();
		SLZData temp,maxTemp;
		maxTemp.SetRecvTime(CTime::GetCurrentTime());
		while(pos)
		{
			temp = m_doingDataList.GetNext(pos);
			for(int i=0;i<3;i++)
			{
				if(temp.GetBussName() == pCallerWnd->m_strCanDoBuss[i])
				{
					flag = TRUE;
					maxTemp =  temp.GetRecvTime() < maxTemp.GetRecvTime() ? temp : maxTemp;
					break;
				}
			}

		}
		if(flag)
		{
			data = maxTemp;
		}
	}
//	m_mtDoingLock.Unlock();
	return flag;
	*/
}

int CDealData::GetDoingDataCount()
{
//	m_mtDoingLock.Lock();
	int count = m_doingDataList.GetCount();
//	m_mtDoingLock.Unlock();
	return count;
}

BOOL CDealData::ModifyDoingListData( SLZData& data)
{
	
	BOOL flag = FALSE;
	POSITION pos = m_doingDataList.GetHeadPosition();
	POSITION poslast;
	while(pos)
	{
		poslast = pos;
		SLZData dataTemp = m_doingDataList.GetNext(pos);
		if(dataTemp.GetSerialId() == data.GetSerialId())
		{
			m_mtDoingLock.Lock();
			m_doingDataList.SetAt(poslast,data);
			m_mtDoingLock.Unlock();
			flag = TRUE;
			break;
		}
	}
	
	return flag;
}

BOOL CDealData::IsEmptyDoingListDat()
{
	BOOL flag = FALSE;
//	m_mtDoingLock.Lock();
	flag = m_doingDataList.IsEmpty();
//	m_mtDoingLock.Unlock();
	return flag;
}

void CDealData::RemoveHeadDoingListData()
{
	m_mtDoingLock.Lock();
	m_doingDataList.RemoveHead();
	m_mtDoingLock.Unlock();
}

BOOL CDealData::SaveDoingData()
{
	CFile file;
	if(file.Open(m_doingSaveDataPath,CFile::modeCreate | CFile::modeWrite))
	{
//		m_mtDoingLock.Lock();
		POSITION pos = m_doingDataList.GetHeadPosition();
		while(pos)
		{
			SLZData data =m_doingDataList.GetNext(pos);
			CArchive ar(&file,CArchive::store);
			ar<<&data;
			ar.Close();
		}
//		m_mtDoingLock.Unlock();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CDealData::ReadDoingData()
{
	CFile file;
	if(file.Open(m_doingSaveDataPath,CFile::modeRead))
	{
		SLZData* pData = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pData;
				if (pData)
				{
					AddDoingListData(*pData);
					delete pData;
					pData = NULL;
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CDealData::SaveWaitData()
{
	CFile file;
	if(file.Open(m_waitSaveDataPath,CFile::modeCreate | CFile::modeWrite))
	{
		m_mtWaitLock.Lock();
		POSITION pos = m_WaitList.GetHeadPosition();
		while(pos)
		{
			SLZData data =m_WaitList.GetNext(pos);
			CArchive ar(&file,CArchive::store);
			ar<<&data;
			ar.Close();
		}
		m_mtWaitLock.Unlock();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CDealData::ReadWaitData()
{
	CFile file;
	if(file.Open(m_waitSaveDataPath,CFile::modeRead))
	{
		SLZData* pData = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pData;
				if (pData)
				{
					AddWaitListData(*pData);
					delete pData;
					pData = NULL;
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CDealData::RemoveCallerDoingListData(CallerCmd* callerCmd,SLZData& data)
{
	
	BOOL flag = FALSE;
	SLZData tempData;
	POSITION pos = m_doingDataList.GetHeadPosition();
	POSITION posLast;
	while(pos)
	{
		posLast = pos;
		tempData = m_doingDataList.GetNext(pos);
		if(tempData.GetCallingAdd() == callerCmd->GetCallerAdd() && tempData.GetIsCalling())
		{
			data = tempData;
			flag = TRUE;
			m_mtDoingLock.Lock();
			m_doingDataList.RemoveAt(posLast);
			m_mtDoingLock.Unlock();
			break;
		}
	}
	
	return flag;
}