// ComSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "ComSetDlg.h"
#include "CommonConvert.h"
#include "CallJttsSetDlg.h"
#include "ShowMsgSetDlg.h"
#include "CommonStrMethod.h"
#include "SoundPlay.h"
#include "com/ComInit.h"
#include "ShortMsgModem.h"
#include "DealData.h"
#include "SelectSocketServer.h"
// CComSetDlg 对话框

IMPLEMENT_DYNAMIC(CComSetDlg, CDialog)

CComSetDlg::CComSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComSetDlg::IDD, pParent)
{
	m_pPlaySound = CSoundPlay::GetInstance();
	//m_pComInOut = CDoComInOut::GetInstance();
//	m_pSever = new CTcpSever();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
	m_pShortMsg = CShortMsgModem::GetInstance();
	m_pComInit = CComInit::GetInstance();
	
	m_pDealData = CDealData::GetInstance();

	m_pSever = new CSelectSocketServer;
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
	DDX_Text(pDX,IDC_EDIT_BUSS1,m_strEditBuss1);
	DDX_Text(pDX,IDC_EDIT_BUSS2,m_strEditBuss2);
	DDX_Text(pDX,IDC_EDIT_CALLWAITTIME,m_strCallWaitTime);
	DDX_Control(pDX,IDC_EDIT_WNDID2,m_ed_wndAdd2);
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


// CComSetDlg 消息处理程序
BOOL CComSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow(GetDesktopWindow());
	LoadInfo();
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	AddTrayIcon();


	m_pSever->InitServer();
	m_pPlaySound->Init();
	m_pDealData->Start();

	CString strComWnd = m_pComInit->GetWndComm();
	m_combo_WndCom.SelectString(0,m_pComInit->GetWndComm());

	CString strShortMsgCom = m_pComInit->GetMsgComm();
	m_combo_MsgCom.SelectString(0,m_pComInit->GetMsgComm());
	


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
		MessageBox(_T("呼叫器串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}

	int i_MsgCom=0;
	convert.CStringToint(i_MsgCom,MsgCom);
	m_pShortMsg->CloseMsgComm();
	if (i_MsgCom!=0)
	{
		if(!m_pShortMsg->OpenMsgComm(i_MsgCom))
		{
			MessageBox(_T("短信猫串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
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
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Buss1"),NULL,wbuf,255,m_strCallPath);
	CString strBuss1(wbuf);
	m_strEditBuss1 = strBuss1;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("Buss2"),NULL,wbuf,255,m_strCallPath);
	CString strBuss2(wbuf);
	m_strEditBuss2 = strBuss2;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallWaitTime"),NULL,wbuf,255,m_strCallPath);
	CString strwait(wbuf);
	m_strCallWaitTime = strwait;

	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("WndId2"),NULL,wbuf,255,m_strCallPath);
	CString strWndId2(wbuf);
	m_strWnd2Add = strWndId2;
	m_ed_wndAdd2.SetWindowText(strWndId2);
	
	SecWndInfo wndInfo1;
	int nWndId1 = 0;
	int nCallerId1 = 0;
	CCommonConvert::CStringToint(nWndId1,strWndId);
	CCommonConvert::CStringToint(nCallerId1,strCallerAdd1);
	wndInfo1.nWndid = nWndId1;
	wndInfo1.nCallerid = nCallerId1;


	SecWndInfo wndInfo2;
	int nWndId2 = 0;
	int nCallerId2 = 0;
	CCommonConvert::CStringToint(nWndId2,strWndId2);
	CCommonConvert::CStringToint(nCallerId2,strCallerAdd2);
	wndInfo2.nWndid = nWndId1;
	wndInfo2.nCallerid = nCallerId2;

	
	theApp.m_map_secwndinfo[strBuss1] = wndInfo1;
	theApp.m_map_secwndinfo[strBuss2] = wndInfo2;
	UpdateData(FALSE);
}

HCURSOR CComSetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CComSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
	WritePrivateProfileString(_T("CompSet"),_T("Buss1"),m_strEditBuss1,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("Buss2"),m_strEditBuss2,m_strCallPath);
	WritePrivateProfileString(_T("CompSet"),_T("CallWaitTime"),m_strCallWaitTime,m_strCallPath);

	CString wndAdd2;
	m_ed_wndAdd2.GetWindowText(wndAdd2);
	WritePrivateProfileString(_T("CompSet"),_T("WndId2"),wndAdd2,m_strCallPath);
	m_strWnd2Add = wndAdd2;

	ShowWindow(SW_HIDE);

	////////////////////////////////////////////////
	SecWndInfo wndInfo1;
	int nWndId1 = 0;
	int nCallerId1 = 0;
	CCommonConvert::CStringToint(nWndId1,m_strWndId);
	CCommonConvert::CStringToint(nCallerId1,m_strCallerAdd1);
	wndInfo1.nWndid = nWndId1;
	wndInfo1.nCallerid = nCallerId1;


	SecWndInfo wndInfo2;
	int nWndId2 = 0;
	int nCallerId2 = 0;
	CCommonConvert::CStringToint(nWndId2,m_strWnd2Add);
	CCommonConvert::CStringToint(nCallerId2,m_strCallerAdd2);
	wndInfo2.nWndid = nWndId1;
	wndInfo2.nCallerid = nCallerId2;


	theApp.m_map_secwndinfo[m_strEditBuss1] = wndInfo1;
	theApp.m_map_secwndinfo[m_strEditBuss2] = wndInfo2;
}

void CComSetDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
	//	CDialog::OnClose();
}

void CComSetDlg::AddTrayIcon(void)
{
	wchar_t tip[128] = {0};
	wsprintf(tip, _T("呼叫终端"));
	//wchar_t tip[] = _T("排队系统硬件驱动程式\0");
	//CHAR tip[] = "排队对接\0";
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
			SetForegroundWindow();//点击界面其他位置右下角弹出菜单消失
			CMenu menu;
			menu.LoadMenu(IDR_MENU1);
			CMenu   *pContextMenu=menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
			CPoint point;//定义一个用于确定光标位置的位置  
			GetCursorPos(&point);//获取当前光标的位置，以便使得菜单可以跟随光标  
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,AfxGetMainWnd()); //在指定位置显示弹出菜单
		}
		break;
	}
	return 0;
}

void CComSetDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加命令处理程序代码
	CCallJttsSetDlg SoundDlg;
	SoundDlg.DoModal();
}

void CComSetDlg::OnShowset()
{
	// TODO: 在此添加命令处理程序代码
	CShowMsgSetDlg ShowDlg;
	ShowDlg.DoModal();
}

void CComSetDlg::OnComset()
{
	// TODO: 在此添加命令处理程序代码
	this->ShowWindow(SW_NORMAL);
}

void CComSetDlg::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	if(IDOK==MessageBox(_T("确定退出吗?"),_T("警告"),MB_OKCANCEL | MB_ICONINFORMATION))
	{
		DestroyWindow();
	}
}

void CComSetDlg::OnCbnSelchangeComboMsgcom()
{
	// TODO: 在此添加控件通知处理程序代码
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
			MessageBox(_T("短信猫串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
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
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox(_T("呼叫器串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
}

BOOL CComSetDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	RemoveTrayIcon();
	return CDialog::DestroyWindow();
}

void CComSetDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnCancel();
}
