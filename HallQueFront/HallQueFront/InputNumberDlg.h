#pragma once


// CInputNumberDlg �Ի���

class CInputNumberDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputNumberDlg)

public:
	CInputNumberDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputNumberDlg();

// �Ի�������
	enum { IDD = IDD_INPUTNUMERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_ed_strPhoneNum;
	CFont m_editfont;
public:
	CString m_strPhoneNum;
	afx_msg void OnBnClickedButtonNum1();
	afx_msg void OnBnClickedButtonNum2();
	afx_msg void OnBnClickedButtonNum3();
	afx_msg void OnBnClickedButtonNum4();
	afx_msg void OnBnClickedButtonNum5();
	afx_msg void OnBnClickedButtonNum6();
	afx_msg void OnBnClickedButtonNum7();
	afx_msg void OnBnClickedButtonNum8();
	afx_msg void OnBnClickedButtonNum9();
	afx_msg void OnBnClickedButtonNum0();
	afx_msg void OnBnClickedButtonNumdel();
	afx_msg void OnBnClickedOk();
};
