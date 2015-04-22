#include "StdAfx.h"
#include "FinshQueData.h"
#include "ComplSocketClient.h"
#include "HallQueFront.h"
#include "ComplSocketClient.h"
#include "ProducePacket.h"
#include "ShortMsgModem.h"
#include "CreatePacket.h"
#include "DoFile.h"
#include "CommonStrMethod.h"

CFinshQueData::CFinshQueData(void) : m_pDoFinshedDataThread(NULL)
{
	CDoFile doFile;
	m_filePath = doFile.GetExeFullFilePath();
	m_filePath += _T("\\JudgeShortMsg");
	m_filePath += _T("\\JudgeShortMsg.dat");
	m_strLogFilePath = CommonStrMethod::GetModuleDir() + _T("log/");
	ReadJudgeShortMsg();
	Start();
}

CFinshQueData::~CFinshQueData(void)
{
	if(m_pDoFinshedDataThread)
	{
//  		while(TRUE)
//  		{
//  			if(m_listFinshQue.size()==0)
//  			{
//  				break;
//  			}
// 			WaitForSingleObject(m_pDoFinshedDataThread->m_hThread,1000);
//  		}
		TerminateThread(m_pDoFinshedDataThread->m_hThread,0);
		delete m_pDoFinshedDataThread;
		m_pDoFinshedDataThread = NULL;
	}
	ReleaseListShorMsg();
}

void CFinshQueData::Add(const SLZData& data)
{
	m_mtFinshQue.Lock();
	m_listFinshQue.push_back(data);
	m_mtFinshQue.Unlock();
}

BOOL CFinshQueData::GetFinshedData(const UINT winID,SLZData& data)
{
	BOOL flag = FALSE;
	m_mtFinshQue.Lock();
	std::list<SLZData>::const_iterator itera = m_listFinshQue.begin();
	for(itera;itera!=m_listFinshQue.end();itera++)
	{
		if(itera->GetWindowId() == winID)
		{
			data = *itera;
			m_listFinshQue.erase(itera);
			flag = TRUE;
			break;
		}
	}
	m_mtFinshQue.Unlock();
	return flag;
}

BOOL CFinshQueData::GetFinshedData()
{
	SLZData data;
	m_mtFinshQue.Lock();
	std::list<SLZData>::const_iterator itera = m_listFinshQue.begin();
	data = *itera;
	m_listFinshQue.pop_front();
	BOOL flag = FALSE;
	m_mtFinshQue.Unlock();
	///////////发送到服务端//////////////
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		CComplSocketClient client;
		CProducePacket packet;
 		int leftNum = theApp.m_Controller.m_pInlineQueData->GetBussCount(data.GetBussinessType());
 		CString sendPacket = packet.ProduceSendPacket(data,leftNum,evateNumData);
 		std::string recvMsg;
 		int actRecvSize = 0;
 		flag = client.SendData(sendPacket,recvMsg,actRecvSize); 
 		int errCode = packet.JudgePacketRet(recvMsg);
 		if(errCode!=1) flag=FALSE; 
	}
	else
	{
		CComplSocketClient client;
		CCreatePacket packet;
		data.SetWaitTime(theApp.m_logicVariables.iWaitTime);
		CString sendmsg = packet.ProducePacket(data);
		std::string recvMsg;
		int actRecvSize = 0;
		flag = client.SendData(sendmsg,recvMsg,actRecvSize); 
		if (flag)
		{
			WriteLogWithTime(_T("send:")+sendmsg);
		}
		else WriteErrLog(_T("发送到下一级失败"));
	}
	
	///////////出现差评时发送短信到手机//////////////
	if(theApp.m_logicVariables.IsOpenJudgeShortMsg)
	{
		if(data.GetEvaluateLevel() == evaBad)
		{
			//发送短信
			SendMsgToPhone(data);
		}	
	}
	return flag;
}

BOOL CFinshQueData::Start()
{
	m_pDoFinshedDataThread = AfxBeginThread(DoFinshedData,this,0,0,0,NULL);
	if(m_pDoFinshedDataThread)
		return TRUE;
	else
		return FALSE;
}

UINT CFinshQueData::DoFinshedData(LPVOID pParam)
{
	CFinshQueData* pThis = (CFinshQueData*)pParam;
	while(TRUE)
	{
		if(pThis->m_listFinshQue.size()>0)
		{
			pThis->GetFinshedData();
		}
		else
		{
			Sleep(10);
		}
	}
	return 0;
}

void CFinshQueData::ReleaseListShorMsg()
{
	list<CJudgeShortMsg*>::const_iterator itera = m_list_shortmsg.begin();
	for(itera;itera!=m_list_shortmsg.end();itera++)
	{
		CJudgeShortMsg* pMsg = *itera;
		delete pMsg;
		pMsg = NULL;
	}
	m_list_shortmsg.clear();
}

BOOL CFinshQueData::ReadJudgeShortMsg()
{
	ReleaseListShorMsg();//先释放
	CFile file;
	CFileException e;
	if (file.Open(m_filePath,CFile::modeRead,&e))
	{
		////注意序列化出来的指针变量已经分配了内存
		CJudgeShortMsg* pJudgeShortMsg = NULL;
		UINT cardConfigID = 0;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
		{
			do
			{
				ar>>pJudgeShortMsg;
				if (pJudgeShortMsg)
				{
					m_list_shortmsg.push_back(pJudgeShortMsg);
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
	}
	else return FALSE;
	return TRUE;
}

BOOL CFinshQueData::ReFlushListShortMsg()
{
	return ReadJudgeShortMsg();
}

BOOL CFinshQueData::SendMsgToPhone(const SLZData& data)
{
	BOOL flag = FALSE;
	list<CJudgeShortMsg*>::const_iterator itera = m_list_shortmsg.begin();
	for(itera;itera!=m_list_shortmsg.end();itera++)
	{
		CJudgeShortMsg* pMsg = *itera;
		CString strShortMsg = pMsg->GetShortMsg();
		UINT winddowID = data.GetWindowId();
		CString strWindowID = theApp.m_Controller.GetWindowNameByID(winddowID);
//		strWindowID.Format(_T("%d号窗口"),winddowID);
		strShortMsg.Replace(_T("[窗口号]"),strWindowID);
		CString staffName = theApp.m_Controller.GetStaffNameByID(data.GetStaffId());
		strShortMsg.Replace(_T("[员工姓名]"),staffName);
		strShortMsg.Replace(_T("[员工工号]"),data.GetStaffId());
		CShortMsgModem* pMsgModem = CShortMsgModem::GetInstance();
		pMsgModem->ClearSendBox();
		flag = pMsgModem->SendMsg(pMsg->GetPhoneNum(),strShortMsg);
	}
	return flag;
}

void CFinshQueData::WriteErrLog(CString strSockLog)
{
	CString str = _T("Error: ") + strSockLog;
	WriteLogWithTime(str);
}

void CFinshQueData::WriteLogWithTime(CString strSockLog)
{
	CTime time = CTime::GetCurrentTime();
	CString str = time.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("  \t") + strSockLog + _T("\r\n\r\n");
	CString strLogFile = time.Format(_T("log_%Y%m%d.log"));
	if(!CommonStrMethod::PathFileExist(m_strLogFilePath))
	{
		if(!CommonStrMethod::CreatePath(m_strLogFilePath))
		{
			return;
		}
	}
	AppendWriteFile(str, m_strLogFilePath + strLogFile);
}

BOOL CFinshQueData::AppendWriteFile(CString strText, CString strFileName)
{
	CFile file;
	if(!file.Open(strFileName, 
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		//CString str;
		//str.Format(_T("创建或打开日志文件失败,您所使用的计算机帐号没有相应的磁盘写权限:\r\n\t%s"), g_pControl->m_strLogFilePath);
		//AfxMessageBox(str);
		return FALSE;
	}
	CHAR* szBuf = (CHAR*)malloc(strText.GetLength()*2 + 1);
	memset(szBuf, 0, strText.GetLength()*2 + 1);
	CommonStrMethod::WChar2Char(szBuf, 
		strText.GetLength()*2 + 1, strText.GetBuffer());
	file.SeekToEnd();
	file.Write(szBuf, strlen(szBuf));
	file.Close();

	return TRUE;
}