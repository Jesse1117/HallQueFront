#pragma once
#include "HallQueFront.h"
class SLZWindow;
// CScreenAddSetDlg 对话框
#define  WNDSCRADD  _T("窗口屏地址")
#define  COMSCRADD  _T("综合屏地址")
#define  THROUGHADD    _T("通屏IP地址")
#define  THROUGHPHY  _T("通屏物理地址")

class CScreenAddSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenAddSetDlg)

public:
	CScreenAddSetDlg(SLZWindow& windowinfo,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenAddSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };
	SLZWindow m_WindowInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CListBox m_list_AddType;
	CListBox m_list_AddNum;
	CString m_strAddNUm;
	CArray m_wndarray;
	CArray m_comarray;
	CArray m_phyarray;
	CStringArray m_strIParray;
	int m_cursel;
public:
	afx_msg void OnLbnSelchangeListAddtype();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
