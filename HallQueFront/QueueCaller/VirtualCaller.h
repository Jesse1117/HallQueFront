#pragma once


// CVirtualCaller 对话框

class CVirtualCaller : public CDialog
{
	DECLARE_DYNAMIC(CVirtualCaller)

public:
	CVirtualCaller(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVirtualCaller();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
