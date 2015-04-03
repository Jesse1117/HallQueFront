// MainFrm.h : CMainFrame ��Ľӿ�
//
#pragma once
//#include "HardWareCaller.h"
#include "DataDef.h"
#define  WM_MY_TRAYICON (WM_USER+2049)

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//ȫ��
	void FullScreen();
// ʵ��
public:
	virtual ~CMainFrame();
	//�˳�ȫ��
	void EndFullScreen();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;

private:
//	WINDOWPLACEMENT m_oldWindowPlaceMent;
	//���ڴ���ȫ����BOOL����
	BOOL m_bFullScreen;
	CRect m_FullScreenRect;
	CRect m_windowRect;
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	//��������ico imagelist
	CImageList m_imageList;
	CToolBar    m_wndToolBar;
	/////�ؼ�����롢�Ҷ����һϵ�в���
	afx_msg void OnIconLeft();
	afx_msg void OnIconMiddle();
	afx_msg void OnIconRight();
	afx_msg void OnIconWidth();
	afx_msg void OnIconHeight();
	afx_msg void OnIconSameSize();
	afx_msg void OnIconTop();
	afx_msg void OnIconBottom();
	afx_msg void OnIconHor();
	afx_msg void OnIconVer();
	/////////////////////////////////////////
private:
	CRect m_wndToolBarRect;
	//Ӳ������������Ӳ����������ʵ��������
	//��Ϊ���к�����Ϣ����ʱ������Ӧ��ȥ��������
	//���䴦������������������
//	CHardWareCaller m_hardCaller;
protected:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	BOOL ShutDown();
	BOOL DeleteLogFile();
private:
	//��������
	void AddTrayIcon(void);
	BOOL TaskBarAddIcon(HWND hwnd, UINT uID, HICON hIcon, LPCWSTR lpszTip);
	BOOL TaskBarDeleteIcon(HWND hwnd, UINT uID);
	void RemoveTrayIcon();
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTipShow();
	afx_msg void OnTipExit();
	void SetFullScreen(BOOL bFullScreen){m_bFullScreen = bFullScreen;}
	virtual BOOL DestroyWindow();
};


