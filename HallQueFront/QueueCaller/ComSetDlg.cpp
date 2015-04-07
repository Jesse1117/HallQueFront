// ComSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "ComSetDlg.h"


// CComSetDlg 对话框

IMPLEMENT_DYNAMIC(CComSetDlg, CDialog)

CComSetDlg::CComSetDlg(CWnd* pParent /*=NULL*/,ComInfo info)
	: CDialog(CComSetDlg::IDD, pParent)
	,m_cominfo(info);
{

}

CComSetDlg::~CComSetDlg()
{
}

void CComSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CComSetDlg, CDialog)
END_MESSAGE_MAP()


// CComSetDlg 消息处理程序
