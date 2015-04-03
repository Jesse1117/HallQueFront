// QueueCallerDlg.h : 头文件
//

#pragma once
#include "DataDef.h"

// CQueueCallerDlg 对话框
class CQueueCallerDlg : public CDialog
{
// 构造
public:
	CQueueCallerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QUEUECALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRegister();

private:
	BOOL ReadUserFileFromFiles();
public:
	afx_msg void OnBnClickedButtonFindpassword();
	afx_msg void OnBnClickedCheckRemember();
	afx_msg void OnBnClickedCheckAutologin();
	afx_msg void OnCbnSelchangeComboUsername();

private:
	CString m_strPassWord;
	CString m_strUserName;
	CString m_strUserInfoFilePath;
	CComboBox m_cs_ComUserInfo;
	CMap<int,int,UserInfo,UserInfo> m_UserInfoMap;
};
