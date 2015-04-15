// ComSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "ComSetDlg.h"
#include "CommonConvert.h"
#include "CallJttsSetDlg.h"
#include "ShowMsgSetDlg.h"
#include "CommonStrMethod.h"
#include "TcpSever.h"
#include "SoundPlay.h"
#include "com/ComInit.h"
#include "ShortMsgModem.h"
// CComSetDlg �Ի���

IMPLEMENT_DYNAMIC(CComSetDlg, CDialog)

CComSetDlg::CComSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComSetDlg::IDD, pParent)
{
	m_pPlaySound = CSoundPlay::GetInstance();
	//m_pComInOut = CDoComInOut::GetInstance();
	m_pSever = new CTcpSever();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINFRAME);
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
	m_pShortMsg = CShortMsgModem::GetInstance();
	m_pComInit = CComInit::GetInstance();
}

CComSetDlg::~CComSetDlg()
{
}

void CComSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_IP,m_strIP);
	DDX_Text(pDX,IDC_EDIT_PORT,m_strPort);
	DDX_Text(pDX,IDC_EDIT_WNDID,m_strWndId);
	DDX_Text(pDX,IDC_EDIT_CALLNAME,m_strCallName);
	DDX_Text(pDX,IDC_EDIT_WAITTIME,m_strWatiTime);
	DDX_Text(pDX,IDC_EDIT_PLAYTIME,m_strPlayTimes);
	DDX_Text(pDX,IDC_EDIT_SELFPORT,m_strSelfPort);
	DDX_Text(pDX,IDC_COMBO_WNDCOM,m_strCom);
	DDX_Text(pDX,IDC_COMBO_MSGCOM,m_strMsgCom);
	DDX_Control(pDX,IDC_CHECK_SENDMSG,m_Check_SendMsg);
	DDX_Text(pDX,IDC_EDIT_SENDMSG,m_strShortMsg);
	DDX_Control(pDX,IDC_COMBO_WNDCOM,m_combo_WndCom);
	DDX_Control(pDX,IDC_COMBO_MSGCOM,m_combo_MsgCom);
	DDX_Text(pDX,IDC_EDIT_CALLADD1,m_strCallerAdd1);
	DDX_Text(pDX,IDC_EDIT_CALLADD2,m_strCallerAdd2);
}


BEGIN_MESSAGE_MAP(CComSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CComSetDlg::OnBnClickedOk)
	ON_MESSAGE(WM_MY_TRAYICON, OnTrayIcon)
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_WM_QUERYOPEN()
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_COMMAND(ID_CALLSET, &CComSetDlg::OnCallset)
	ON_COMMAND(ID_SHOWSET, &CComSetDlg::OnShowset)
	ON_COMMAND(ID_COMSET, &CComSetDlg::OnComset)
	ON_COMMAND(ID_QUIT, &CComSetDlg::OnQuit)
	ON_CBN_SELCHANGE(IDC_COMBO_MSGCOM, &CComSetDlg::OnCbnSelchangeComboMsgcom)
	ON_CBN_SELCHANGE(IDC_COMBO_WNDCOM, &CComSetDlg::OnCbnSelchangeComboWndcom)
END_MESSAGE_MAP()


// CComSetDlg ��Ϣ�������
BOOL CComSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow(GetDesktopWindow());
	LoadInfo();
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	AddTrayIcon();
	m_pSever->Start();
	m_pPlaySound->Init();
	//m_pComInOut->Start();
	SetTimer(11,10,NULL);
	return TRUE;
}

void CComSetDlg::LoadInfo()
{
	CCommonConvert convert;
	CString callerCom = m_pComInit->GetWndComm();
	CString MsgCom = m_pComInit->GetMsgComm();
	int i_wndsScreenCom=0;
	convert.CStringToint(i_wndsScreenCom,callerCom);
	if(m_pComInit->OpenWndScreen(i_wndsScreenCom) == -1)
	{
		m_combo_WndCom.SetCurSel(0);		
		MessageBox(_T("���������ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
	}

	int i_MsgCom=0;
	convert.CStringToint(i_MsgCom,MsgCom);
	m_pShortMsg->CloseMsgComm();
	if (i_MsgCom!=0)
	{
		if(!m_pShortMsg->OpenMsgComm(i_MsgCom))
		{
			MessageBox(_T("����è���ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
			m_combo_MsgCom.SetCurSel(0);
		}
		else 
		{
			CString strCom;
			CCommonConvert::intToCString(i_MsgCom,strCom);
			m_pComInit->SetMsgComm(strCom);
		}
	}
	m_combo_WndCom.AddString(_T("0"));
	m_combo_MsgCom.AddString(_T("0"));
	for(int i=0;i<10;i++)
	{
		if(m_pComInit->m_canUse[i]>0)
		{
			CString comm;
			comm.Format(_T("%d"),m_pComInit->m_canUse[i]);
			m_combo_WndCom.AddString(comm);
			m_combo_MsgCom.AddString(comm);
		}
	}
	for(int i=0;i<m_combo_WndCom.GetCount();i++)
	{
		CString content;
		m_combo_WndCom.GetLBText(i,content);
		if(callerCom == content)
		{
			m_combo_WndCom.SetCurSel(i);
			break;
		}
	}

	for(int i=0;i<m_combo_MsgCom.GetCount();i++)
	{
		CString content;
		m_combo_MsgCom.GetLBText(i,content);
		if(MsgCom == content)
		{
			m_combo_MsgCom.SetCurSel(i);
			break;
		}
	}
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("IP"),NULL,wbuf,255,m_strCallPath);
	CString strIp(wbuf);
	m_strIP = strIp;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Port"),NULL,wbuf,255,m_strCallPath);
	CString strPort(wbuf);
	m_strPort = strPort;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("WndId"),NULL,wbuf,255,m_strCallPath);
	CString strWndId(wbuf);
	m_strWndId = strWndId;
	/*ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerCom"),NULL,wbuf,255,m_strCallPath);
	CString strCom(wbuf);
	m_strCom = strCom;*/
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallName"),NULL,wbuf,255,m_strCallPath);
	CString strCallName(wbuf);
	m_strCallName = strCallName;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("WaitTime"),NULL,wbuf,255,m_strCallPath);
	CString strWaitTime(wbuf);
	m_strWatiTime = strWaitTime;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("PlayTimes"),NULL,wbuf,255,m_strCallPath);
	CString strPlayTimes(wbuf);
	m_strPlayTimes = strPlayTimes;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("SelfPort"),NULL,wbuf,255,m_strCallPath);
	CString strSelfPort(wbuf);
	m_strSelfPort = strSelfPort;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("IfSendMsg"),NULL,wbuf,255,m_strCallPath);
	CString strSendMsg(wbuf);
	int bSendMsg = CommonStrMethod::Str2Int(strSendMsg);
	if (bSendMsg)
	{
		m_Check_SendMsg.SetCheck(BST_CHECKED);
	}
	else
	{
		m_Check_SendMsg.SetCheck(BST_UNCHECKED);
	}
	/*ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("MsgCom"),NULL,wbuf,255,m_strCallPath);
	CString strMsgCom(wbuf);
	m_strMsgCom = strMsgCom;*/
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("ShortMsg"),NULL,wbuf,255,m_strCallPath);
	CString strShortMsg(wbuf);
	m_strShortMsg = strShortMsg;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerAdd1"),NULL,wbuf,255,m_strCallPath);
	CString strCallerAdd1(wbuf);
	m_strCallerAdd1 = strCallerAdd1;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallerAdd2"),NULL,wbuf,255,m_strCallPath);
	CString strCallerAdd2(wbuf);
	m_strCallerAdd2 = strCallerAdd2;
	UpdateData(FALSE);
}

HCURSOR CComSetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CComSetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	WritePrivateProfileString(_T("CompSet"),_T("IP"),m_strIP,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("Port"),m_strPort,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("SelfPort"),m_strSelfPort,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("WndId"),m_strWndId,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("CallerCom"),m_strCom,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("CallName"),m_strCallName,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("WaitTime"),m_strWatiTime,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("PlayTimes"),m_strPlayTimes,m_strCallPath);
	if (m_Check_SendMsg.GetCheck()==BST_CHECKED)
	{
		WritePrivateProfileString(_T("CompSet"),_T("IfSendMsg"),_T("1"),m_strCallPath);
	}
	else
	{
		WritePrivateProfileString(_T("CompSet"),_T("IfSendMsg"),_T("0"),m_strCallPath);
	}
	WritePrivateProfileString(_T("CompSet"),_T("MsgCom"),m_strMsgCom,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("ShortMsg"),m_strShortMsg,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("CallerAdd1"),m_strCallerAdd1,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("CallerAdd2"),m_strCallerAdd2,m_strCallPath);
	ShowWindow(SW_HIDE);
}

void CComSetDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowWindow(SW_HIDE);
	//	CDialog::OnClose();
}

void CComSetDlg::AddTrayIcon(void)
{
	wchar_t tip[128] = {0};
	wsprintf(tip, _T("�����ն�"));
	//wchar_t tip[] = _T("�Ŷ�ϵͳӲ��������ʽ\0");
	//CHAR tip[] = "�ŶӶԽ�\0";
	TaskBarAddIcon(this->m_hWnd, IDI_MAINFRAME, m_hIcon, (LPCWSTR)tip);
}

BOOL CComSetDlg::TaskBarAddIcon(HWND hwnd, UINT uID, HICON hIcon, LPCWSTR lpszTip)
{
	BOOL res;
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_MY_TRAYICON;
	nid.hIcon = hIcon;
	if (lpszTip)
	{
		//lstrcpyn(nid.szTip, lpszTip, sizeof(nid.szTip));
		wcscpy_s(nid.szTip, sizeof(nid.szTip), lpszTip);
	}
	else
	{
		nid.szTip[0] = '\0';
	}
	res = Shell_NotifyIcon(NIM_ADD, &nid);

	if (hIcon)
	{
		DestroyIcon(hIcon);
	}
	return res;
}

BOOL CComSetDlg::TaskBarDeleteIcon(HWND hwnd, UINT uID)
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CComSetDlg::RemoveTrayIcon()
{
	TaskBarDeleteIcon(this->m_hWnd, IDI_MAINFRAME);
}

LRESULT CComSetDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDI_MAINFRAME)
	{
		return 1;
	}
	switch(lParam)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		{
			this->ShowWindow(SW_SHOW);
			this->ShowWindow(SW_NORMAL);
		}
		break;
	case WM_RBUTTONUP:
		{
			SetForegroundWindow();//�����������λ�����½ǵ����˵���ʧ
			CMenu menu;
			menu.LoadMenu(IDR_MENU1);
			CMenu   *pContextMenu=menu.GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵� 
			CPoint point;//����һ������ȷ�����λ�õ�λ��  
			GetCursorPos(&point);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,AfxGetMainWnd()); //��ָ��λ����ʾ�����˵�
		}
		break;
	}
	return 0;
}

void CComSetDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 11:
		ShowWindow(SW_HIDE);
		UpdateWindow();
		KillTimer(11);
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CComSetDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	LONG lRes = CDialog::DefWindowProc(message, wParam, lParam);
	switch(message)
	{
	case WM_SYSCOMMAND:
		if(wParam == SC_MINIMIZE)
		{
			ShowWindow(SW_HIDE);
		}
		break;
	}
	return lRes;
}

void CComSetDlg::OnCallset()
{
	// TODO: �ڴ���������������
	CCallJttsSetDlg SoundDlg;
	SoundDlg.DoModal();
}

void CComSetDlg::OnShowset()
{
	// TODO: �ڴ���������������
	CShowMsgSetDlg ShowDlg;
	ShowDlg.DoModal();
}

void CComSetDlg::OnComset()
{
	// TODO: �ڴ���������������
	this->ShowWindow(SW_NORMAL);
}

void CComSetDlg::OnQuit()
{
	// TODO: �ڴ���������������
	if(IDOK==MessageBox(_T("ȷ���˳���?"),_T("����"),MB_OKCANCEL | MB_ICONINFORMATION))
	{
		RemoveTrayIcon();
		DestroyWindow();
	}
}

void CComSetDlg::OnCbnSelchangeComboMsgcom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCommonConvert convert;
	int index=m_combo_MsgCom.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString MsgCom=_T("");
	m_combo_MsgCom.GetLBText(index,MsgCom);
	int i_MsgCom=0;
	convert.CStringToint(i_MsgCom,MsgCom);
	m_pShortMsg->CloseMsgComm();
	if (i_MsgCom!=0)
	{
		if(!m_pShortMsg->OpenMsgComm(i_MsgCom))
		{
			MessageBox(_T("����è���ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
			m_combo_MsgCom.SetCurSel(0);
		}
		else 
		{
			CString strCom;
			CCommonConvert::intToCString(i_MsgCom,strCom);
			m_pComInit->SetMsgComm(strCom);
		}
	}
	else m_pComInit->SetMsgComm(L"0");
}

void CComSetDlg::OnCbnSelchangeComboWndcom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCommonConvert convert;
	int index=m_combo_WndCom.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString wndScreenCom=_T("");
	m_combo_WndCom.GetLBText(index,wndScreenCom);
	int i_wndsScreenCom=0;
	convert.CStringToint(i_wndsScreenCom,wndScreenCom);

	if(m_pComInit->OpenWndScreen(i_wndsScreenCom) == -1)
	{
		m_combo_WndCom.SetCurSel(0);		
		MessageBox(_T("���������ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
	}
}
