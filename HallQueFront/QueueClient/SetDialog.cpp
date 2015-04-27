// SetDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueClient.h"
#include "SetDialog.h"
#include "../HallQueFront/CommonConvert.h"
#include "com/ComInit.h"
#include "ShortMsgModem.h"
#include "SystemBasicData.h"
#include "../HallQueFront/DoFile.h"

// CSetDialog 对话框

IMPLEMENT_DYNAMIC(CSetDialog, CDialog)

CSetDialog::CSetDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDialog::IDD, pParent)
	,m_pCommInit(NULL)
	,m_pShortMsg(NULL)
{
	m_pShortMsg = CShortMsgModem::GetInstance();
	CDoFile doFile;
	m_strSavePath = doFile.GetExeFullFilePath();
	m_strSavePath += _T("\\BasiceSet");
	doFile.CreateMyDirectory(m_strSavePath);
	m_strSavePath += _T("\\BasiceSet.dat");
}

CSetDialog::~CSetDialog()
{
	ReleaseAllCallerWnd();
}

void CSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WND, m_listctr_wnd);
	DDX_Text(pDX, IDC_EDIT_WNDNAME, m_ed_wndName);
	DDX_Text(pDX, IDC_EDIT_WNDID, m_ed_wndAdd);
	DDX_Text(pDX, IDC_EDIT_CALLERID, m_ed_callerAdd);
	DDX_Text(pDX, IDC_EDIT_DOBUSS1, m_ed_doBuss1);
	DDX_Text(pDX, IDC_EDIT_DOBUSS2, m_ed_doBuss2);
	DDX_Text(pDX, IDC_EDIT_DOBUSS3, m_ed_doBuss3);
	DDX_Control(pDX, IDC_EDIT_NEXTIP, m_ed_nextIP);
	DDX_Control(pDX, IDC_COMBO_CALLER, m_combo_wnd);
	DDX_Control(pDX, IDC_COMBO_SHORTMSG, m_combo_shortmsg);
	DDX_Control(pDX, IDC_EDIT_CALLWAITTIME, m_ed_callWaitTime);
	DDX_Control(pDX, IDC_EDIT_SENDWAITTIME, m_ed_sendWaitTime);
	DDX_Control(pDX, IDC_EDIT_CALLNUM, m_ed_callNum);
	DDX_Control(pDX, IDC_CHECK_ISSHORTMSG, m_check_shotrmsg);
	DDX_Control(pDX, IDC_EDIT_SHORTMSG, m_ed_shortmsg);
}


BEGIN_MESSAGE_MAP(CSetDialog, CDialog)
	ON_BN_CLICKED(IDC_BN_ADD, &CSetDialog::OnBnClickedBnAdd)
	ON_BN_CLICKED(IDC_BN_DELETE, &CSetDialog::OnBnClickedBnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WND, &CSetDialog::OnLvnItemchangedListWnd)
	ON_EN_CHANGE(IDC_EDIT_WNDNAME, &CSetDialog::OnEnChangeEditWndname)
	ON_EN_CHANGE(IDC_EDIT_WNDID, &CSetDialog::OnEnChangeEditWndid)
	ON_EN_CHANGE(IDC_EDIT_CALLERID, &CSetDialog::OnEnChangeEditCallerid)
	ON_EN_CHANGE(IDC_EDIT_DOBUSS1, &CSetDialog::OnEnChangeEditDobuss1)
	ON_EN_CHANGE(IDC_EDIT_DOBUSS2, &CSetDialog::OnEnChangeEditDobuss2)
	ON_EN_CHANGE(IDC_EDIT_DOBUSS3, &CSetDialog::OnEnChangeEditDobuss3)
	ON_BN_CLICKED(IDOK, &CSetDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_CALLER, &CSetDialog::OnCbnSelchangeComboCaller)
	ON_CBN_SELCHANGE(IDC_COMBO_SHORTMSG, &CSetDialog::OnCbnSelchangeComboShortmsg)
END_MESSAGE_MAP()


// CSetDialog 消息处理程序

BOOL CSetDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listctr_wnd.SetBkColor(RGB(255,255,255));
	m_listctr_wnd.SetRowHeigt(15);
	m_listctr_wnd.SetFontHW(13, 0);
	m_listctr_wnd.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_listctr_wnd.SetExtendedStyle(LVS_EX_GRIDLINES);

	CRect rect;
	m_listctr_wnd.GetClientRect(&rect);
	// TODO:  在此添加额外的初始化
	int nCol = 0;
	m_listctr_wnd.InsertColumn(nCol++,_T("序号"),LVCFMT_CENTER,rect.Width()*7/100,-1);
	m_listctr_wnd.InsertColumn(nCol++,_T("窗口名称"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_wnd.InsertColumn(nCol++,_T("呼叫器地址"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_wnd.InsertColumn(nCol++,_T("窗口屏地址"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_wnd.InsertColumn(nCol++,_T("处理业务1"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_wnd.InsertColumn(nCol++,_T("处理业务2"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_wnd.InsertColumn(nCol++,_T("处理业务3"),LVCFMT_CENTER,rect.Width()*15/100,-1);


	m_combo_wnd.AddString(_T("0"));
	m_combo_shortmsg.AddString(_T("0"));

	m_pCommInit = CComInit::GetInstance();
	CString strCom;
	for(int i=0;i<10;++i)
	{
		if(m_pCommInit->m_canUse[i] >0)
		{
			strCom.Format(_T("%d"),m_pCommInit->m_canUse[i]);
			m_combo_wnd.AddString(strCom);
			m_combo_shortmsg.AddString(strCom);
		}
	}

	CSystemBasicData sysBasiceData;
	m_ed_nextIP.SetWindowText(sysBasiceData.GetNextIP());
	m_ed_sendWaitTime.SetWindowText(sysBasiceData.GetSendWaitTime());
	m_ed_shortmsg.SetWindowText(sysBasiceData.GetShortMsg());
	m_ed_callWaitTime.SetWindowText(sysBasiceData.GetCallWaitTime());
	
	CString strCallNum;
	strCallNum.Format(_T("%d"),sysBasiceData.GetReCallNum());
	m_ed_callNum.SetWindowText(strCallNum);

	if(sysBasiceData.GetIsSendShortMsg())
		m_check_shotrmsg.SetCheck(BST_CHECKED);
	else
		m_check_shotrmsg.SetCheck(BST_UNCHECKED);


	ReadBasicSetMsg();
	m_listctr_wnd.SetItemState(m_listctr_wnd.GetItemCount() - 1, LVIS_SELECTED, LVIS_SELECTED);


	CString strWndComm = m_pCommInit->GetWndComm();
	CString strModComm = m_pCommInit->GetMsgComm();

	for(int i = 0;i<m_combo_wnd.GetCount();i++)
	{
		CString temp;
		m_combo_wnd.GetLBText(i,temp);
		if(strWndComm == temp)
			m_combo_wnd.SetCurSel(i);
	}

	for(int i = 0;i<m_combo_shortmsg.GetCount();i++)
	{
		CString temp;
		m_combo_shortmsg.GetLBText(i,temp);
		if(strModComm == temp)
			m_combo_shortmsg.SetCurSel(i);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetDialog::OnBnClickedBnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CCallerWnd* pCallerWnd = new CCallerWnd;
	UINT nID = pCallerWnd->GetSerialID();
	pCallerWnd->SetCallerID(nID);
	pCallerWnd->SetWndID(nID);
	CString strName,	strCallerID,strWndID,strSerial;
	strName.Format(_T("%d号窗口"),nID);
	pCallerWnd->SetWndName(strName);

	CCommonConvert::intToCString(pCallerWnd->GetCallerID(),strCallerID);
	CCommonConvert::intToCString(pCallerWnd->GetWndID(),strWndID);

	UINT nCount = m_listctr_wnd.GetItemCount();
	int nHor = nCount;
	strSerial.Format(_T("%d"),++nCount);
	

	int nCol = 0;
	int nIndex = m_listctr_wnd.InsertItem(nHor,strSerial);
	if(nIndex == -1)return;

	m_listctr_wnd.SetItemText(nHor,++nCol,strName);
	m_listctr_wnd.SetItemText(nHor,++nCol,strCallerID);
	m_listctr_wnd.SetItemText(nHor,++nCol,strWndID);

	m_map_callerWnd[nIndex] = pCallerWnd;

	m_listctr_wnd.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
}

void CSetDialog::OnBnClickedBnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.find(m_nCurrSelectItem);
	if(itera != m_map_callerWnd.end())
	{
		m_map_callerWnd.erase(itera);
		m_listctr_wnd.DeleteItem(m_nCurrSelectItem);
		int nCount = m_listctr_wnd.GetItemCount();
		if(nCount)
			m_listctr_wnd.SetItemState(nCount-1,LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CSetDialog::OnLvnItemchangedListWnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_nCurrSelectItem = pNMLV->iItem;
	ShowCallerWndMsg();
	*pResult = 0;
}

void CSetDialog::ShowCallerWndMsg()
{
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.find(m_nCurrSelectItem);
	if(itera != m_map_callerWnd.end())
	{
		CCallerWnd* pCallerWnd = itera->second;
		m_ed_wndName = pCallerWnd->GetWndName();
		CString strWndID,strCallerID;
		CCommonConvert::intToCString(pCallerWnd->GetCallerID(),strCallerID);
		CCommonConvert::intToCString(pCallerWnd->GetWndID(),strWndID);
		m_ed_wndAdd = strWndID;
		m_ed_callerAdd = strCallerID;
		m_ed_doBuss1 = pCallerWnd->m_strCanDoBuss[0];
		m_ed_doBuss2 = pCallerWnd->m_strCanDoBuss[1];
		m_ed_doBuss3 = pCallerWnd->m_strCanDoBuss[2];
		UpdateData(FALSE);
 	}
}
void CSetDialog::OnEnChangeEditWndname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.find(m_nCurrSelectItem);
	if(itera != m_map_callerWnd.end())
	{
		CCallerWnd* pCallerWnd = itera->second;
		pCallerWnd->SetWndName(m_ed_wndName);
		m_listctr_wnd.SetItemText(m_nCurrSelectItem,1,m_ed_wndName);
	}
	
	// TODO:  在此添加控件通知处理程序代码
}

void CSetDialog::OnEnChangeEditWndid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.find(m_nCurrSelectItem);
	if(itera != m_map_callerWnd.end())
	{
		CCallerWnd* pCallerWnd = itera->second;
		int nWndID = 0;
		CCommonConvert::CStringToint(nWndID,m_ed_wndAdd);
		pCallerWnd->SetWndID(nWndID);
		m_listctr_wnd.SetItemText(m_nCurrSelectItem,3,m_ed_wndAdd);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CSetDialog::OnEnChangeEditCallerid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.find(m_nCurrSelectItem);
	if(itera != m_map_callerWnd.end())
	{
		CCallerWnd* pCallerWnd = itera->second;
		int nCallerID = 0;
		CCommonConvert::CStringToint(nCallerID,m_ed_callerAdd);
		pCallerWnd->SetCallerID(nCallerID);
		m_listctr_wnd.SetItemText(m_nCurrSelectItem,2,m_ed_callerAdd);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CSetDialog::OnEnChangeEditDobuss1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.find(m_nCurrSelectItem);
	if(itera != m_map_callerWnd.end())
	{
		CCallerWnd* pCallerWnd = itera->second;
		wcscpy_s(pCallerWnd->m_strCanDoBuss[0],LENGTH-1,m_ed_doBuss1.GetBuffer(0));
		m_listctr_wnd.SetItemText(m_nCurrSelectItem,4,m_ed_doBuss1);
		m_ed_doBuss1.ReleaseBuffer(0);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CSetDialog::OnEnChangeEditDobuss2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.find(m_nCurrSelectItem);
	if(itera != m_map_callerWnd.end())
	{
		CCallerWnd* pCallerWnd = itera->second;
		wcscpy_s(pCallerWnd->m_strCanDoBuss[1],LENGTH-1,m_ed_doBuss2.GetBuffer(0));
		m_listctr_wnd.SetItemText(m_nCurrSelectItem,5,m_ed_doBuss2);
		m_ed_doBuss2.ReleaseBuffer(0);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CSetDialog::OnEnChangeEditDobuss3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.find(m_nCurrSelectItem);
	if(itera != m_map_callerWnd.end())
	{
		CCallerWnd* pCallerWnd = itera->second;
		wcscpy_s(pCallerWnd->m_strCanDoBuss[2],LENGTH-1,m_ed_doBuss3.GetBuffer(0));
		m_listctr_wnd.SetItemText(m_nCurrSelectItem,6,m_ed_doBuss3);
		m_ed_doBuss3.ReleaseBuffer(0);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void CSetDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strCalltime,strSendTime,strRecallNum,strShortMsg,strNextIP;
	m_ed_callWaitTime.GetWindowText(strCalltime);
	CSystemBasicData systemBasicData;
	systemBasicData.SetCallWaitTime(strCalltime);
	
	m_ed_sendWaitTime.GetWindowText(strSendTime);
	systemBasicData.SetSendWaitTime(strSendTime);

	m_ed_callNum.GetWindowText(strRecallNum);
	int nNum = 0;
	CCommonConvert::CStringToint(nNum,strRecallNum);
	systemBasicData.SetReCallNum(nNum);

	m_ed_shortmsg.GetWindowText(strShortMsg);
	systemBasicData.SetShortMsg(strShortMsg);

	m_ed_nextIP.GetWindowText(strNextIP);
	systemBasicData.SetNextIP(strNextIP);

	if(m_check_shotrmsg.GetCheck() == BST_CHECKED)
	{
		systemBasicData.SetIsSendShortMsg(TRUE);
	}
	else
	{
		systemBasicData.SetIsSendShortMsg(FALSE);
	}
	systemBasicData.SaveBasicMsg();

	//////////////////////////////////////////////////////////
	WriteBasicSetMsg();
	//////////////////////////////////////////////////////////
	int nIndex = m_combo_shortmsg.GetCurSel();
	if(nIndex != CB_ERR)
	{
		CString strShortMsgCom;
		m_combo_shortmsg.GetLBText(nIndex,strShortMsgCom);
		m_pCommInit->SetMsgComm(strShortMsgCom);
	}
	m_pCommInit->SaveComm();
	OnOK();
}

void CSetDialog::OnCbnSelchangeComboCaller()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonConvert convert;
	int index=m_combo_wnd.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString wndScreenCom=_T("");
	m_combo_wnd.GetLBText(index,wndScreenCom);
	int i_wndsScreenCom=0;
	convert.CStringToint(i_wndsScreenCom,wndScreenCom);

	if(m_pCommInit->OpenWndScreen(i_wndsScreenCom) == -1)
	{
		m_combo_wnd.SetCurSel(0);		
		MessageBox(_T("呼叫器串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
}

void CSetDialog::OnCbnSelchangeComboShortmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonConvert convert;
	int index=m_combo_shortmsg.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString MsgCom=_T("");
	m_combo_shortmsg.GetLBText(index,MsgCom);
	int i_MsgCom=0;
	convert.CStringToint(i_MsgCom,MsgCom);
	m_pShortMsg->CloseMsgComm();
	if (i_MsgCom!=0)
	{
		if(!m_pShortMsg->OpenMsgComm(i_MsgCom))
		{
			MessageBox(_T("短信猫串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
			m_combo_shortmsg.SetCurSel(0);
		}
		else 
		{
			CString strCom;
			CCommonConvert::intToCString(i_MsgCom,strCom);
			m_pCommInit->SetMsgComm(strCom);
		}
	}
	else m_pCommInit->SetMsgComm(L"0");
}


BOOL CSetDialog::WriteBasicSetMsg()
{
	CFile file;
	if(file.Open(m_strSavePath,CFile::modeCreate | CFile::modeWrite))
	{
		map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.begin();
		for(itera;itera != m_map_callerWnd.end();++itera)
		{
			CCallerWnd* pCallerWnd = itera->second;
			CArchive ar(&file,CArchive::store);
			ar<<pCallerWnd;
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CSetDialog::ReadBasicSetMsg()
{
	CFile file;
	if(file.Open(m_strSavePath,CFile::modeRead))
	{
		CCallerWnd* pCallerWnd = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pCallerWnd;
				if (pCallerWnd)
				{
					AddDataToListctr(pCallerWnd);
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

void CSetDialog::AddDataToListctr(CCallerWnd* pCallerWnd)
{
	ASSERT(pCallerWnd);
	int nItem = m_listctr_wnd.GetItemCount();
	CString strSerial,strCallerID,strWndID;
	strSerial.Format(_T("%d"),nItem+1);
	int nCol = 0;
	int nIndex = m_listctr_wnd.InsertItem(nItem,strSerial);
	m_listctr_wnd.SetItemText(nItem,++nCol,pCallerWnd->GetWndName());
	strCallerID.Format(_T("%d"),pCallerWnd->GetCallerID());
	strWndID.Format(_T("%d"),pCallerWnd->GetWndID());
	m_listctr_wnd.SetItemText(nItem,++nCol,strWndID);
	m_listctr_wnd.SetItemText(nItem,++nCol,strWndID);
	m_listctr_wnd.SetItemText(nItem,++nCol,pCallerWnd->m_strCanDoBuss[0]);
	m_listctr_wnd.SetItemText(nItem,++nCol,pCallerWnd->m_strCanDoBuss[1]);
	m_listctr_wnd.SetItemText(nItem,++nCol,pCallerWnd->m_strCanDoBuss[2]);

	m_map_callerWnd[nIndex] = pCallerWnd;
}

void CSetDialog::ReleaseAllCallerWnd()
{
	map<int,CCallerWnd*>::const_iterator itera = m_map_callerWnd.begin();
	for(itera;itera != m_map_callerWnd.end();++itera)
	{
		if(itera->second)
			delete itera->second;
	}

	m_map_callerWnd.clear();
}