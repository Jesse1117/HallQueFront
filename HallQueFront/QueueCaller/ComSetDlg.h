#pragma once
#include "DataDef.h"

// CComSetDlg �Ի���

class CComSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CComSetDlg)

public:
	CComSetDlg(CWnd* pParent = NULL,ComInfo cominfo);   // ��׼���캯��
	virtual ~CComSetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	ComInfo m_cominfo;
};
