// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "RegisterDlg.h"
#include "CommonConvert.h"

// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialog)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDlg::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPassWord(_T(""))
	, m_strPassWord2(_T(""))
{
	CCommonConvert convert;
	m_strUserInfoFilePath = convert.GetExeFullFilePath();
	m_strUserInfoFilePath+=_T("\\UserInfo\\UserInfo.dat");
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
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序

BOOL CRegisterDlg::ReadUserInfoFromFiles()
{
	CFile file;
	CFileException e;
	if (file.Open(m_strUserInfoFilePath,CFile::modeRead,&e))
	{
		UserInfo* userinfo=NULL;
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
			UserInfo userinfo;
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