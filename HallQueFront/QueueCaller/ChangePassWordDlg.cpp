// ChangePassWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "ChangePassWordDlg.h"
#include "CommonStrMethod.h"

// CChangePassWordDlg 对话框

IMPLEMENT_DYNAMIC(CChangePassWordDlg, CDialog)

CChangePassWordDlg::CChangePassWordDlg(CWnd* pParent /*=NULL*/,CString strUserName)
	: CDialog(CChangePassWordDlg::IDD, pParent)
	, m_strOldPassWord(_T(""))
	, m_strNewWord(_T(""))
	, m_strNewPassWord2(_T(""))
	,m_strUserName(strUserName)
{
	CommonStrMethod convert;
	m_strUserInfoFilePath = convert.GetModuleDir()+_T("UserInfo\\");
	convert.CreatePath(m_strUserInfoFilePath);
	m_strUserInfoFilePath+=_T("UserInfo.dat");
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
	
	ON_BN_CLICKED(IDOK2, &CChangePassWordDlg::OnBnClickedOk2)
END_MESSAGE_MAP()


// CChangePassWordDlg 消息处理程序

BOOL CChangePassWordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ReadUserInfoFromFiles();
	for (int i=0;i<m_UserInfoMap.GetCount();i++)
	{
		CUserInfo userinfo;
		m_UserInfoMap.Lookup(i,userinfo);
		if (m_strUserName==userinfo.GetUserName())
		{
			m_UserInfo = userinfo;
			m_index =i;
			break;
		}
	}
	return TRUE;
}

void CChangePassWordDlg::OnBnClickedOk2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_strOldPassWord!=m_UserInfo.GetPassWord())
	{
		AfxMessageBox(_T("原密码输入错误"));
		return;
	}
	if (m_strNewWord!=m_strNewPassWord2)
	{
		AfxMessageBox(_T("两次输入密码不一致"));
		return;
	}
	m_UserInfo.SetPassWord(m_strNewPassWord2);
	m_UserInfoMap.SetAt(m_index,m_UserInfo);
	WriteUserInfoIntoFiles();
	return CDialog::OnOK();
}

BOOL CChangePassWordDlg::ReadUserInfoFromFiles()
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

BOOL CChangePassWordDlg::WriteUserInfoIntoFiles()
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