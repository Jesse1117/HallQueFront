// QueueCallerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueCaller.h"
#include "QueueCallerDlg.h"
#include "RegisterDlg.h"
#include "ChangePassWordDlg.h"
#include "ChangePassWordDlg.h"
#include "resource.h"
#include "CommonConvert.h"
#include "VirtualCallerDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CQueueCallerDlg 对话框




CQueueCallerDlg::CQueueCallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQueueCallerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CCommonConvert convert;
	m_strUserInfoFilePath = convert.GetExeFullFilePath();
	m_strUserInfoFilePath+=_T("\\UserInfo\\UserInfo.dat");
}

void CQueueCallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_COMBO_USERNAME,m_strUserName);
	DDX_Text(pDX,IDC_EDIT_PASSWORD,m_strPassWord);
	DDX_Control(pDX,IDC_COMBO_USERNAME,m_cs_ComUserInfo);
	DDX_Control(pDX,IDC_CHECK_AUTOLOGIN,m_cs_AutoLogin);
	DDX_Control(pDX,IDC_CHECK_REMEMBER,m_cs_Remember);
}

BEGIN_MESSAGE_MAP(CQueueCallerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CQueueCallerDlg::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_FINDPASSWORD, &CQueueCallerDlg::OnBnClickedButtonFindpassword)
	ON_BN_CLICKED(IDC_CHECK_REMEMBER, &CQueueCallerDlg::OnBnClickedCheckRemember)
	ON_BN_CLICKED(IDC_CHECK_AUTOLOGIN, &CQueueCallerDlg::OnBnClickedCheckAutologin)
	ON_CBN_SELCHANGE(IDC_COMBO_USERNAME, &CQueueCallerDlg::OnCbnSelchangeComboUsername)
	ON_CBN_EDITCHANGE(IDC_COMBO_USERNAME, &CQueueCallerDlg::OnCbnEditchangeComboUsername)
	ON_BN_CLICKED(ID_BUTTON_LOGIN, &CQueueCallerDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// CQueueCallerDlg 消息处理程序

BOOL CQueueCallerDlg::OnInitDialog()
{		
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	ReadUserFileFromFiles();
	//for (int i=0;i<m_UserInfoMap.GetCount();i++)
	//{
	//	CUserInfo info;
	//	m_UserInfoMap.Lookup(i,info);
	//	if (info.isLogin())
	//	{
	//		CVirtualCallerDlg* VirtualDlg = new CVirtualCallerDlg(this,info.GetUserName());
	//		VirtualDlg->Create(IDD_DIALOG_CALLER);
	//		VirtualDlg->ShowWindow(SW_SHOWNORMAL);
	//		ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);//从任务栏中去掉.
	//		WINDOWPLACEMENT wp;
	//		wp.length=sizeof(WINDOWPLACEMENT);
	//		wp.flags=WPF_RESTORETOMAXIMIZED;
	//		wp.showCmd=SW_HIDE;
	//		SetWindowPlacement(&wp);
	//		break;  
	//	}
	//}
	SetTimer(1,1,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQueueCallerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent==1)
	{
		KillTimer(1);
		for (int i=0;i<m_UserInfoMap.GetCount();i++)
			{
				CUserInfo info;
				m_UserInfoMap.Lookup(i,info);
				if (info.isLogin())
				{
					CVirtualCallerDlg* VirtualDlg = new CVirtualCallerDlg(this,info.GetUserName());
					VirtualDlg->Create(IDD_DIALOG_CALLER);
					VirtualDlg->ShowWindow(SW_SHOWNORMAL);
					break;  
				}
			}
	}
	CDialog::OnTimer(nIDEvent);
}


void CQueueCallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQueueCallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQueueCallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQueueCallerDlg::OnBnClickedButtonRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	CRegisterDlg RegisterDlg(this);
	if (RegisterDlg.DoModal()==IDOK)
	{
		ReadUserFileFromFiles();
	}	
}

BOOL CQueueCallerDlg::ReadUserFileFromFiles()
{
	m_UserInfoMap.RemoveAll();
	m_cs_ComUserInfo.ResetContent();
	CFile file;
	CFileException e;
	if (file.Open(m_strUserInfoFilePath,CFile::modeRead,&e))
	{
		CUserInfo* userinfo;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>userinfo;
				if (userinfo)
				{
					m_UserInfoMap[m_cs_ComUserInfo.AddString(userinfo->GetUserName())]
					= *userinfo;
					delete userinfo;
					userinfo = NULL;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
			return TRUE;
	}
	else return FALSE;
	return TRUE;
}
void CQueueCallerDlg::OnBnClickedButtonFindpassword()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CChangePassWordDlg ChangePassWordDlg(this,m_strUserName);
	ChangePassWordDlg.DoModal();
}

void CQueueCallerDlg::OnBnClickedCheckRemember()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CQueueCallerDlg::OnBnClickedCheckAutologin()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CQueueCallerDlg::OnCbnSelchangeComboUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
		for (int i=0;i<m_UserInfoMap.GetCount();i++)
		{
			CUserInfo info;
			m_UserInfoMap.Lookup(i,info);
			CString strUser;
			m_cs_ComUserInfo.GetLBText(m_cs_ComUserInfo.GetCurSel(),strUser);
			if (strUser==info.GetUserName())
			{
				if (info.IsRemember())
				{
					m_strPassWord=info.GetPassWord();
					m_cs_Remember.SetCheck(BST_CHECKED);								
				}
				else m_cs_Remember.SetCheck(BST_UNCHECKED);
				if (info.isLogin())
				{
					m_cs_AutoLogin.SetCheck(BST_CHECKED);
				}
				else m_cs_AutoLogin.SetCheck(BST_UNCHECKED);
				UpdateData(FALSE);	
				break;
			}
		}
		CButton* button = (CButton*)GetDlgItem(IDC_BUTTON_CHANGEPASSWORD);
		button->EnableWindow();

}

void CQueueCallerDlg::OnCbnEditchangeComboUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (!m_strUserName.IsEmpty())
	{
		CButton* button = (CButton*)GetDlgItem(IDC_BUTTON_CHANGEPASSWORD);
		button->EnableWindow();
	}	
}

void CQueueCallerDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("请输入用户名"));
		return;
	}
	for (int i=0;i<m_UserInfoMap.GetCount();i++)
	{
		CUserInfo info;
		m_UserInfoMap.Lookup(i,info);
		if (m_strUserName==info.GetUserName())
		{
			if (m_strPassWord==info.GetPassWord())
			{
				if (m_cs_AutoLogin.GetCheck()==BST_CHECKED)
				{
					SetOrtherUnAuto();

					info.SetLogin(TRUE);
				}
				if (m_cs_Remember.GetCheck()==BST_CHECKED)
					info.SetRemember(TRUE);
				m_UserInfoMap.SetAt(i,info);
				WriteUserInfoIntoFiles();
				CVirtualCallerDlg* VirtualDlg = new CVirtualCallerDlg(this,info.GetUserName());
				VirtualDlg->Create(IDD_DIALOG_CALLER);
				VirtualDlg->ShowWindow(SW_SHOWNORMAL);

				//this->ShowWindow(SW_HIDE);
			}
			else
			{
				AfxMessageBox(_T("密码错误"));
				return;
			}
		}
	}
}

BOOL CQueueCallerDlg::WriteUserInfoIntoFiles()
{
	CFile file;
	CFileException e;
	if (file.Open(m_strUserInfoFilePath,CFile::modeCreate|CFile::modeWrite,&e))
	{
		int count = m_UserInfoMap.GetCount();
		for (int i=0;i<count;i++)
		{
			CArchive ar(&file,CArchive::store);
			CUserInfo userinfo;
			m_UserInfoMap.Lookup(i,userinfo);
			ar<<&userinfo;
			ar.Close();
		}
		file.Close();
		return TRUE;
	}
	else
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
}

void CQueueCallerDlg::SetOrtherUnAuto()
{
	for (int i=0;i<m_UserInfoMap.GetCount();i++)
	{
		CUserInfo info;
		m_UserInfoMap.Lookup(i,info);
		info.SetLogin(FALSE);
		m_UserInfoMap.SetAt(i,info);
	}
}