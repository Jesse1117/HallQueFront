#pragma once
#include "HallQueFront.h"
class SLZWindow;
// CScreenAddSetDlg �Ի���
#define  WNDSCRADD  _T("��������ַ")
#define  COMSCRADD  _T("�ۺ�����ַ")
#define  THROUGHADD    _T("ͨ��IP��ַ")
#define  THROUGHPHY  _T("ͨ�������ַ")

class CScreenAddSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenAddSetDlg)

public:
	CScreenAddSetDlg(SLZWindow& windowinfo,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScreenAddSetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };
	SLZWindow m_WindowInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
