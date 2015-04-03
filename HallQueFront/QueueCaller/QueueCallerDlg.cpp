// QueueCallerDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CQueueCallerDlg �Ի���




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
}

BEGIN_MESSAGE_MAP(CQueueCallerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CQueueCallerDlg::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_FINDPASSWORD, &CQueueCallerDlg::OnBnClickedButtonFindpassword)
	ON_BN_CLICKED(IDC_CHECK_REMEMBER, &CQueueCallerDlg::OnBnClickedCheckRemember)
	ON_BN_CLICKED(IDC_CHECK_AUTOLOGIN, &CQueueCallerDlg::OnBnClickedCheckAutologin)
	ON_CBN_SELCHANGE(IDC_COMBO_USERNAME, &CQueueCallerDlg::OnCbnSelchangeComboUsername)
	ON_CBN_EDITCHANGE(IDC_COMBO_USERNAME, &CQueueCallerDlg::OnCbnEditchangeComboUsername)
	ON_BN_CLICKED(ID_BUTTON_LOGIN, &CQueueCallerDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// CQueueCallerDlg ��Ϣ�������

BOOL CQueueCallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	ReadUserFileFromFiles();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQueueCallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CQueueCallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQueueCallerDlg::OnBnClickedButtonRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRegisterDlg RegisterDlg(this);
	if (RegisterDlg.DoModal()==IDOK)
	{
		ReadUserFileFromFiles();
	}	
}

BOOL CQueueCallerDlg::ReadUserFileFromFiles()
{
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CChangePassWordDlg ChangePassWordDlg(this,m_strUserName);
	ChangePassWordDlg.DoModal();
}

void CQueueCallerDlg::OnBnClickedCheckRemember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CQueueCallerDlg::OnBnClickedCheckAutologin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CQueueCallerDlg::OnCbnSelchangeComboUsername()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		CButton* button = (CButton*)GetDlgItem(IDC_BUTTON_CHANGEPASSWORD);
		button->EnableWindow();

}

void CQueueCallerDlg::OnCbnEditchangeComboUsername()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (!m_strUserName.IsEmpty())
	{
		CButton* button = (CButton*)GetDlgItem(IDC_BUTTON_CHANGEPASSWORD);
		button->EnableWindow();
	}	
}

void CQueueCallerDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("�������û���"));
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
				CVirtualCallerDlg VirtualDlg(NULL);
				VirtualDlg.DoModal();
				/*this->DestroyWindow();*/
			}
			else
			{
				AfxMessageBox(_T("�������"));
				return;
			}
		}
	}
}
