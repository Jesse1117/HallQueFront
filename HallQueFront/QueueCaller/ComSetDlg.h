#pragma once
#include "DataDef.h"
#include "afxwin.h"
// CComSetDlg 对话框
#define WM_MY_TRAYICON (WM_USER+100)
class CTcpSever;
class CSoundPlay;
//class CDoComInOut;
class CComSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CComSetDlg)

public:
	CComSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	CString m_strIP;
	CString m_strPort;
	CString m_strWndId;
	UINT m_iWndId;
	CString m_strCom;
	CString m_strCallName;
	UINT m_iWaitTime;
	CString m_strWatiTime;
	UINT m_iPlayTimes;
	CString m_strPlayTimes;
	CString m_strCallPath;
	CString m_strSelfPort;
private:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
private:
	void AddTrayIcon(void);
	BOOL TaskBarAddIcon(HWND hwnd, UINT uID, HICON hIcon, LPCWSTR lpszTip);
	BOOL TaskBarDeleteIcon(HWND hwnd, UINT uID);
	void RemoveTrayIcon();
	void LoadInfo();
	CTcpSever* m_pSever;
	CSoundPlay* m_pPlaySound;
	//CDoComInOut* m_pComInOut;
public:
	afx_msg void OnCallset();
	afx_msg void OnShowset();
	afx_msg void OnComset();
	afx_msg void OnQuit();
};
