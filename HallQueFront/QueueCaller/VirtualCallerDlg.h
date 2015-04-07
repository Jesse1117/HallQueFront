#pragma once
#include "CallerInfo.h"

// CVirtualCallerDlg �Ի���

class CVirtualCallerDlg : public CDialog
{
	DECLARE_DYNAMIC(CVirtualCallerDlg)

public:
	CVirtualCallerDlg(CWnd* pParent = NULL,CString strUserName=_T(""));   // ��׼���캯��
	virtual ~CVirtualCallerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CALLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();

private:
	CString m_strUserName;
	CallerInfo m_info;

	BOOL ReadCallerInfoFromFile();
	BOOL WriteCallerInfoIntoFile();
public:
	afx_msg void On32771();

	afx_msg void On32772();
	afx_msg void On32773();
	afx_msg void On32774();
	afx_msg void On32775();
};
