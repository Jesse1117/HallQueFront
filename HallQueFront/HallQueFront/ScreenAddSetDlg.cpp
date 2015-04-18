// ScreenAddSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenAddSetDlg.h"
#include "SLZWindow.h"
#include "CommonStrMethod.h"
// CScreenAddSetDlg 对话框

IMPLEMENT_DYNAMIC(CScreenAddSetDlg, CDialog)

CScreenAddSetDlg::CScreenAddSetDlg(SLZWindow& windowinfo,CWnd* pParent /*=NULL*/)
	: CDialog(CScreenAddSetDlg::IDD, pParent)
{
	m_WindowInfo = windowinfo;
	m_cursel =0;
}

CScreenAddSetDlg::~CScreenAddSetDlg()
{
}

void CScreenAddSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_ADDTYPE,m_list_AddType);
	DDX_Control(pDX,IDC_LIST_ADDNUM,m_list_AddNum);
	DDX_Text(pDX,IDC_EDIT_ADDNUM,m_strAddNUm);
}


BEGIN_MESSAGE_MAP(CScreenAddSetDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_ADDTYPE, &CScreenAddSetDlg::OnLbnSelchangeListAddtype)
	ON_BN_CLICKED(IDC_BUTTON1, &CScreenAddSetDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CScreenAddSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CScreenAddSetDlg 消息处理程序
BOOL CScreenAddSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_list_AddType.AddString(WNDSCRADD);
	m_list_AddType.AddString(COMSCRADD);
	m_list_AddType.AddString(THROUGHADD);
	m_list_AddType.AddString(THROUGHPHY);
	m_WindowInfo.GetWndScreenId(m_wndarray);
	m_WindowInfo.GetComScreenId(m_comarray);
	m_WindowInfo.GetLEDPhyId(m_phyarray);
	m_WindowInfo.GetLEDIPId(m_strIParray);
	return TRUE;
}
void CScreenAddSetDlg::OnLbnSelchangeListAddtype()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_list_AddType.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString strSel;
	m_list_AddType.GetText(index,strSel);
	if (strSel==WNDSCRADD)
	{			
		m_cursel = 1;
		m_list_AddNum.ResetContent();
		CArray WndScrAdd;
		m_WindowInfo.GetWndScreenId(WndScrAdd);
		int count = WndScrAdd.GetCount();
		if (count>0)
		{
			for (int i = 0;i<count;i++)
			{
				CString strAdd;
				m_list_AddNum.AddString(CommonStrMethod::Int2Str(WndScrAdd[i]));
			}
		}
	}
	if (strSel==COMSCRADD)
	{
		m_cursel = 2;
		m_list_AddNum.ResetContent();
		CArray WndScrAdd;
		m_WindowInfo.GetComScreenId(WndScrAdd);
		int count = WndScrAdd.GetCount();
		if (count>0)
		{			
			for (int i = 0;i<count;i++)
			{
				CString strAdd;
				m_list_AddNum.AddString(CommonStrMethod::Int2Str(WndScrAdd[i]));
			}
		}
	}
	if (strSel==THROUGHADD)
	{
		m_cursel = 3;
		m_list_AddNum.ResetContent();
		CStringArray WndScrAdd;
		m_WindowInfo.GetComScreenId(WndScrAdd);
		int count = WndScrAdd.GetCount();
		if (count>0)
		{			
			for (int i = 0;i<count;i++)
			{
				CString strAdd;
				m_list_AddNum.AddString(WndScrAdd[i]);
			}
		}
	}
	if (strSel==THROUGHPHY)
	{
		m_cursel = 4;
		m_list_AddNum.ResetContent();
		CArray WndScrAdd;
		m_WindowInfo.GetLEDPhyId(WndScrAdd);
		int count = WndScrAdd.GetCount();
		if (count>0)
		{			
			for (int i = 0;i<count;i++)
			{
				CString strAdd;
				m_list_AddNum.AddString(CommonStrMethod::Int2Str(WndScrAdd[i]));
			}
		}
	}
}

void CScreenAddSetDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_list_AddType.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	UpdateData();
	if (m_list_AddNum==_T(""))
	{
		return;
	}
	m_list_AddNum.AddString(m_strAddNUm);
	if (m_cursel==1)
	{
		m_wndarray.Add(CommonStrMethod::Str2Int(m_strAddNUm));
	}
	if (m_cursel==2)
	{
		m_comarray.Add(CommonStrMethod::Str2Int(m_strAddNUm));
	}
	if (m_cursel==3)
	{
		m_strIParray.Add(m_strAddNUm);
	}
	if (m_cursel==4)
	{
		m_phyarray.Add(CommonStrMethod::Str2Int(m_strAddNUm));
	}
}

void CScreenAddSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_WindowInfo.SetWndScreenId(m_wndarray);
	m_WindowInfo.SetComScreenId(m_comarray);
	m_WindowInfo.SetLEDIPId(m_strIParray);
	m_WindowInfo.SetLEDPhyId(m_phyarray);
	OnOK();
}
