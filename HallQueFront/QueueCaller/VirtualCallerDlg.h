#pragma once


// CVirtualCallerDlg �Ի���

class CVirtualCallerDlg : public CDialog
{
	DECLARE_DYNAMIC(CVirtualCallerDlg)

public:
	CVirtualCallerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVirtualCallerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CALLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
