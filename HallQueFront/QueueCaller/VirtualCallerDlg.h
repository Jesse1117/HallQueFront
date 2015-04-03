#pragma once


// CVirtualCallerDlg 对话框

class CVirtualCallerDlg : public CDialog
{
	DECLARE_DYNAMIC(CVirtualCallerDlg)

public:
	CVirtualCallerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVirtualCallerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
