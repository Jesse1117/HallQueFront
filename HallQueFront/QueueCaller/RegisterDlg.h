#pragma once
#include "DataDef.h"

// CRegisterDlg �Ի���

class CRegisterDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterDlg();

// �Ի�������
	enum { IDD = IDD_REGISTERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_strUserName;
	CString m_strPassWord;
	CString m_strPassWord2;
	CString m_strUserInfoFilePath;
	CMap<int,int,UserInfo,UserInfo> m_UserInfoMap;
	BOOL WriteUserInfoIntoFiles();
	BOOL ReadUserInfoFromFiles();
};
