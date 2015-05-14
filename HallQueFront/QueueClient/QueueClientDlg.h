// QueueClientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "../HallQueFront/ListCtrlCl.h"
#include <map>
using namespace std;


class CComInit;
class CShortMsgModem;
class CCallThread;
class CSoundPlay;
class CSelectSocketServer;
class SLZData;

// CQueueClientDlg 对话框
class CQueueClientDlg : public CDialog
{
// 构造
public:
	CQueueClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QUEUECLIENT_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedListQueue(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg LRESULT OnShowWaitNum(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnShowAddMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnShowCallMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnShowDoneMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnShowNormalMsg(WPARAM wParam,LPARAM lParam);
//	afx_msg LRESULT OnMyShowMessage(WPARAM wParam, LPARAM lParam);//接受界面显示缺纸消息函数
private:
	int m_nCurrSlectItem;
	CComInit* m_pComInit;
	CShortMsgModem* m_pShortMsg;
	void OpenAllComm();
	CSoundPlay* m_pPlaySound;
	CSelectSocketServer* m_pServer;
	CCallThread* m_pCallThread;
	map<int,SLZData*> m_map_data;
public:
	CListCtrlCl m_listctr_que;
	CStatic m_sta_waitNum;
	virtual BOOL DestroyWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
