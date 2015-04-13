#pragma once
#include "UserInfo.h"

// CChangePassWordDlg 对话框

class CChangePassWordDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangePassWordDlg)

public:
	CChangePassWordDlg(CWnd* pParent = NULL,CString strUserName=_T(""));   // 标准构造函数
	virtual ~CChangePassWordDlg();

// 对话框数据
	enum { IDD = IDD_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog(); 
	DECLARE_MESSAGE_MAP()
public:

private:
	CString m_strOldPassWord;
	CString m_strNewWord;
	CString m_strNewPassWord2;
	CString m_strUserName;
	CString m_strUserInfoFilePath;
	CUserInfo m_UserInfo;
	int m_index;
	CMap<int,int,CUserInfo,CUserInfo> m_UserInfoMap;
	BOOL WriteUserInfoIntoFiles();
	BOOL ReadUserInfoFromFiles();
public:
	afx_msg void OnBnClickedOk2();
};
