#pragma once
#include "DataDef.h"

// CComSetDlg 对话框

class CComSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CComSetDlg)

public:
	CComSetDlg(CWnd* pParent = NULL,ComInfo cominfo);   // 标准构造函数
	virtual ~CComSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	ComInfo m_cominfo;
};
