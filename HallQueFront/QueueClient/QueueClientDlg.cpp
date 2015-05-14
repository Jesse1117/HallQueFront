// QueueClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QueueClient.h"
#include "QueueClientDlg.h"
#include "SetDialog.h"
#include "../HallQueFront/ComInit.h"
#include "../HallQueFront/ShortMsgModem.h"
#include "../HallQueFront/CommonConvert.h"
#include "CallThread.h"
#include "SoundPlay.h"
#include "DealData.h"
#include "SelectSocketServer.h"
#include "CallThread.h"

extern void MyWriteConsole(CString str);

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


// CQueueClientDlg �Ի���




CQueueClientDlg::CQueueClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQueueClientDlg::IDD, pParent)
	,m_pComInit(NULL)
	,m_pShortMsg(NULL)
	,m_pPlaySound(NULL)
	,m_pCallThread(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pServer = new CSelectSocketServer;
	
}

void CQueueClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QUEUE, m_listctr_que);
	DDX_Control(pDX, IDC_STA_WAITNUM, m_sta_waitNum);
}

BEGIN_MESSAGE_MAP(CQueueClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CQueueClientDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_QUEUE, &CQueueClientDlg::OnLvnItemchangedListQueue)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
//	ON_MESSAGE(WAITNUM_MSG,&CQueueClientDlg::)
	ON_MESSAGE(WM_WAITNUMMSG,&CQueueClientDlg::OnShowWaitNum)
	ON_MESSAGE(WM_SHOWMSG,&CQueueClientDlg::OnShowAddMsg)
	ON_MESSAGE(WM_SHOWCALLMSG,&CQueueClientDlg::OnShowCallMsg)
	ON_MESSAGE(WM_SHOWDONEMSG,&CQueueClientDlg::OnShowDoneMsg)
	ON_MESSAGE(WM_SHOWNORMALMSG,&CQueueClientDlg::OnShowNormalMsg)
END_MESSAGE_MAP()


// CQueueClientDlg ��Ϣ�������

BOOL CQueueClientDlg::OnInitDialog()
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
	m_pComInit = CComInit::GetInstance();
	m_pShortMsg = CShortMsgModem::GetInstance();
	m_pCallThread = new CCallThread;
	m_pCallThread->Start();
	
	m_pPlaySound = CSoundPlay::GetInstance();
	m_pPlaySound->Init();
	m_pServer->InitServer();

	

	m_listctr_que.SetBkColor(RGB(255,255,255));
	m_listctr_que.SetRowHeigt(17);
	m_listctr_que.SetFontHW(15, 0);
	m_listctr_que.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_listctr_que.SetExtendedStyle(LVS_EX_GRIDLINES);

	CRect rect;
	m_listctr_que.GetClientRect(&rect);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int nCol = 0;
	m_listctr_que.InsertColumn(nCol++,_T("���"),LVCFMT_CENTER,rect.Width()*7/100,-1);
	m_listctr_que.InsertColumn(nCol++,_T("����"),LVCFMT_CENTER,rect.Width()*10/100,-1);
	m_listctr_que.InsertColumn(nCol++,_T("���֤��"),LVCFMT_CENTER,rect.Width()*20/100,-1);
	m_listctr_que.InsertColumn(nCol++,_T("ҵ������"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_que.InsertColumn(nCol++,_T("�ֻ�����"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_que.InsertColumn(nCol++,_T("�ŶӺ���"),LVCFMT_CENTER,rect.Width()*15/100,-1);
	m_listctr_que.InsertColumn(nCol++,_T("ID��"),LVCFMT_CENTER,rect.Width()*17/100,-1);
	

	CDealData* pDealData = CDealData::GetInstance();
	POSITION pos = pDealData->m_doingDataList.GetHeadPosition();
	SLZData data;
	while(pos)
	{
		data = pDealData->m_doingDataList.GetNext(pos);

		int count = m_listctr_que.GetItemCount();
		CString strCount;
		strCount.Format(_T("%d"),count+1);

		m_listctr_que.InsertItem(count,strCount);
		int nCol = 1;
		m_listctr_que.SetItemText(count,nCol++,data.GetCustName());
		m_listctr_que.SetItemText(count,nCol++,data.GetCardNumber());
		m_listctr_que.SetItemText(count,nCol++,data.GetBussName());
		m_listctr_que.SetItemText(count,nCol++,data.GetPhoneNum());
		m_listctr_que.SetItemText(count,nCol++,data.GetQueueNumber());
		m_listctr_que.SetItemText(count,nCol++,data.GetSerialId());
	}
	int count = pDealData->GetDoingDataCount();
	CString strCount;
	strCount.Format(_T("�ȴ�����:%d"),count);
	m_sta_waitNum.SetWindowText(strCount);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CQueueClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQueueClientDlg::OnPaint()
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
HCURSOR CQueueClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQueueClientDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSetDialog dlg(this);
	if(IDOK == dlg.DoModal())
	{
		m_pPlaySound->ReadWndData();
		CDealData* pDealData = CDealData::GetInstance();
		pDealData->ReadWndData();
	}
}

void CQueueClientDlg::OnLvnItemchangedListQueue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nCurrSlectItem = pNMLV->iItem;
	*pResult = 0;
}


void CQueueClientDlg::OpenAllComm()
{

	CString callerCom = m_pComInit->GetWndComm();
	CString MsgCom = m_pComInit->GetMsgComm();
	int i_wndsScreenCom=0;
	CCommonConvert::CStringToint(i_wndsScreenCom,callerCom);
	if(m_pComInit->OpenWndScreen(i_wndsScreenCom) == -1)
	{
		MessageBox(_T("���������ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
	}

	int i_MsgCom=0;
	CCommonConvert::CStringToint(i_MsgCom,MsgCom);
	m_pShortMsg->CloseMsgComm();
	if (i_MsgCom!=0)
	{
		if(!m_pShortMsg->OpenMsgComm(i_MsgCom))
		{
			MessageBox(_T("����è���ڴ�ʧ�ܻ�ռ��"),_T("ע��"),MB_OK|MB_ICONINFORMATION);
		}
		else 
		{
			CString strCom;
			CCommonConvert::intToCString(i_MsgCom,strCom);
			m_pComInit->SetMsgComm(strCom);
		}
	}
}
BOOL CQueueClientDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDealData* pDealData = CDealData::GetInstance();
	pDealData->SaveDoingData();
	pDealData->SaveWaitData();

	if(m_pCallThread)
	{
		delete m_pCallThread;
		m_pCallThread = NULL;
	}
	if(m_pServer)
	{
		delete m_pServer;
		m_pServer = NULL;
	}
	return CDialog::DestroyWindow();
}

BOOL CQueueClientDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	BOOL flag = CDialog::OnEraseBkgnd(pDC);
	return flag;
}

HBRUSH CQueueClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(m_sta_waitNum.m_hWnd == pWnd->m_hWnd)
	{
		CFont font;
		font.CreatePointFont(300,_T("����"));
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(0,0,255));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


LRESULT CQueueClientDlg::OnShowWaitNum(WPARAM wParam,LPARAM lParam)
{
	CString* pText = (CString*)wParam;
	if(!pText)return 0;
	m_sta_waitNum.SetWindowText(*pText);
	delete pText;
	return 0;
}

LRESULT CQueueClientDlg::OnShowAddMsg(WPARAM wParam,LPARAM lParam)
{
	SLZData* pData = (SLZData*)wParam;
	if(!pData)return 0;
	int count = m_listctr_que.GetItemCount();
	CString strCount;
	strCount.Format(_T("%d"),count+1);

	m_listctr_que.InsertItem(count,strCount);
	int nCol = 1;
	m_listctr_que.SetItemText(count,nCol++,pData->GetCustName());
	m_listctr_que.SetItemText(count,nCol++,pData->GetCardNumber());
	m_listctr_que.SetItemText(count,nCol++,pData->GetBussName());
	m_listctr_que.SetItemText(count,nCol++,pData->GetPhoneNum());
	m_listctr_que.SetItemText(count,nCol++,pData->GetQueueNumber());
	m_listctr_que.SetItemText(count,nCol++,pData->GetSerialId());
	
	delete pData;
	return 0;
}

LRESULT CQueueClientDlg::OnShowCallMsg(WPARAM wParam,LPARAM lParam)
{
	SLZData* pData = (SLZData*)wParam;
	if(!pData)return 0;
	
	int count = m_listctr_que.GetItemCount();
	CString strSerialID;
	for(int i=0;i<count;i++)
	{
		strSerialID = m_listctr_que.GetItemText(i,6);
		if(strSerialID == pData->GetSerialId())
		{
			m_listctr_que.SetItemText(i,1,pData->GetCustName());
			m_listctr_que.SetItemColor(1,i,RGB(255,0,0));
			break;
		}
	}
#ifdef _DEBUG
	MyWriteConsole(_T("���յ�callMsg"));
#endif
	delete pData;
	pData = NULL;

	return 0;
}

LRESULT CQueueClientDlg::OnShowDoneMsg(WPARAM wParam,LPARAM lParam)
{
	SLZData* pData = (SLZData*)wParam;

	int count = m_listctr_que.GetItemCount();
	CString strSerialID;
	for(int i=0;i<count;i++)
	{
		strSerialID = m_listctr_que.GetItemText(i,6);
		if(strSerialID == pData->GetSerialId())
		{
			m_listctr_que.SetItemText(i,1,pData->GetCustName());
			m_listctr_que.SetItemColor(1,i,RGB(144,144,144));
			break;
		}
	}

	delete pData;
	return 0;
}

LRESULT CQueueClientDlg::OnShowNormalMsg(WPARAM wParam,LPARAM lParam)
{
	SLZData* pData = (SLZData*)wParam;

	int count = m_listctr_que.GetItemCount();
	CString strSerialID;
	for(int i=0;i<count;i++)
	{
		strSerialID = m_listctr_que.GetItemText(i,6);
		if(strSerialID == pData->GetSerialId())
		{
//			m_listctr_que.SetItemText(i,1,pData->GetCustName());
//			m_listctr_que.SetItemColor(1,i,RGB(255,255,255));
			m_listctr_que.DeleteItem(i);
			break;
		}
	}

	count = m_listctr_que.GetItemCount();
	CString strCount;
	strCount.Format(_T("%d"),count);
	m_listctr_que.InsertItem(count,strCount);
	int nCol = 1;
	m_listctr_que.SetItemText(count,nCol++,pData->GetCustName());
	m_listctr_que.SetItemText(count,nCol++,pData->GetCardNumber());
	m_listctr_que.SetItemText(count,nCol++,pData->GetBussName());
	m_listctr_que.SetItemText(count,nCol++,pData->GetPhoneNum());
	m_listctr_que.SetItemText(count,nCol++,pData->GetQueueNumber());
	m_listctr_que.SetItemText(count,nCol++,pData->GetSerialId());

	delete pData;
	return 0;
}