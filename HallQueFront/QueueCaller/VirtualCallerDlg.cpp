// VirtualCallerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "VirtualCallerDlg.h"


// CVirtualCallerDlg 对话框

IMPLEMENT_DYNAMIC(CVirtualCallerDlg, CDialog)

CVirtualCallerDlg::CVirtualCallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualCallerDlg::IDD, pParent)
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
END_MESSAGE_MAP()


// CVirtualCallerDlg 消息处理程序

BOOL CVirtualCallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	CRect rect;
	this->GetWindowRect(rect);
	this->SetWindowPos(NULL,nFullWidth-rect.Width()-nFullWidth/10,(nFullHeight-rect.Height())/2,rect.Height(),rect.Width(),SWP_NOSIZE);
	return TRUE;
}

void CVirtualCallerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->GetParent()->DestroyWindow();
	CDialog::OnClose();
}
