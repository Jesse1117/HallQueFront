// VirtualCallerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "VirtualCallerDlg.h"
#include "CallJttsSetDlg.h"
#include "ShowMsgSetDlg.h"
#include "ComSetDlg.h"
// CVirtualCallerDlg 对话框

IMPLEMENT_DYNAMIC(CVirtualCallerDlg, CDialog)

CVirtualCallerDlg::CVirtualCallerDlg(CWnd* pParent /*=NULL*/,CString strUserName)
	: CDialog(CVirtualCallerDlg::IDD, pParent)
,m_strUserName(strUserName)
{

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
	
	return TRUE;
}

void CVirtualCallerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加命令处理程序代码
	CString strCallMsg;
	CCallJttsSetDlg SoundDlg(strCallMsg);
	SoundDlg.DoModal();
}
/*呼叫显示设置*/
void CVirtualCallerDlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	//pShowInfo *info=new pShowInfo();
	//CShowMsgSetDlg ShowDlg(*info,this);
	//ShowDlg.DoModal();
}

void CVirtualCallerDlg::On32775()
{
	// TODO: 在此添加命令处理程序代码

	/*CComSetDlg ComSetDlg(this,)*/
}
