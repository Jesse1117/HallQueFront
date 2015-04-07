#pragma once
#include "resource.h"

// CCallJttsSetDlg �Ի���
//���ʹ��JTTS ��������������öԻ���

class CCallJttsSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CCallJttsSetDlg)

public:
	CCallJttsSetDlg(CString& strConf,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCallJttsSetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_JTTSCALLSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CString& m_StrConf;
	CListBox m_lbList;
	CListBox m_lbWavLib;
	CButton m_check_mkjttsall;
	CButton m_Sel_CommonChinese;
	CButton m_Sel_English;
public:
	afx_msg void OnBnClickedButtonQuenum();
	afx_msg void OnBnClickedButtonWndname();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonMvup();
	afx_msg void OnBnClickedButtonMvdown();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedOk();
	
	BOOL m_bMkJttsAll;
	afx_msg void OnBnClickedCheckMkjttsall();
	afx_msg void OnBnClickedButtonBusname();
};
