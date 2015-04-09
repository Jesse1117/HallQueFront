// VirtualCallerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "VirtualCallerDlg.h"
#include "CallJttsSetDlg.h"
#include "ShowMsgSetDlg.h"
#include "ComSetDlg.h"
#include "CommonStrMethod.h"
// CVirtualCallerDlg 对话框

IMPLEMENT_DYNAMIC(CVirtualCallerDlg, CDialog)

CVirtualCallerDlg::CVirtualCallerDlg(CWnd* pParent /*=NULL*/,CString strUserName)
	: CDialog(CVirtualCallerDlg::IDD, pParent)
,m_strUserName(strUserName)
,m_index(0)
{
	CommonStrMethod convert;
	m_strCallerInfoPath = convert.GetModuleDir()+_T("CallerInfo\\");
	convert.CreatePath(m_strCallerInfoPath);
	m_strCallerInfoPath+=_T("CallerInfo.dat");
}

CVirtualCallerDlg::~CVirtualCallerDlg()
{
	
}

void CVirtualCallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVirtualCallerDlg, CDialog)
	ON_WM_CLOSE()
	ON_COMMAND(ID_32771, &CVirtualCallerDlg::On32771)
	ON_COMMAND(ID_32772, &CVirtualCallerDlg::On32772)
	ON_COMMAND(ID_32773, &CVirtualCallerDlg::On32773)
	ON_COMMAND(ID_32774, &CVirtualCallerDlg::On32774)
	ON_COMMAND(ID_32775, &CVirtualCallerDlg::On32775)
END_MESSAGE_MAP()


// CVirtualCallerDlg 消息处理程序

BOOL CVirtualCallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->GetParent()->ShowWindow(SW_HIDE);
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	CRect rect;
	this->GetWindowRect(rect);
	this->SetWindowPos(NULL,nFullWidth-rect.Width()-nFullWidth/10,(nFullHeight-rect.Height())/2,rect.Height(),rect.Width(),SWP_NOSIZE);
	SetWindowText(_T("呼叫终端   当前用户：")+m_strUserName);
	if (ReadCallerInfoFromFile())
	{
		for (int i=0;i<m_CallerInfoMap.GetCount();i++)
		{
			CallerInfo info;
			m_CallerInfoMap.Lookup(i,info);
			if (m_strUserName==info.GetUserName())
			{
				m_info = info;
				m_index = i;
				break;
			}
		}
	}
	return TRUE;
}

void CVirtualCallerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_info.GetUserName()==_T(""))
	{
		m_info.SetUserName(m_strUserName);
		m_info.SetCallName(m_strUserName);
		m_CallerInfoMap.SetAt(m_index,m_info);
	}
	WriteCallerInfoIntoFile();
	this->GetParent()->DestroyWindow();
	CDialog::OnClose();
}

void CVirtualCallerDlg::On32771()
{
	// TODO: 在此添加命令处理程序代码
	GetParent()->ShowWindow(SW_SHOWNORMAL);
	this->DestroyWindow();
}

void CVirtualCallerDlg::On32772()
{
	// TODO: 在此添加命令处理程序代码
	OnClose();
}

/*呼叫语音设置*/
void CVirtualCallerDlg::On32773()
{
	// TODO: 在此添加命令处理程序代
	CCallJttsSetDlg SoundDlg(m_info.GetCallMsg(),this);
	if (SoundDlg.DoModal()==IDOK)
	{
		m_info.SetCallMsg(SoundDlg.m_StrConf);
		m_CallerInfoMap.SetAt(m_index,m_info);
	}
	
}
/*呼叫显示设置*/
void CVirtualCallerDlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	pShowInfo info=new ShowInfo();
	info->iShowTime=m_info.GetMsgShowTime();
	info->strAdMsg = m_info.GetAdMsg();
	info->strShowMsg = m_info.GetShowMsg();
	CShowMsgSetDlg ShowDlg(info,this);
	
	if (ShowDlg.DoModal()==IDOK)
	{
		m_info.SetMsgShowTime(ShowDlg.m_iShowTime);
		m_info.SetAdMsg(ShowDlg.m_strAdMsg);
		m_info.SetShowMsg(ShowDlg.m_strConf);
		m_CallerInfoMap.SetAt(m_index,m_info);
	}	
}

void CVirtualCallerDlg::On32775()
{
	// TODO: 在此添加命令处理程序代码
	pComInfo info = new ComInfo();
	info->strIP = m_info.GetStrIP();
	info->strPort = m_info.GetStrPort();
	info->iWndId = m_info.GetWndScreenId();
	info->strCallName = m_info.GetCallName();
	info->iWatiTime = m_info.GetWatiTime();
	info->iPlayTimes = m_info.GetPlayTimes();
	CComSetDlg ComSetDlg(info,this);
	if (ComSetDlg.DoModal()==IDOK)
	{
		m_info.SetStrIP(ComSetDlg.m_strIP);
		m_info.SetStrPort(ComSetDlg.m_strPort);
		m_info.SetWndScreenId(ComSetDlg.m_iWndId);
		m_info.SetCallName(ComSetDlg.m_strCallName);
		m_info.SetWaitTime(ComSetDlg.m_iWaitTime);
		m_info.SetPlayTimes(ComSetDlg.m_iPlayTimes);
		m_CallerInfoMap.SetAt(m_index,m_info);
	}
}

BOOL CVirtualCallerDlg::ReadCallerInfoFromFile()
{
	m_CallerInfoMap.RemoveAll();
	CFile file;
	CFileException e;
	if (file.Open(m_strCallerInfoPath,CFile::modeRead,&e))
	{
		CallerInfo* callerinfo;
		CArchive ar(&file,CArchive::load);
		int index=0;
		if (file.GetLength()) 
			do
			{
				ar>>callerinfo;
				if (callerinfo)
				{
					m_CallerInfoMap[index]
					= *callerinfo;
					delete callerinfo;
					callerinfo = NULL;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
			return TRUE;
	}
	else return FALSE;
	return TRUE;
}

BOOL CVirtualCallerDlg::WriteCallerInfoIntoFile()
{
	CFile file;
	CFileException e;
	if (file.Open(m_strCallerInfoPath,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count = m_CallerInfoMap.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			CallerInfo callerrinfo;
			m_CallerInfoMap.Lookup(i,callerrinfo);
			ar<<&callerrinfo;
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	else
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
}