#pragma once


// CVirtualCaller �Ի���

class CVirtualCaller : public CDialog
{
	DECLARE_DYNAMIC(CVirtualCaller)

public:
	CVirtualCaller(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVirtualCaller();

// �Ի�������
	enum { IDD = IDD_DIALOG_CALLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
