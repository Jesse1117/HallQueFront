// ChangePassWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "ChangePassWordDlg.h"


// CChangePassWordDlg 对话框

IMPLEMENT_DYNAMIC(CChangePassWordDlg, CDialog)

CChangePassWordDlg::CChangePassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePassWordDlg::IDD, pParent)
	, m_strOldPassWord(_T(""))
	, m_strNewWord(_T(""))
	, m_strNewPassWord2(_T(""))
{

}

CChangePassWordDlg::~CChangePassWordDlg()
{
}

void CChangePassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_OLDPASSWORD, m_strOldPassWord);
	DDX_Text(pDX, IDC_EDIT_NEWPASSWORD, m_strNewWord);
	DDX_Text(pDX, IDC_EDIT_NEWPASSWORD2, m_strNewPassWord2);
}


BEGIN_MESSAGE_MAP(CChangePassWordDlg, CDialog)

END_MESSAGE_MAP()


// CChangePassWordDlg 消息处理程序

