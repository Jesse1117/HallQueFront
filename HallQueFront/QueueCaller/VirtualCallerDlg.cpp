// VirtualCallerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "VirtualCallerDlg.h"
#include "CallJttsSetDlg.h"
#include "ShowMsgSetDlg.h"
#include "ComSetDlg.h"
// CVirtualCallerDlg �Ի���

IMPLEMENT_DYNAMIC(CVirtualCallerDlg, CDialog)

CVirtualCallerDlg::CVirtualCallerDlg(CWnd* pParent /*=NULL*/,CString strUserName)
	: CDialog(CVirtualCallerDlg::IDD, pParent)
,m_strUserName(strUserName)
{

}

CVirtualCallerDlg::~CVirtualCallerDlg()
{
	
}

void CVirtualCallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVirtualCallerDlg, CDialog)
	ON_WM_CLOSE()
	ON_COMMAND(ID_32771, &CVirtualCallerDlg::On32771)
	ON_COMMAND(ID_32772, &CVirtualCallerDlg::On32772)
	ON_COMMAND(ID_32773, &CVirtualCallerDlg::On32773)
	ON_COMMAND(ID_32774, &CVirtualCallerDlg::On32774)
	ON_COMMAND(ID_32775, &CVirtualCallerDlg::On32775)
END_MESSAGE_MAP()


// CVirtualCallerDlg ��Ϣ�������

BOOL CVirtualCallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->GetParent()->ShowWindow(SW_HIDE);
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	CRect rect;
	this->GetWindowRect(rect);
	this->SetWindowPos(NULL,nFullWidth-rect.Width()-nFullWidth/10,(nFullHeight-rect.Height())/2,rect.Height(),rect.Width(),SWP_NOSIZE);
	SetWindowText(_T("�����ն�   ��ǰ�û���")+m_strUserName);
	
	return TRUE;
}

void CVirtualCallerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->GetParent()->DestroyWindow();
	CDialog::OnClose();
}

void CVirtualCallerDlg::On32771()
{
	// TODO: �ڴ���������������
	GetParent()->ShowWindow(SW_SHOWNORMAL);
	this->DestroyWindow();
}

void CVirtualCallerDlg::On32772()
{
	// TODO: �ڴ���������������
	OnClose();
}

/*������������*/
void CVirtualCallerDlg::On32773()
{
	// TODO: �ڴ���������������
	CString strCallMsg;
	CCallJttsSetDlg SoundDlg(strCallMsg);
	SoundDlg.DoModal();
}
/*������ʾ����*/
void CVirtualCallerDlg::On32774()
{
	// TODO: �ڴ���������������
	//pShowInfo *info=new pShowInfo();
	//CShowMsgSetDlg ShowDlg(*info,this);
	//ShowDlg.DoModal();
}

void CVirtualCallerDlg::On32775()
{
	// TODO: �ڴ���������������

	/*CComSetDlg ComSetDlg(this,)*/
}
