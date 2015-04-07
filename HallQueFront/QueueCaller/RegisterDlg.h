#pragma once
#include "UserInfo.h"

// CRegisterDlg 对话框

class CRegisterDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterDlg();

// 对话框数据
	enum { IDD = IDD_REGISTERDLG };
	afx_msg void OnBnClickedIDOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CString m_strUserName;
	CString m_strPassWord;
	CString m_strPassWord2;
	CString m_strUserInfoFilePath;
	CMap<int,int,CUserInfo,CUserInfo> m_UserInfoMap;
	BOOL WriteUserInfoIntoFiles();
	BOOL ReadUserInfoFromFiles();
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
