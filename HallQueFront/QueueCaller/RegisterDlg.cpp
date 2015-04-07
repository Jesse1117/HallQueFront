// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "RegisterDlg.h"
#include "CommonStrMethod.h"

// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialog)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDlg::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPassWord(_T(""))
	, m_strPassWord2(_T(""))
{
	CommonStrMethod convert;
	m_strUserInfoFilePath = convert.GetModuleDir()+_T("UserInfo\\");
	convert.CreatePath(m_strUserInfoFilePath);
	m_strUserInfoFilePath+=_T("UserInfo.dat");
}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD1, m_strPassWord);
	DDX_Text(pDX, IDC_EDIT_PASSWORD2, m_strPassWord2);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRegisterDlg::OnBnClickedIDOK)
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序

BOOL CRegisterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ReadUserInfoFromFiles();
	return TRUE;
}


BOOL CRegisterDlg::ReadUserInfoFromFiles()
{
	m_UserInfoMap.RemoveAll();
	CFile file;
	CFileException e;
	if (file.Open(m_strUserInfoFilePath,CFile::modeRead,&e))
	{
		CUserInfo* userinfo=NULL;
		CArchive ar(&file,CArchive::load);
		int index=0;
		if (file.GetLength()) 
			do
			{
				ar>>userinfo;
				if (userinfo)
				{
					m_UserInfoMap[index]
					= *userinfo;
					delete userinfo;
					userinfo = NULL;
					index++;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
			return TRUE;
	}
	else return FALSE;
}

BOOL CRegisterDlg::WriteUserInfoIntoFiles()
{
	CFile file;
	CFileException e;
	if (file.Open(m_strUserInfoFilePath,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count = m_UserInfoMap.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			CUserInfo userinfo;
			m_UserInfoMap.Lookup(i,userinfo);
			ar<<&userinfo;
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

void CRegisterDlg::OnBnClickedIDOK()
{
	UpdateData();
	if (m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空"));
		return;
	}
	for (int i=0;i<m_UserInfoMap.GetCount();i++)
	{
		CUserInfo userinfo;
		m_UserInfoMap.Lookup(i,userinfo);
		if (m_strUserName == userinfo.GetUserName())
		{
			AfxMessageBox(_T("用户名已经存在"));
			return;
		}
	}
	if (m_strPassWord!=m_strPassWord2)
	{
		AfxMessageBox(_T("两次密码输入不一致"));
		return;
	}
	CUserInfo userinfo;
	userinfo.SetUserName(m_strUserName);
	userinfo.SetPassWord(m_strPassWord);
	m_UserInfoMap.SetAt(m_UserInfoMap.GetCount(),userinfo);
	WriteUserInfoIntoFiles();
	return CDialog::OnOK();
}
//void CRegisterDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialog::OnTimer(nIDEvent);
//}
