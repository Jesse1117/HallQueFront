#pragma once


// CChangePassWordDlg 对话框

class CChangePassWordDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangePassWordDlg)

public:
	CChangePassWordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangePassWordDlg();

// 对话框数据
	enum { IDD = IDD_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

private:
	CString m_strOldPassWord;
	CString m_strNewWord;
	CString m_strNewPassWord2;
};
