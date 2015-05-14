// QueueClientDlg.h : ͷ�ļ�
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

// CQueueClientDlg �Ի���
class CQueueClientDlg : public CDialog
{
// ����
public:
	CQueueClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QUEUECLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
//	afx_msg LRESULT OnMyShowMessage(WPARAM wParam, LPARAM lParam);//���ܽ�����ʾȱֽ��Ϣ����
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
