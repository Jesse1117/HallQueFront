#pragma once


// CChangePassWordDlg �Ի���

class CChangePassWordDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangePassWordDlg)

public:
	CChangePassWordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangePassWordDlg();

// �Ի�������
	enum { IDD = IDD_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

private:
	CString m_strOldPassWord;
	CString m_strNewWord;
	CString m_strNewPassWord2;
};
