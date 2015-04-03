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
END_MESSAGE_MAP()


// CVirtualCallerDlg 消息处理程序
