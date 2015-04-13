// CallJttsSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "CallJttsSetDlg.h"
#include "CommonStrMethod.h"

// CCallJttsSetDlg �Ի���

IMPLEMENT_DYNAMIC(CCallJttsSetDlg, CDialog)

CCallJttsSetDlg::CCallJttsSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCallJttsSetDlg::IDD, pParent)
{
	m_bMkJttsAll = FALSE;
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
}

CCallJttsSetDlg::~CCallJttsSetDlg()
{
}

void CCallJttsSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_SOUND,m_lbList);
	DDX_Control(pDX,IDC_LIST_LIB,m_lbWavLib);
	DDX_Control(pDX,IDC_CHECK_MKJTTSALL,m_check_mkjttsall);
	DDX_Control(pDX,IDC_RAD_COMCHN,m_Sel_CommonChinese);
	DDX_Control(pDX,IDC_RAD_ENGLISH,m_Sel_English);
}


BEGIN_MESSAGE_MAP(CCallJttsSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_QUENUM, &CCallJttsSetDlg::OnBnClickedButtonQuenum)
	ON_BN_CLICKED(IDC_BUTTON_WNDNAME, &CCallJttsSetDlg::OnBnClickedButtonWndname)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCallJttsSetDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MVUP, &CCallJttsSetDlg::OnBnClickedButtonMvup)
	ON_BN_CLICKED(IDC_BUTTON_MVDOWN, &CCallJttsSetDlg::OnBnClickedButtonMvdown)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCallJttsSetDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &CCallJttsSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_MKJTTSALL, &CCallJttsSetDlg::OnBnClickedCheckMkjttsall)
	ON_BN_CLICKED(IDC_BUTTON_BUSNAME, &CCallJttsSetDlg::OnBnClickedButtonBusname)
END_MESSAGE_MAP()

BOOL CCallJttsSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CString strSound;
	//m_pRset->GetFieldValue(_T("sound_wait"), strSound);
	m_check_mkjttsall.SetCheck(BST_UNCHECKED);
	m_Sel_CommonChinese.SetCheck(BST_CHECKED);
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CallSet"),_T("CallMsg"),NULL,wbuf,255,m_strCallPath);
	CString strCallMsg(wbuf);
	m_StrConf = strCallMsg;
	CStringArray arrStr;
	CommonStrMethod::StrSplit(m_StrConf, arrStr,_T("#"));
	for(int i = 1; i < arrStr.GetCount(); i++)
	{
		m_lbList.AddString(arrStr[i]);
	}

	CFileFind finder;
	BOOL bFound = finder.FindFile(CommonStrMethod::GetModuleDir() + _T("wavLib\\*.wav"));
	while(bFound)
	{
		bFound = finder.FindNextFile();
		m_lbWavLib.AddString(finder.GetFileTitle());
	}
	finder.Close();
	CenterWindow(GetDesktopWindow());
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// CCallJttsSetDlg ��Ϣ�������

void CCallJttsSetDlg::OnBnClickedButtonQuenum()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Sel_CommonChinese.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[�ŶӺ���]"));
		m_lbList.SetCurSel(nItem);
	}
	else if (m_Sel_English.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[�ŶӺ���(Ӣ)]"));
		m_lbList.SetCurSel(nItem);
	}
}



void CCallJttsSetDlg::OnBnClickedButtonWndname()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Sel_CommonChinese.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[��������]"));
		m_lbList.SetCurSel(nItem);
	}
	else if (m_Sel_English.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[��������(Ӣ)]"));
		m_lbList.SetCurSel(nItem);
	}
}


void CCallJttsSetDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index=m_lbWavLib.GetCurSel();
	if (index==CB_ERR)
	{
		return;
	}
	CString strSelect;
	m_lbWavLib.GetText(index,strSelect);
	int nItem = m_lbList.AddString(strSelect);
	m_lbList.SetCurSel(nItem);
}


void CCallJttsSetDlg::OnBnClickedButtonMvup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem = m_lbList.GetCurSel();
	if(nItem <= 0) return;
	CString strText;
	m_lbList.GetText(nItem, strText);
	m_lbList.DeleteString(nItem);
	m_lbList.InsertString(--nItem, strText);
	m_lbList.SetCurSel(nItem);
}


void CCallJttsSetDlg::OnBnClickedButtonMvdown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem = m_lbList.GetCurSel();
	if(nItem >= m_lbList.GetCount()-1 || nItem < 0) return;
	CString strText;
	m_lbList.GetText(nItem, strText);
	m_lbList.DeleteString(nItem);
	m_lbList.InsertString(++nItem, strText);
	m_lbList.SetCurSel(nItem);
}


void CCallJttsSetDlg::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index=m_lbList.GetCurSel();
	int count=m_lbList.GetCount();
	if(count<=0)
	{
		return;
	}
	if (index==CB_ERR)
	{
		MessageBox(_T("��ѡ��Ҫɾ����"),MB_OK,MB_ICONINFORMATION);
		return;
	}
	m_lbList.DeleteString(index);
	m_lbList.SetCurSel(index);
	if(m_lbList.GetCount()>0)
	{
		m_lbList.SetCurSel(m_lbList.GetCount()-1);
	}
}

void CCallJttsSetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strAll;
	for(int i = 0; i < m_lbList.GetCount(); i++)
	{
		CString strLine;
		m_lbList.GetText(i, strLine);
		strAll += _T("#")+strLine;
	}

	WritePrivateProfileString(_T("CallSet"),_T("CallMsg"),strAll,m_strCallPath);
	OnOK();
}

void CCallJttsSetDlg::OnBnClickedCheckMkjttsall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(BST_CHECKED == m_check_mkjttsall.GetCheck())
	{
		m_bMkJttsAll = TRUE;
	}
	else
	{
		m_bMkJttsAll = FALSE;
	}
}

void CCallJttsSetDlg::OnBnClickedButtonBusname()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Sel_CommonChinese.GetCheck()==BST_CHECKED)
	{
		int nItem = m_lbList.AddString(_T("[ҵ������]"));
		m_lbList.SetCurSel(nItem);
	}
	else 
	{
		AfxMessageBox(_T("ҵ������ֻ֧�ֺ���"));
	}
}
