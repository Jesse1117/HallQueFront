#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CallerWnd.h"
#include <map>
#include "../HallQueFront/ListCtrlCl.h"
using namespace std;

// CSetDialog 对话框
class CComInit;
class CShortMsgModem;

class CSetDialog : public CDialog
{
	DECLARE_DYNAMIC(CSetDialog)

public:
	CSetDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlCl m_listctr_wnd;
	CString m_ed_wndName;
	CString m_ed_wndAdd;
	CString m_ed_callerAdd;
	CString m_ed_doBuss1;
	CString m_ed_doBuss2;
	CString m_ed_doBuss3;
	CEdit m_ed_nextIP;
	CComboBox m_combo_wnd;
	CComboBox m_combo_shortmsg;
	CEdit m_ed_callWaitTime;
	CEdit m_ed_sendWaitTime;
	CEdit m_ed_callNum;
	CButton m_check_shotrmsg;
	CEdit m_ed_shortmsg;
	virtual BOOL OnInitDialog();

private:
	map<int,CCallerWnd*> m_map_callerWnd;
	int m_nCurrSelectItem;
public:
	afx_msg void OnBnClickedBnAdd();
	afx_msg void OnBnClickedBnDelete();
	afx_msg void OnLvnItemchangedListWnd(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void ShowCallerWndMsg();
	BOOL WriteBasicSetMsg();
	BOOL ReadBasicSetMsg();
	void AddDataToListctr(CCallerWnd* pCallerWnd);
	void ReleaseAllCallerWnd();
public:
	afx_msg void OnEnChangeEditWndname();
	afx_msg void OnEnChangeEditWndid();
	afx_msg void OnEnChangeEditCallerid();
	afx_msg void OnEnChangeEditDobuss1();
	afx_msg void OnEnChangeEditDobuss2();
	afx_msg void OnEnChangeEditDobuss3();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboCaller();
	afx_msg void OnCbnSelchangeComboShortmsg();
private:
	CComInit* m_pCommInit;
	CShortMsgModem* m_pShortMsg;
	CString m_strSavePath;
};
