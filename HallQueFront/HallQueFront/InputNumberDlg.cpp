// InputNumberDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HallQueFront.h"
#include "InputNumberDlg.h"


// CInputNumberDlg �Ի���

IMPLEMENT_DYNAMIC(CInputNumberDlg, CDialog)

CInputNumberDlg::CInputNumberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputNumberDlg::IDD, pParent)
{
	m_strPhoneNum = _T("");
}

CInputNumberDlg::~CInputNumberDlg()
{
}

void CInputNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_PHONENUM,m_ed_strPhoneNum);
}


BEGIN_MESSAGE_MAP(CInputNumberDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NUM1, &CInputNumberDlg::OnBnClickedButtonNum1)
	ON_BN_CLICKED(IDC_BUTTON_NUM2, &CInputNumberDlg::OnBnClickedButtonNum2)
	ON_BN_CLICKED(IDC_BUTTON_NUM3, &CInputNumberDlg::OnBnClickedButtonNum3)
	ON_BN_CLICKED(IDC_BUTTON_NUM4, &CInputNumberDlg::OnBnClickedButtonNum4)
	ON_BN_CLICKED(IDC_BUTTON_NUM5, &CInputNumberDlg::OnBnClickedButtonNum5)
	ON_BN_CLICKED(IDC_BUTTON_NUM6, &CInputNumberDlg::OnBnClickedButtonNum6)
	ON_BN_CLICKED(IDC_BUTTON_NUM7, &CInputNumberDlg::OnBnClickedButtonNum7)
	ON_BN_CLICKED(IDC_BUTTON_NUM8, &CInputNumberDlg::OnBnClickedButtonNum8)
	ON_BN_CLICKED(IDC_BUTTON_NUM9, &CInputNumberDlg::OnBnClickedButtonNum9)
	ON_BN_CLICKED(IDC_BUTTON_NUM0, &CInputNumberDlg::OnBnClickedButtonNum0)
	ON_BN_CLICKED(IDC_BUTTON_NUMDEL, &CInputNumberDlg::OnBnClickedButtonNumdel)
	ON_BN_CLICKED(IDOK, &CInputNumberDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputNumberDlg ��Ϣ�������
BOOL CInputNumberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ed_strPhoneNum.SetFocus();
	m_editfont.CreatePointFont(300,_T("����"));
	m_ed_strPhoneNum.SetFont(&m_editfont);
	return TRUE;
}

void CInputNumberDlg::OnBnClickedButtonNum1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD1,MapVirtualKey(VK_NUMPAD1,0),0,0);  
	keybd_event(VK_NUMPAD1,MapVirtualKey(VK_NUMPAD1,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD2,MapVirtualKey(VK_NUMPAD2,0),0,0);  
	keybd_event(VK_NUMPAD2,MapVirtualKey(VK_NUMPAD2,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD3,MapVirtualKey(VK_NUMPAD3,0),0,0);  
	keybd_event(VK_NUMPAD3,MapVirtualKey(VK_NUMPAD3,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD4,MapVirtualKey(VK_NUMPAD4,0),0,0);  
	keybd_event(VK_NUMPAD4,MapVirtualKey(VK_NUMPAD4,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD5,MapVirtualKey(VK_NUMPAD5,0),0,0);  
	keybd_event(VK_NUMPAD5,MapVirtualKey(VK_NUMPAD5,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD6,MapVirtualKey(VK_NUMPAD6,0),0,0);  
	keybd_event(VK_NUMPAD6,MapVirtualKey(VK_NUMPAD6,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD7,MapVirtualKey(VK_NUMPAD7,0),0,0);  
	keybd_event(VK_NUMPAD7,MapVirtualKey(VK_NUMPAD7,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD8,MapVirtualKey(VK_NUMPAD8,0),0,0);  
	keybd_event(VK_NUMPAD8,MapVirtualKey(VK_NUMPAD8,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD9,MapVirtualKey(VK_NUMPAD9,0),0,0);  
	keybd_event(VK_NUMPAD9,MapVirtualKey(VK_NUMPAD9,0),KEYEVENTF_KEYUP,0);  
}


void CInputNumberDlg::OnBnClickedButtonNum0()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_NUMPAD0,MapVirtualKey(VK_NUMPAD0,0),0,0);  
	keybd_event(VK_NUMPAD0,MapVirtualKey(VK_NUMPAD0,0),KEYEVENTF_KEYUP,0);  
}

void CInputNumberDlg::OnBnClickedButtonNumdel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.SetFocus();
	keybd_event(VK_BACK,MapVirtualKey(VK_BACK,0),0,0);  
	keybd_event(VK_BACK,MapVirtualKey(VK_BACK,0),KEYEVENTF_KEYUP,0);
}

void CInputNumberDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ed_strPhoneNum.GetWindowText(m_strPhoneNum);
	OnOK();
}
