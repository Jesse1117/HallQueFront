#include "StdAfx.h"
#include "SLZController.h"
//#include "SLZCallerData.h"
//#include "SLZCardReader.h"
//#include "SLZCCaller.h"
//#include "SLZPrinter.h"
#include "SLZCEvaluator.h"
#include "HallQueFront.h"
#include "CommonConvert.h"
#include "QueSetDlg.h"
#include "HallQueFrontView.h"
#include "DoFile.h"
#include "SLZCWndScreen.h"
#include "ProduceClientPacket.h"
#include "ComplSocketClient.h"
#include "ShortMsgSetDlg.h"

extern  void MyWriteConsole(CString str); 

 //Ψһ�ŶӺ�

SLZController::SLZController(void) 
: m_pTakingNumThread(NULL)
, m_pTakingViewThread(NULL)
, m_pCallThread(NULL)
, m_pInlineQueData(NULL)
, m_pCalledQueData(NULL)
, m_pGetHardCallerCmd(NULL)
, m_pGetHardEvaCmd(NULL)
, m_pEvaThread(NULL)
, m_pPlaySound(NULL)
, m_pAlarmToCaller(NULL)
, m_pFinshQueData(NULL)
{
	m_print.Start();
	m_infofile_path = m_convert.GetExeFullFilePath();
	m_infofile_path += _T("\\QueBasicInfo\\QueBasicInfo.dat");
	m_InlineDataPath = m_convert.GetExeFullFilePath();
	m_InlineDataPath += _T("\\Data");
	CDoFile doFile;
	doFile.CreateMyDirectory(m_InlineDataPath);
	m_InlineDataPath += _T("\\InlineData.dat");
	m_MapQuePath = m_convert.GetExeFullFilePath()+_T("\\record");
	doFile.CreateMyDirectory(m_MapQuePath);
	//m_MapQuePath += _T("\\MapQue.dat");
	////////////////////////////////
// 	CString exePath = doFile.GetExeFullFilePath();
// 	m_caChePath = exePath + _T("\\Cache");
// 	doFile.CreateMyDirectory(m_caChePath);
// 	m_caChePath += _T("\\Cache.dat");
}

SLZController::~SLZController(void)
{
	if(m_pTakingViewThread)
	{
		TerminateThread(m_pTakingViewThread->m_hThread,0);
		delete m_pTakingViewThread;
		m_pTakingViewThread=NULL;
	}
	if (m_pTakingNumThread)
	{
		TerminateThread(m_pTakingNumThread->m_hThread,0);
		delete m_pTakingNumThread;
		m_pTakingNumThread=NULL;
	}
	if(m_pGetHardCallerCmd)
	{
		TerminateThread(m_pGetHardCallerCmd->m_hThread,0);
		delete m_pGetHardCallerCmd;
		m_pGetHardCallerCmd=NULL;
	}
	if(m_pGetHardEvaCmd)
	{
		TerminateThread(m_pGetHardEvaCmd->m_hThread,0);
		delete m_pGetHardEvaCmd;
		m_pGetHardEvaCmd = NULL;
	}
	//////////////////
	if(m_pAlarmToCaller)
	{
		TerminateThread(m_pAlarmToCaller->m_hThread,0);
		delete m_pAlarmToCaller;
		m_pAlarmToCaller = NULL;
	}
	if(m_pCalledQueData)
	{
		/*
		map<UINT, SLZWindow>::iterator iter=m_windowTable.m_mapIdWindow.begin();
		for (iter;iter!=m_windowTable.m_mapIdWindow.end();iter++)
		{
			SLZWindow Window;
			Window = iter->second;
			SLZData	data;
			if(m_pCalledQueData->GetCalledQueData(Window.GetWindowId(),data))
			{
				CTime t_FinishTime = CTime::GetCurrentTime();
				data.SetFinishTime(t_FinishTime);
				m_pFinshQueData->Add(data);
				m_pCalledQueData->DeleteCalledQueData(data);
			}
		}
		*/
		delete m_pCalledQueData;
		m_pCalledQueData = NULL;
	}
	End();
	if(m_pFinshQueData)
	{
		delete m_pFinshQueData;
		m_pFinshQueData = NULL;
	}
	
	if(m_pInlineQueData)
	{
		delete m_pInlineQueData;
		m_pInlineQueData = NULL;
	}
	/////////////////����û�кõĴ���ʽ����inline�д�����
   ////////////////ԭ������Ϊ�߳�֮������Ϸ���m_pInlineData���ַ��ʿ������⡣
	//////�����۽�������Գ���û��Ӱ�죬��Ϊ�������˳�ʱ������
}

BOOL SLZController::Start()
{
	////��Զ�����ݿ����Ϣ
	BOOL flag1 = FALSE;BOOL flag2 = FALSE; BOOL flag3 = FALSE; BOOL flag4 = FALSE;
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		flag1 = ReadCommDaoOrgInfo();
		flag2 = ReadCommDaoQueInfo();
		flag3 = ReadCommDaoStaffInfo();
		flag4 = ReadCommDaoWndInfo();
	}
	//////////////////////////
	ReadQueInfoFromFile();//��������Ϣ
	m_staffTable.LoadStaffs();
	m_windowTable.LoadWindows();
	m_pCaller = SLZCCaller::GetInstance();
	m_pEvaluator = SLZCEvaluator::GetInstance();
	m_pInlineQueData = new CInlineQueData(m_windowTable);
	m_pCalledQueData = new CCalledQueData(m_windowTable);
	m_pFinshQueData = new CFinshQueData;
	ReFlushSwingCardTable();
	m_cardread.Run();
	/////////////////////////���ͱ�����Ϣ����������ݿ�
	if(flag1&&flag2&&flag3&&flag4)
	{
		if(SendOrgData())
		{
			///�ȷ�ɾ��
			SendDelQue();
			SendDelStaff();
			SendDelWnd();
			///////////////
			SendQueData();
			SendStaffData();
			SendWndData();
		}
	}
	/////////////////////////�����߼���Ҫ��ʼ������
	InitLoginMap();//��ʼ����¼��
//	InitThroughScreen();//��ʼ��ͨ��
	///////////////�Ƿ��ȡ�����ϴ�δ����������ݵ�����
	if(IsHaveLastData())
	{
		if(IDYES == AfxMessageBox(_T("�Ƿ��ȡ�ϴ��Ŷ���Ϣ?"),MB_YESNO|MB_ICONINFORMATION))
		{
			ReadInlineDataFromFile();
			int QueCount = m_map_que.GetCount();
			for (int i=0;i<QueCount;i++)
			{
				CQueueInfo queinfo;
				if(m_map_que.Lookup(i,queinfo))
				{
					CString queid = queinfo.GetQueID();
					map_QueNum[queid] = m_pInlineQueData->GetMaxQueNum(queid);
				}
			}
		}
	}
	else /*if ()*/
	{
		ReadListQueFromFile();
		int datacount = m_list_Data.GetCount();
		if (datacount!=0)
		{
			if(IDYES == AfxMessageBox(_T("�Ƿ��ȡ�ϴ��Ŷ���Ϣ?"),MB_YESNO|MB_ICONINFORMATION))
			{
				for(int i = 0;i<datacount;i++)
				{
					SLZData data;
					POSITION pos = m_list_Data.FindIndex(i);
					if (pos!=NULL)
					{
						data = m_list_Data.GetAt(pos);
						if (JudgeTodayOrNot(data))
						{
							CString queid = data.GetBussinessType();
							map_QueNum[queid] = data.GetIntQueNum();
						}
					}
				}
			}
		}
	}
	///////////�������������
	m_pPlaySound = SoundPlay::GetInstance(m_windowTable);
	//////////////////////////
	m_pTakingNumThread = AfxBeginThread(TakingNumThreadProc,this,0,0,0,NULL);//�������߳�
	if (m_pTakingNumThread==NULL)
	{
		return FALSE;
	}
	m_pTakingViewThread = AfxBeginThread(TakingViewThreadProc,this,0,0,0,NULL);//�������߳�
	if (m_pTakingViewThread==NULL)
	{
		return FALSE;
	}
	m_pGetHardCallerCmd = AfxBeginThread(GetHardCallerCmdProc,this,0,0,0,NULL);//�������߳�
	if(!m_pGetHardCallerCmd)
	{
		return FALSE;
	}
	m_pGetHardEvaCmd = AfxBeginThread(GetHardEvaltorCmdProc,this,0,0,0,NULL);//�������߳�
	if(!m_pGetHardEvaCmd)
	{
		return FALSE;
	}
	m_pAlarmToCaller = AfxBeginThread(CountToCallerAlarm,this,0,0,0,NULL);
	if(!m_pAlarmToCaller)
	{
		return FALSE;
	}

	//�����߳̿�ʼ
	m_pCallThread = new CCallThread(*m_pInlineQueData,
		*m_pCalledQueData, m_CallerCmdData,*m_pFinshQueData,m_discardQueData,
		m_waitQueData,theApp.m_logicVariables,m_mapLoginList,m_map_que,m_staffTable);
//	m_pCallThread->OpenMsgQueue();
	m_pCallThread->Start();
	
	//�����߳̿�ʼ
	m_pEvaThread = new CEvaThread(m_EvaCmdData,m_pCallThread);
	m_pEvaThread->Start();

	
	return TRUE;
}

void SLZController::InitThroughScreen()
{
	SLZCWndScreen* pWndScreen = SLZCWndScreen::GetInstance();
	int throughID[255]={0};
	int size = m_windowTable.GetAllThroughID(throughID);
	for(int i=0;i<size;i++)
	{
		if(throughID[i]>0)
			pWndScreen->InitThroughScreen(throughID[i]);
	}
}

BOOL SLZController::End()
{
	if(m_pEvaThread)
	{
		m_pEvaThread->Exit();
		delete m_pEvaThread;
		m_pEvaThread = NULL;
	}
	if(m_pCallThread)
	{
		m_pCallThread->Exit();
		delete m_pCallThread;
		m_pCallThread = NULL;
	}
	return TRUE;
}

UINT SLZController::TakingNumThreadProc(LPVOID pParam)
{
	SLZController* pControl=(SLZController*) pParam; 
	//////////run
	while(1)
	{
		if(pControl->m_cardread.HasData())
		{
			SLZData data;
			CARDINFO cardInfo = pControl->m_cardread.GetData();
			if(pControl->DataNumOut(cardInfo.strAttchQueID))
			{
				AfxMessageBox(_T("ȡ��ʧ�ܣ���ȡ�������룡"));
			}
			else
			{
				////�Ŷ�����
				unsigned int CurNum=0;
				UINT iQueNum;
				UINT beginnum = pControl->GetQueBeginNum(cardInfo.strAttchQueID);
				pControl->map_QueNum.Lookup(cardInfo.strAttchQueID,CurNum);
				if (beginnum!=0&&CurNum==0)
				{
					iQueNum = CurNum+beginnum;
				}
				else
				{
					iQueNum = CurNum+1;
				}
				////�ŶӺ���
				CString QueCount;
				CString StrQueNum;
				StrQueNum.Format(_T("%03d"),iQueNum);
				StrQueNum=pControl->m_queinfo.GetFrontID()+StrQueNum;
				data.SetQueSerialID(pControl->m_queinfo.GetQueManNum());//���б��
				data.SetQueueNumber(StrQueNum);
				pControl->map_QueNum.SetAt(cardInfo.strAttchQueID,iQueNum);	
				data.SetIntQueNum(iQueNum);
				data.SetCardType(cardInfo.iCardType);//������
				data.SetCardNumber(cardInfo.strCardNumber);//����
				data.SetBussinessType(cardInfo.strAttchQueID);//�����ĸ�����
				data.SetCustomerLevel(cardInfo.iCustLevel);//�ͻ�����
				data.SetCustName(cardInfo.strCustName);//�ͻ�����
				CString bussName = pControl->GetQueNameFromID(cardInfo.strAttchQueID);
				if(!bussName.IsEmpty()) data.SetBussName(bussName);//��������
			///����
				CString cardNumber = data.GetCardNumber();			
				CTime GetTime = CTime::GetCurrentTime();
				data.SetTakingNumTime(GetTime);
				///���û������������
				data.SetOrganId(theApp.m_logicVariables.strOrganID);
				data.SetOrganName(theApp.m_logicVariables.strOrganNmae);
				if (!pControl->InsertListData(data))
				{
					pControl->m_list_Data.AddTail(data);
				}

				pControl->WriteListQueIntoFile();
				//////////////////////////
				pControl->m_pInlineQueData->Add(data);
				/////////////////////////������Ϣ�������ļ�
				pControl->WriteInlineDataToFile();
				///��ӡ
				CurNum=pControl->m_pInlineQueData->GetBussCount(cardInfo.strAttchQueID);
				EnumPrintStaus status = pControl->m_print.CheckPrinterStatus();
				pControl->DoPrintStatus(status,data,CurNum);
				///������ʾ�ȴ�����
				theApp.m_pView->ShowWaitNum(data.GetBussinessType(),CurNum);
				///���������µȴ�����
				pControl->m_pCallThread->ShowCallerWaitNum(data.GetBussinessType());
			}
		}
		else Sleep(10);
	}
	return 0;
}

UINT SLZController::TakingViewThreadProc(LPVOID pParam)
{
	SLZController* pControl=(SLZController*) pParam;
	while(1)
	{
		if(theApp.m_pView)
		{
		if(theApp.m_pView->HasData())
		{
			CARDINFO cardinfo=theApp.m_pView->GetData();
			CString queserial_id = cardinfo.strAttchQueID;
			if ( queserial_id== L"������ʾ")
			{
				CShortMsgSetDlg MsgDlg(NULL,*pControl->m_pInlineQueData);
				if(MsgDlg.DoModal()==IDOK)
				{
					Sleep(100);
					ShowVariables variables;
					memset(&variables,0,sizeof(variables));
					variables.bSendMsg = TRUE;
					SendMessage(theApp.m_pView->m_hWnd,WM_SHOWMSG,(WPARAM)&variables,0);
				}
			}
			else
			{
			if(pControl->VerifyCountLimit(queserial_id))
			{
				for (int i=0;i<pControl->m_map_que.GetCount();i++)
				{
					pControl->m_map_que.Lookup(i,pControl->m_queinfo);
					if (queserial_id==pControl->m_queinfo.GetQueID())
					{
						SLZData data;
						if (pControl->DataNumOut(queserial_id))
						{
							AfxMessageBox(_T("ȡ��ʧ�ܣ���ȡ�������룡"));
						}
						else
						{
							data.SetPhoneNum(cardinfo.strPhoneNum);
							data.SetCustName(cardinfo.strCustName);
							data.SetCardNumber(cardinfo.strCardNumber);
							//�����ŶӺ���
							unsigned int CurNum=0;
							UINT iQueNum;
							pControl->map_QueNum.Lookup(queserial_id,CurNum);
							UINT beginnum = pControl->GetQueBeginNum(queserial_id);
							if (beginnum!=0&&CurNum==0)
							{
								iQueNum = CurNum+beginnum;
							}
							else
							{
								iQueNum = CurNum+1;
							}
							CString QueCount;
							CString StrQueNum;
							StrQueNum.Format(_T("%03d"),iQueNum);
							StrQueNum=pControl->m_queinfo.GetFrontID()+StrQueNum;
							data.SetQueueNumber(StrQueNum);
							pControl->map_QueNum.SetAt(queserial_id,iQueNum);
							data.SetIntQueNum(iQueNum);
							CTime GetTime;
							GetTime=GetTime.GetCurrentTime();
							CString QueName;
							QueName=pControl->m_queinfo.GetBussName();
							data.SetBussName(QueName);
							data.SetTakingNumTime(GetTime);
							data.SetBussinessType(queserial_id);
							/////���û������������
							data.SetOrganId(theApp.m_logicVariables.strOrganID);
							data.SetOrganName(theApp.m_logicVariables.strOrganNmae);
							data.SetQueSerialID(pControl->m_queinfo.GetQueManNum());//���б��
							if (!pControl->InsertListData(data))
							{
								pControl->m_list_Data.AddTail(data);
							}
							pControl->WriteListQueIntoFile();
							////////////////////
							pControl->m_pInlineQueData->Add(data);
							//////////////////������Ϣ�������ļ�
							pControl->WriteInlineDataToFile();
							EnumPrintStaus status = pControl->m_print.CheckPrinterStatus();
							CurNum=pControl->m_pInlineQueData->GetBussCount(queserial_id);

							///�����ӡ
							pControl->DoPrintStatus(status,data,CurNum);

							///������ʾ�ȴ�����
							theApp.m_pView->ShowWaitNum(data.GetBussinessType(),CurNum);
							///���������µȴ�����
							pControl->m_pCallThread->ShowCallerWaitNum(data.GetBussinessType());
							//���̽������Ƿ񷵻�������
							if(theApp.m_pView->m_pTrackCtrl->GetSerialID()!=0 && theApp.m_logicVariables.IsAutoChangePage)
							{
//								Sleep(1000);
//								theApp.m_pView->ShowPage(0);//����������
								UINT nPageID = 0;
								SendMessage(theApp.m_pView->m_hWnd,WM_SHOWFIRSTPAGE,(WPARAM)nPageID,NULL);
							}
							break;
						}
					}
				}
			}
			else AfxMessageBox(_T("�ѳ�����������"));
			}

		}
		else Sleep(10);
		}
		else
		{
			Sleep(10);
		}
	}
	return 0;
}

BOOL SLZController::DataNumOut(CString QueId)
{
	UINT beginnum = GetQueBeginNum(QueId);
	UINT endnum = GetQueEndNum(QueId);
	UINT curmaxnum;
	map_QueNum.Lookup(QueId,curmaxnum);
	if (endnum==0)
	{
		return FALSE;
	}
	if (curmaxnum==0)
	{
		return FALSE;
	}
	else if ((curmaxnum+1)<=endnum)
		{
			return FALSE;
		}
		else 
		{
			return TRUE;
		}
}

BOOL SLZController::ReadQueInfoFromFile()
{
	CFile file;
	CFileException e;
	int index = 0;
	if (file.Open(m_infofile_path,CFile::modeRead,&e))
	{
		m_map_que.RemoveAll();
		CQueueInfo* queinfo=NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength()) 
			do
			{
				ar>>queinfo;
				if (queinfo)
				{
					m_map_que[index]=*queinfo;
					index++;
					delete queinfo;
					queinfo = NULL;
				}
			}while(!ar.IsBufferEmpty());
			ar.Close();
			file.Close();
	}
	return TRUE;
}

UINT SLZController::GetHardCallerCmdProc(LPVOID pParam)
{
	SLZController* pThis = (SLZController*)pParam;

	while(TRUE)
	{
		if(pThis->m_pCaller->HasData())
		{
			SLZCallerData callerData = pThis->m_pCaller->GetData();
			if(callerData.GetCmdType() == callerCmdShowAdd)
			{
				pThis->m_pCaller->AddWriteCallerData(callerData);
			}
			else
			{
				SLZWindow Window;
				BOOL flag = pThis->m_windowTable.QueryWindowByCallerId(
					callerData.GetCallerId(),Window);
				if(flag)
				{
					CallerCmd cmd;
					if(!Window.GetWindowId())
						continue;
					cmd.SetWindowId(Window.GetWindowId());
					cmd.SetCmdType(callerData.GetCmdType());
					cmd.SetCarriedData(callerData.GetAttchMsg());
					pThis->m_CallerCmdData.AddTail(cmd);
				}
			}
		}
		else
		{
			Sleep(10);
		}
	}
	::ExitThread(0);
	return 0;
}

BOOL SLZController::VerifyCountLimit(const CString& QueId)
{
	CTime time;
	time=time.GetCurrentTime();
	CString StrHour=time.Format("%H");
	int hour;
	m_convert.CStringToint(hour,StrHour);
	//ReadQueInfoFromFile();
	for (int i=0;i<m_map_que.GetCount();i++)
	{
		m_map_que.Lookup(i,m_queinfo);
		if (QueId==m_queinfo.GetQueID())
		{
			UINT amlimit=m_queinfo.GetAmLimitCustomer();
			UINT pmlimit=m_queinfo.GetPmLimitCustomer();
			if (hour<12)
			{
				if ((m_pInlineQueData->GetCount()<amlimit)||amlimit==0)
				{
					return TRUE; 
				}
				else return FALSE;
			}
			else if ((m_pInlineQueData->GetCount()<pmlimit)||pmlimit==0)
			{
				return TRUE;
			}
			else return FALSE;
		}
	}
	return TRUE;
}

BOOL SLZController::ReFlushPrintInfoTable()
{
	return m_print.ReFreshPrintInfo(); 
}

BOOL SLZController::ReFlushQueInfoTable()
{
	ReadQueInfoFromFile();
	ReFlushWindowTable();
	ReFlushSwingCardTable();
	///���Ͷ�������
	if(ReadCommDaoQueInfo())
	{
		SendDelQue();
		SendQueData();
	}
	return TRUE;
}

BOOL SLZController::ReFlushStaffTable()
{
	m_staffTable.LoadStaffs();
	ReFlushWindowTable();
	///����Ա������
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		if(ReadCommDaoStaffInfo())
		{
			SendDelStaff();
			SendStaffData();
		}
	}
	return TRUE;
}

BOOL SLZController::ReFlushWindowTable()
{
	m_windowTable.LoadWindows(); 
	m_pEvaluator->ReFlushWindowTable();
	InitLoginMap();
	////���ʹ�������
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		if(ReadCommDaoWndInfo())
		{
			SendDelWnd();
			SendWndData();
		}
	}
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	return TRUE;
}

BOOL SLZController::ReFlushSysLogicVarlibles()
{
	///���߼�����
	BOOL flag = theApp.ReadLogicVariablesFromFile();
	///������ʱ���Ͷ�������
	flag = m_pFinshQueData->ReFlushListShortMsg();
	//���ڿ��ܻ����Ѹ�������Ҫ���·�������,�ȶ����ݡ��ٷ���
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	BOOL flag1 = FALSE;BOOL flag2 = FALSE;BOOL flag3 = FALSE;
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		flag1 = ReadCommDaoQueInfo();
		flag2 = ReadCommDaoStaffInfo();
		flag3 = ReadCommDaoWndInfo();
	}
	//////////////�������ݵ������
	if(flag1&&flag2&&flag3)
	{
		if(SendOrgData())
		{
			SendDelQue();
			SendDelWnd();
			SendDelStaff();
			SendQueData();
			SendStaffData();
			SendWndData();
		}
	}
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	return flag;
}

BOOL SLZController::ReFlushSwingCardTable()
{
	m_cardread.ReFlushCardConnectInfo();//ˢ�¿�������Ϣ
	m_cardread.ReFlushCardConfigInof();//ˢ�¿�ʶ����Ϣ
	m_cardread.ReFlushCardLevelInfo();//ˢ�¿��ĶԽӵȼ�����Ӧ������Ϣ
	return TRUE;
}

CString SLZController::GetQueNameFromID(const CString& queID)
{
	if(queID.IsEmpty()) return _T("");
	CString bussName;
	for (int i=0;i<m_map_que.GetCount();i++)
	{
		m_map_que.Lookup(i,m_queinfo);
		if (queID == m_queinfo.GetQueID())
		{
			bussName = m_queinfo.GetBussName();
			break;
		}
	}
	return bussName;
}

UINT SLZController::GetQueBeginNum(const CString& queID)
{
	if(queID.IsEmpty()) return 0;
	int num;
	for (int i=0;i<m_map_que.GetCount();i++)
	{
		m_map_que.Lookup(i,m_queinfo);
		if (queID == m_queinfo.GetQueID())
		{
			num = m_queinfo.GetQueNumStart();
			break;
		}
	}
	return num;
}

UINT SLZController::GetQueEndNum(const CString& queID)
{
	if(queID.IsEmpty()) return 0;
	int num;
	for (int i=0;i<m_map_que.GetCount();i++)
	{
		m_map_que.Lookup(i,m_queinfo);
		if (queID == m_queinfo.GetQueID())
		{
			num = m_queinfo.GetQueNumEnd();
			break;
		}
	}
	return num;
}

void SLZController::DoPrintStatus(EnumPrintStaus status,const SLZData& data,const UINT waitNum)
{
	ShowVariables showVaria;
	memset(&showVaria,0,sizeof(showVaria));
	switch(status)
	{
	case enumPrintNormal://����
	case enumPrintPrinting:
		{
			m_print.Print(data,waitNum-1);
			showVaria.bShowWait = TRUE;
			SendMessage(theApp.m_pView->m_hWnd,WM_SHOWMSG,(WPARAM)&showVaria,0);
		}
		break;
	case enumPrintErr://��ӡ����
		{
			showVaria.bShowWait = TRUE;
			if(theApp.m_logicVariables.IsLackPaperAlarm)//����
			{
				//����ȱֽ��������
				m_pPlaySound->DataPlay(_T("��ӡ����,�����ӡ��"));
			}
			SendMessage(theApp.m_pView->m_hWnd,WM_SHOWMSG,(WPARAM)&showVaria,0);
		}
		break;
	case enumPrintPaperOut://ȱֽ
		{
			showVaria.bShowNoPage = TRUE;
			if(theApp.m_logicVariables.IsLackPaperAlarm)//ȱֽ����
			{
				//����ȱֽ��������
				m_pPlaySound->DataPlay(_T("��ӡ��ȱֽ"));
			}
			SendMessage(theApp.m_pView->m_hWnd,WM_SHOWMSG,(WPARAM)&showVaria,0);
		}
		break;
	case enumPrintOffline://����
		{
			showVaria.bShowWait = TRUE;
			if(theApp.m_logicVariables.IsLackPaperAlarm)//����
			{
				//����ȱֽ��������
				m_pPlaySound->DataPlay(_T("��ӡ������"));
			}
			SendMessage(theApp.m_pView->m_hWnd,WM_SHOWMSG,(WPARAM)&showVaria,0);
		}
		break;
	case enumPrintNoDefPrinter://û��Ĭ�ϴ�ӡ��
		{
			showVaria.bShowWait = TRUE;
			SendMessage(theApp.m_pView->m_hWnd,WM_SHOWMSG,(WPARAM)&showVaria,0);
		}
		break;
	case enumPrintFailedJob://��ȡ��ӡ����ʧ��
		{
			showVaria.bShowWait = TRUE;
			SendMessage(theApp.m_pView->m_hWnd,WM_SHOWMSG,(WPARAM)&showVaria,0);
		}
		break;
	default:
		{
			showVaria.bShowWait = TRUE;
			SendMessage(theApp.m_pView->m_hWnd,WM_SHOWMSG,(WPARAM)&showVaria,0);
		}
		break;
	}
}

UINT SLZController::GetHardEvaltorCmdProc(LPVOID pParam)
{
	SLZController* pThis = (SLZController*)pParam;

	while(TRUE)
	{
		if(pThis->m_pEvaluator->HasData())
		{
			SLZEvaData evaData = pThis->m_pEvaluator->GetData();
			SLZWindow Window; 
			BOOL flag = pThis->m_windowTable.QueryWindowByEvaId(
 				evaData.GetEvaluatorId(),Window);
// #ifdef _DEBUG
// 			CString str;
// 			str.Format(_T("%d"),evaData.GetEvaluatorId());
// 			MyWriteConsole(str);
// #endif
			if(flag)
			{
				pThis->m_EvaCmdData.Add(evaData);
			}
		}
		else
		{
			Sleep(10);
		}
	}
	return 0;
}

BOOL SLZController::JudgeWorkTimeOut(const CString& queID)
{
	int count = m_map_que.GetCount();
	for(int i=0;i<count;i++)
	{
		CQueueInfo queInfo = m_map_que[i];
		if(queID == queInfo.GetQueID())//�ҵ�����
		{
			CTime timeSart = queInfo.GetQueWorkTimeStart();
			CTime timeEnd = queInfo.GetQueWorkTimeEnd();
			CTime currTime = CTime::GetCurrentTime();
			CTime ModiFytimeSart(currTime.GetYear(),currTime.GetMonth(),
				currTime.GetDay(),timeSart.GetHour(),timeSart.GetMinute(),
				timeSart.GetSecond());
			CTime ModiFytimeEnd(currTime.GetYear(),currTime.GetMonth(),
				currTime.GetDay(),timeEnd.GetHour(),timeEnd.GetMinute(),
				timeEnd.GetSecond());
			if(currTime<ModiFytimeSart || currTime>ModiFytimeEnd)//���ڹ���ʱ����
			{
				return TRUE;//��ʱ 
			}
			else
			{
				return FALSE;
			}
			break;
		}
	}
	return FALSE;
}

BOOL SLZController::WriteInlineDataToFile()
{
	CFile file;
	CFileException e;
	int index = 0;
	m_mtWriteFile.Lock();
	if(file.Open(m_InlineDataPath,CFile::modeCreate | CFile::modeWrite,&e)){
		int count=m_pInlineQueData->GetCount();
		for(int i=0;i<count;++i){
			SLZData data;
			m_pInlineQueData->GetInlineQueData(i,data);
			CArchive ar(&file,CArchive::store);
			ar<<&data;
			ar.Close();
		}
		file.Close();
		m_mtWriteFile.Unlock();
		return TRUE;
	}
	else
	{
		m_mtWriteFile.Unlock();
		return FALSE;
	}
}

BOOL SLZController::ReadInlineDataFromFile()
{
	m_pInlineQueData->RemoveAllData();//���
	CFile file;
	CFileException e;
	if (file.Open(m_InlineDataPath,CFile::modeRead,&e))
	{
		SLZData* pData = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pData;
				m_pInlineQueData->Add(*pData);
				if (pData)
				{
					delete pData;
					pData = NULL;
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
	}
	else return FALSE;
	return TRUE;
}

BOOL SLZController::IsHaveLastData()
{
	BOOL flag = FALSE;
	CFile file;
	CFileException e;
	if (file.Open(m_InlineDataPath,CFile::modeRead,&e))
	{
		SLZData* pData = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pData;
				CTime takeNumTime = pData->GetTakingNumTime();
				CTime currTime = CTime::GetCurrentTime();
				if(pData)
				{
					delete pData;
					pData = NULL;
				}
				if(takeNumTime.GetYear()==currTime.GetYear() && takeNumTime.GetMonth()==currTime.GetMonth()
					&& takeNumTime.GetDay()==currTime.GetDay())
				{
					flag = TRUE;
				}
				else
				{
					flag = FALSE;
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();
	}
	return flag;
}

void SLZController::InitShowInlineQueNum()
{
	//////////////////////��ʼ��������ʾ�Ŷ�����
	int count = m_map_que.GetCount();
	for(int i=0;i<count;i++)
	{
		CString queID = m_map_que[i].GetQueID();
		UINT inlineNum = m_pInlineQueData->GetBussCount(queID);
		if(theApp.m_pView)
			theApp.m_pView->ShowWaitNum(queID,inlineNum);
	}
	////////////////////////
}
/*
����Ŷ�����
*/
BOOL SLZController::ClearSystemData()
{
	CTime currTime = CTime::GetCurrentTime();
	if(theApp.m_logicVariables.IsAutoClearData)
	{
		CTime clearTime = theApp.m_logicVariables.ClearDataTime;
		CTime newTime(currTime.GetYear(),currTime.GetMonth(),currTime.GetDay(),
			clearTime.GetHour(),clearTime.GetMinute(),clearTime.GetSecond());
		if(currTime==newTime)
		{
			m_pInlineQueData->RemoveAllData();
			//��ձ����������
			CDoFile doFile;
			CString path = doFile.GetExeFullFilePath();
			path+=_T("\\MapQue.dat");
			CFile file;
			if(file.Open(path,CFile::modeCreate|CFile::modeWrite))
			{
				file.SetLength(0);
				file.Close();
			}
			/////////////////////
			InitShowInlineQueNum();
			return TRUE;
		}
	}
	return FALSE;
}

int SLZController::GetMaxQueNum(const CString QueID)
{
	return m_pInlineQueData->GetMaxQueNum(QueID);
}

UINT SLZController::CountToCallerAlarm(LPVOID pParam)
{
	SLZController* pThis = (SLZController*)pParam;
	while(TRUE)
	{
		pThis->m_mtAlarm.Lock();
		int len = pThis->m_map_que.GetCount();
		for(int i=0;i<len;i++)
		{
			alarmStatus status;
			memset(&status,0,sizeof(status));
			CQueueInfo queinfo;
			if(!pThis->m_map_que.Lookup(i,queinfo))continue;
			CString queID = queinfo.GetQueID();
			UINT count = pThis->m_pInlineQueData->GetBussCount(queID);	
			std::map<CString,alarmStatus>::iterator alarmItera;
			alarmItera = pThis->m_map_alarmStatus.find(queID);
			if(alarmItera!=pThis->m_map_alarmStatus.end())//�ҵ���
			{
				if(count>0)
				{
					alarmItera->second.isMaxZero = TRUE;
				}
				else
				{
					alarmItera->second.isZero = TRUE;
					alarmItera->second.isMaxZero = FALSE;
				}
			}
			else
			{
				if(count>0)
				{
					status.isMaxZero = TRUE;
				}
				else
				{
					status.isZero = TRUE;
					status.isMaxZero = FALSE;
				}
				pThis->m_map_alarmStatus[queID] = status;
			}
		}
		///////////////////////////////////////
		std::map<CString,alarmStatus>::iterator itera = pThis->m_map_alarmStatus.begin();
		for(itera;itera!=pThis->m_map_alarmStatus.end();itera++)
		{
			if(itera->second.isMaxZero && itera->second.isZero)
			{
				std::map<UINT,SLZWindow>::const_iterator iteraWindow =
					pThis->m_windowTable.m_mapIdWindow.begin();
				for(iteraWindow;iteraWindow != pThis->m_windowTable.m_mapIdWindow.end();
					iteraWindow++)
				{
					CStringArray queArrary;
					SLZWindow window = iteraWindow->second;
					window.GetArrayQueId(queArrary);
					for(int j=0; j<queArrary.GetCount(); j++)
					{
						if(itera->first == queArrary[j])
						{
							//////////////////////////////alalram
							SLZCallerData callerData;
							callerData.SetCallerId(window.GetCallerId());
							callerData.SetCmdType(callerCmdShowAlarm);
							pThis->m_pCaller->AddWriteCallerData(callerData);
							break;
						}
					}
				}
				itera->second.isZero = FALSE;
				itera->second.isMaxZero = FALSE;
			}
		}
		pThis->m_mtAlarm.Unlock();
		Sleep(200);
	}
}

BOOL SLZController::WriteListQueIntoFile()
{
	m_mtWriteFile.Lock();
	CFile file;
	CTime time = CTime::GetCurrentTime();
	CString strLogFile = time.Format(_T("\\%Y%m%d.dat"));
	if(file.Open(m_MapQuePath+strLogFile,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		int icount = m_list_Data.GetCount();
		for (int i=0;i<icount;i++)
		{
			CArchive ar(&file,CArchive::store);
			SLZData data;
			POSITION pos  = m_list_Data.FindIndex(i);
			if (pos!=NULL)
			{
				data = m_list_Data.GetAt(pos);
				ar<<&data;
				ar.Close();
			}
		}
		file.Close();
	}
	else 
	{
		m_mtWriteFile.Unlock();
		return FALSE;
	}
	m_mtWriteFile.Unlock();
	return TRUE;
}

BOOL SLZController::ReadListQueFromFile()
{
	m_list_Data.RemoveAll();
	int length=0;
	CFile file;
	CFileException e;
	CTime time = CTime::GetCurrentTime();
	CString strLogFile = time.Format(_T("\\%Y%m%d.dat"));
	if (file.Open(m_MapQuePath+strLogFile,CFile::modeRead,&e))
	{
		SLZData* pData = NULL;
		CArchive ar(&file,CArchive::load);
		if (file.GetLength())
		{
			do
			{
				ar>>pData;
				m_list_Data.AddTail(*pData);
				if (pData)
				{
					delete pData;
					pData = NULL;
				}
			}while(!ar.IsBufferEmpty());
		}
		ar.Close();
		file.Close();

	}
	else 
	{
		char c_cmd[125]={0};
		char recordpath[125]={0};
		CCommonConvert::CStringToChar(m_MapQuePath,recordpath);
		sprintf_s(c_cmd,125,"del %s/f/q ",recordpath);
		system(c_cmd);
		return FALSE;
	}
		return TRUE;
}

BOOL SLZController::InsertListData(SLZData data)
{
	int datacount = m_list_Data.GetCount();
	for (int i=0;i<datacount;i++)
	{
		POSITION pos = m_list_Data.FindIndex(i);
		if (pos!=NULL)
		{
			SLZData rdata;
			rdata = m_list_Data.GetAt(pos);
			if (rdata.GetQueSerialID()==data.GetQueSerialID())
			{
				m_list_Data.SetAt(pos,data);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL SLZController::JudgeTodayOrNot(SLZData data)
{
	CTime takeNumTime = data.GetTakingNumTime();
	CTime currTime = CTime::GetCurrentTime();
	if(takeNumTime.GetYear()==currTime.GetYear() && takeNumTime.GetMonth()==currTime.GetMonth()
		&& takeNumTime.GetDay()==currTime.GetDay())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// BOOL SLZController::WriteCacheMsg()
// {
// 	CFile file;
// 	if(file.Open(m_caChePath,CFile::modeCreate | CFile::modeWrite))
// 	{
// 		std::list<CString>::const_iterator itera = theApp.m_list_caCheMsg.begin();
// 		for(itera;itera!=theApp.m_list_caCheMsg.end();itera++)
// 		{
// 			CString temp = *itera;
// 			char tempBuf[MAXRECVBUF] ={0};
// 			CCommonConvert::CStringToChar(temp,tempBuf);
// 			file.Write(tempBuf,MAXRECVBUF);
// 		}
// 		theApp.m_list_caCheMsg.clear();//�������
// 		file.Close();
// 		return TRUE;
// 	}
// 	return FALSE;
// }


// BOOL SLZController::ReadCacheMsg()
// {
// 	CFile file;
// 	if(file.Open(m_caChePath,CFile::modeRead))
// 	{
// 		DWORD length = (DWORD)file.GetLength()/MAXRECVBUF;
// 		for(DWORD i=0;i<length;i++)
// 		{
// 			char msg[MAXRECVBUF] = {0};
// 			if(file.Read(msg,MAXRECVBUF))
// 			{
// 				CString wStrMsg;
// 				CCommonConvert::CharToCstring(wStrMsg,msg);
// 				theApp.m_list_caCheMsg.push_back(wStrMsg);
// 			}
// 		}
// 		file.Close();
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
// 	return TRUE;
// }


BOOL SLZController::ReadCommDaoOrgInfo()
{
	//	if(theApp.m_logicVariables.IsAutoSendToServer)
	//	{
	CProduceClientPacket produce;
	CString wRetPacket = produce.ProduceSearchOrgInfo();
	CComplSocketClient client;
	std::string recvMsg;int actSize = 0;
	BOOL flag = client.SendData(wRetPacket,recvMsg,actSize);
	if(recvMsg.empty())return FALSE;
	else AnaOrgPacket(recvMsg);
	return flag;
	//	}
	//	return FALSE;
}

BOOL SLZController::ReadCommDaoQueInfo()
{
	//	if(theApp.m_logicVariables.IsAutoSendToServer)
	//	{
	CProduceClientPacket produce;
	CString orgID(theApp.m_logicVariables.strOrganID);
	if(!orgID.IsEmpty())
	{
		CString wRetPacket = produce.ProduceSearchQueInfo(orgID);
		CComplSocketClient client;
		std::string recvMsg;int actSize = 0;
		BOOL flag = client.SendData(wRetPacket,recvMsg,actSize);
		if(recvMsg.empty())return FALSE;
		else AnaQuePacket(recvMsg);
		return flag;
	}
	// 		else
	// 		{
	// 			return FALSE;
	// 		}
	// 	}
	return FALSE;
}

BOOL SLZController::ReadCommDaoStaffInfo()
{
	// 	if(theApp.m_logicVariables.IsAutoSendToServer)
	// 	{
	CProduceClientPacket produce;
	CString orgID(theApp.m_logicVariables.strOrganID);
	if(!orgID.IsEmpty())
	{
		CString wRetPacket = produce.ProduceSearchStaffInfo(orgID);
		CComplSocketClient client;
		std::string recvMsg;int actSize = 0;
		BOOL flag = client.SendData(wRetPacket,recvMsg,actSize);
		if(recvMsg.empty())return FALSE;
		else AnaStaffPacket(recvMsg);
		return flag;
	}
	else
	{
		return FALSE;
	}
	// 	}
	// 	return FALSE;
}

BOOL SLZController::ReadCommDaoWndInfo()
{
	// 	if(theApp.m_logicVariables.IsAutoSendToServer)
	// 	{
	CProduceClientPacket produce;
	CString orgID(theApp.m_logicVariables.strOrganID);
	if(!orgID.IsEmpty())
	{
		CString wRetPacket = produce.ProduceSearchWndInfo(orgID);
		CComplSocketClient client;
		std::string recvMsg;int actSize = 0;
		BOOL flag = client.SendData(wRetPacket,recvMsg,actSize);
		if(recvMsg.empty())return FALSE;
		else AnaWndPacket(recvMsg);
		return flag;
	}
	else
	{
		return FALSE;
	}
	// 	}
	// 	return FALSE;
}

void SLZController::AnaOrgPacket(const std::string strPacket)
{
	theApp.m_list_comOrg.clear();
	CommDaoOrg emptyOrg;
	theApp.m_list_comOrg.push_back(emptyOrg);
	/////////////////////////////////
	std::string temp = strPacket;
	std::string::size_type flag = temp.find("<end>");
	while(flag!=strPacket.npos)
	{
		std::string orgInfo=temp.substr(0,flag+5);
		//////////////////////////////////////////
		CommDaoOrg org;
		std::string::size_type firstIndex = orgInfo.find("<parOrgID>");
		std::string::size_type lastIndex = orgInfo.find("</parOrgID>");
		if(firstIndex==orgInfo.npos||lastIndex==orgInfo.npos)break;
		org.parOrgID = orgInfo.substr(firstIndex+10,lastIndex-firstIndex-10);
		firstIndex = orgInfo.find("<curOrgID>");
		lastIndex = orgInfo.find("</curOrgID>");
		if(firstIndex==orgInfo.npos||lastIndex==orgInfo.npos)break;
		org.curOrgID = orgInfo.substr(firstIndex+10,lastIndex-firstIndex-10);
		firstIndex = orgInfo.find("<parOrgName>");
		lastIndex = orgInfo.find("</parOrgName>");
		if(firstIndex==orgInfo.npos||lastIndex==orgInfo.npos)break;
		org.parOrgName = orgInfo.substr(firstIndex+12,lastIndex-firstIndex-12);
		firstIndex = orgInfo.find("<curOrgName>");
		lastIndex = orgInfo.find("</curOrgName>");
		if(firstIndex==orgInfo.npos||lastIndex==orgInfo.npos)break;
		org.curOrgName = orgInfo.substr(firstIndex+12,lastIndex-firstIndex-12);
//		CString wCurOrgID(org.curOrgID.c_str());
//		if(wCurOrgID != theApp.m_logicVariables.strOrganID)
		theApp.m_list_comOrg.push_back(org);
		/////////////////////////////////////////////////////////////
		temp = temp.substr(flag+5);
		flag = temp.find("<end>");
	}
}

void SLZController::AnaQuePacket(const std::string strPacket)
{
	theApp.m_list_comQue.clear();
	std::string temp = strPacket;
	std::string::size_type flag = temp.find("<end>");
	while(flag!=strPacket.npos)
	{
		std::string queInfo=temp.substr(0,flag+5);
		/////////////////////////////////////////////
		CommDaoQue que;
		std::string::size_type firstIndex = queInfo.find("<queID>");
		std::string::size_type lastIndex = queInfo.find("</queID>");
		if(firstIndex==queInfo.npos||lastIndex==queInfo.npos)break;
		que.queID = queInfo.substr(firstIndex+7,lastIndex-firstIndex-7);
		firstIndex = queInfo.find("<orgID>");
		lastIndex = queInfo.find("</orgID>");
		if(firstIndex==queInfo.npos||lastIndex==queInfo.npos)break;
		que.orgID = queInfo.substr(firstIndex+7,lastIndex-firstIndex-7);
		firstIndex = queInfo.find("<queName>");
		lastIndex = queInfo.find("</queName>");
		if(firstIndex==queInfo.npos||lastIndex==queInfo.npos)break;
		que.queName = queInfo.substr(firstIndex+9,lastIndex-firstIndex-9);
		// 		firstIndex = queInfo.find("<onlyID>");
		// 		lastIndex = queInfo.find("</onlyID>");
		// 		if(firstIndex==queInfo.npos||lastIndex==queInfo.npos)break;
		// 		que.onlyID = queInfo.substr(firstIndex+8,lastIndex-firstIndex-8);
		theApp.m_list_comQue.push_back(que);
		/////////////////////////////////////////////
		temp = temp.substr(flag+5);
		flag = temp.find("<end>");
	}
}

void SLZController::AnaStaffPacket(const std::string strPacket)
{
	theApp.m_list_comStaff.clear();
	std::string temp = strPacket;
	std::string::size_type flag = temp.find("<end>");
	while(flag!=strPacket.npos)
	{
		std::string staffInfo=temp.substr(0,flag+5);
		/////////////////////////////////////////////
		CommDaoStaff staff;
		std::string::size_type firstIndex = staffInfo.find("<staffID>");
		std::string::size_type lastIndex = staffInfo.find("</staffID>");
		if(firstIndex==staffInfo.npos||lastIndex==staffInfo.npos)break;
		staff.staffID = staffInfo.substr(firstIndex+9,lastIndex-firstIndex-9);
		firstIndex = staffInfo.find("<orgID>");
		lastIndex = staffInfo.find("</orgID>");
		if(firstIndex==staffInfo.npos||lastIndex==staffInfo.npos)break;
		staff.orgID = staffInfo.substr(firstIndex+7,lastIndex-firstIndex-7);
		firstIndex = staffInfo.find("<staffName>");
		lastIndex = staffInfo.find("</staffName>");
		if(firstIndex==staffInfo.npos||lastIndex==staffInfo.npos)break;
		staff.staffName = staffInfo.substr(firstIndex+11,lastIndex-firstIndex-11);
		// 		firstIndex = staffInfo.find("<onlyID>");
		// 		lastIndex = staffInfo.find("</onlyID>");
		// 		if(firstIndex==staffInfo.npos||lastIndex==staffInfo.npos)break;
		// 		staff.onlyID = staffInfo.substr(firstIndex+8,lastIndex-firstIndex-8);

		theApp.m_list_comStaff.push_back(staff);
		/////////////////////////////////////////////
		temp = temp.substr(flag+5);
		flag = temp.find("<end>");
	}
}

void SLZController::AnaWndPacket(const std::string strPacket)
{
	theApp.m_list_comWnd.clear();
	std::string temp = strPacket;
	std::string::size_type flag = temp.find("<end>");
	while(flag!=strPacket.npos)
	{
		std::string wndInfo=temp.substr(0,flag+5);
		/////////////////////////////////////////////
		CommDaoWnd wnd;
		std::string::size_type firstIndex = wndInfo.find("<wndID>");
		std::string::size_type lastIndex = wndInfo.find("</wndID>");
		if(firstIndex==wndInfo.npos||lastIndex==wndInfo.npos)break;
		wnd.wndID = wndInfo.substr(firstIndex+7,lastIndex-firstIndex-7);
		firstIndex = wndInfo.find("<orgID>");
		lastIndex = wndInfo.find("</orgID>");
		if(firstIndex==wndInfo.npos||lastIndex==wndInfo.npos)break;
		wnd.orgID = wndInfo.substr(firstIndex+7,lastIndex-firstIndex-7);
		firstIndex = wndInfo.find("<wndName>");
		lastIndex = wndInfo.find("</wndName>");
		if(firstIndex==wndInfo.npos||lastIndex==wndInfo.npos)break;
		wnd.wndName = wndInfo.substr(firstIndex+9,lastIndex-firstIndex-9);
		// 		firstIndex = wndInfo.find("<onlyID>");
		// 		lastIndex = wndInfo.find("</onlyID>");
		// 		if(firstIndex==wndInfo.npos||lastIndex==wndInfo.npos)break;
		// 		wnd.onlyID = wndInfo.substr(firstIndex+8,lastIndex-firstIndex-8);
		theApp.m_list_comWnd.push_back(wnd);
		/////////////////////////////////////////////
		temp = temp.substr(flag+5);
		flag = temp.find("<end>");
	}
}

BOOL SLZController::SendQueData()
{
	CString localOrgID(theApp.m_logicVariables.strOrganID);
	CString localOrgName(theApp.m_logicVariables.strOrganNmae);
	if(localOrgID.IsEmpty()||localOrgName.IsEmpty())return FALSE;
	////////////////////////////////////////////////
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		int count = m_map_que.GetCount();
		for(int i=0;i<count;i++)
		{
			CString localBussName = m_map_que[i].GetBussName();
			CString localManNum = m_map_que[i].GetQueID();//����Ψһ���
			if(theApp.m_list_comQue.size()>0)
			{
				BOOL flag = FALSE;
				list<CommDaoQue>::const_iterator itera = theApp.m_list_comQue.begin();
				for(itera;itera!=theApp.m_list_comQue.end();itera++)
				{
					CommDaoQue daoQue = *itera;
					CString bussName(daoQue.queName.c_str());CString queID(daoQue.queID.c_str());
					if(localBussName == bussName && localManNum == queID)
					{
						flag = TRUE;
						break;
					}
				}
				if(!flag)
				{
					CProduceClientPacket produce;
					CString packet = produce.ProduceQuePacket(localManNum,localBussName,theApp.m_logicVariables.strOrganID);
					CComplSocketClient client;
					std::string recvMsg;
					int size = 0;
					client.SendData(packet,recvMsg,size);
				}
			}
			else
			{
				CProduceClientPacket produce;
				CString packet = produce.ProduceQuePacket(localManNum,localBussName,theApp.m_logicVariables.strOrganID);
				CComplSocketClient client;
				std::string recvMsg;
				int size = 0;
				client.SendData(packet,recvMsg,size);
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL SLZController::SendWndData()
{
	CString localOrgID(theApp.m_logicVariables.strOrganID);
	CString localOrgName(theApp.m_logicVariables.strOrganNmae);
	if(localOrgID.IsEmpty()||localOrgName.IsEmpty())return FALSE;
	///////////////////////////////////////////////////
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		map<UINT,SLZWindow>::const_iterator itera = m_windowTable.m_mapIdWindow.begin();
		for(itera;itera!=m_windowTable.m_mapIdWindow.end();itera++)
		{
			CString localShowID;localShowID.Format(_T("%d"),itera->second.GetWindowId());//����Ψһ���
			CString localWndName=itera->second.GetWindowName(); 
			if(theApp.m_list_comWnd.size()>0)
			{
				BOOL flag = FALSE;
				list<CommDaoWnd>::const_iterator daoItera = theApp.m_list_comWnd.begin();
				for(daoItera;daoItera!=theApp.m_list_comWnd.end();daoItera++)
				{
					CommDaoWnd daoWnd = *daoItera;
					CString wndShowID(daoWnd.wndID.c_str());CString wndName(daoWnd.wndName.c_str());

					if(localShowID==wndShowID && localWndName==wndName)
					{
						flag = TRUE;
						break;
					}
				}
				if(!flag)
				{
					CProduceClientPacket produce;
					CString packet = produce.ProduceWndPacket(localShowID,localWndName,
						theApp.m_logicVariables.strOrganID);
					CComplSocketClient client;
					std::string recvMsg;
					int size = 0;
					client.SendData(packet,recvMsg,size);
				}
			}
			else
			{
				CProduceClientPacket produce;
				CString packet = produce.ProduceWndPacket(localShowID,localWndName,
					theApp.m_logicVariables.strOrganID);
				CComplSocketClient client;
				std::string recvMsg;
				int size = 0;
				client.SendData(packet,recvMsg,size);
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL SLZController::SendStaffData()
{
	CString localOrgID(theApp.m_logicVariables.strOrganID);
	CString localOrgName(theApp.m_logicVariables.strOrganNmae);
	if(localOrgID.IsEmpty()||localOrgName.IsEmpty())return FALSE;
	///////////////////////////////////////////////////
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		map<CString,SLZStaff*>::const_iterator itera = m_staffTable.m_mapIdStaff.begin();
		for(itera;itera!=m_staffTable.m_mapIdStaff.end();itera++)
		{
			CString localStaffID = itera->first;CString localStaffName = itera->second->GetStaffName();
			if(theApp.m_list_comStaff.size()>0)
			{
				BOOL flag = FALSE;
				list<CommDaoStaff>::const_iterator daoItear = theApp.m_list_comStaff.begin();
				for(daoItear;daoItear!=theApp.m_list_comStaff.end();daoItear++)
				{
					CString daoStaffID(daoItear->staffID.c_str());CString daoStaffName(daoItear->staffName.c_str());
					if(localStaffName==daoStaffName && localStaffID==daoStaffID)
					{
						flag = TRUE;
						break;
					}
				}
				if(!flag)
				{
					CProduceClientPacket produce;
					CString packet = produce.ProduceStaffPacket(localStaffID,localStaffName,
						theApp.m_logicVariables.strOrganID);
					CComplSocketClient client;
					std::string recvMsg;
					int size = 0;
					client.SendData(packet,recvMsg,size);
				}
			}
			else
			{
				CProduceClientPacket produce;
				CString packet = produce.ProduceStaffPacket(localStaffID,localStaffName,
					theApp.m_logicVariables.strOrganID);
				CComplSocketClient client;
				std::string recvMsg;
				int size = 0;
				client.SendData(packet,recvMsg,size);
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL SLZController::SendOrgData()
{
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		CString localOrgName = theApp.m_logicVariables.strOrganNmae;
		CString localOrgID = theApp.m_logicVariables.strOrganID;
		CString localParOrgID = theApp.m_logicVariables.strParOrgID;
		CString localParOrgName = theApp.m_logicVariables.strParOrgName;
		if(localOrgID.IsEmpty()||localOrgName.IsEmpty())return FALSE;
		if(JudgeParOrgIsAt())return FALSE;
		if(theApp.m_list_comOrg.size()>0)
		{
			BOOL flag = FALSE;
			list<CommDaoOrg>::const_iterator daoItear = theApp.m_list_comOrg.begin();
			for(daoItear;daoItear!=theApp.m_list_comOrg.end();daoItear++)
			{
				CString orgID(daoItear->curOrgID.c_str());CString orgName(daoItear->curOrgName.c_str());
				CString parOrgID(daoItear->parOrgID.c_str());CString parOrgName(daoItear->parOrgName.c_str());
				if(localOrgName==orgName && localOrgID==orgID && parOrgID==localParOrgID && parOrgName==localParOrgName)
				{
					flag = TRUE;
					break;
				}
			}
			if(!flag)
			{
				CProduceClientPacket produce;
				CString packet = produce.ProduceOrgPacket(localOrgID,localOrgName,localParOrgID,localParOrgName);
				CComplSocketClient client;
				std::string recvMsg;
				int size = 0;
				client.SendData(packet,recvMsg,size);
			}
		}
	}
	return TRUE;
}

BOOL SLZController::SendDelQue()
{
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		list<CommDaoQue>::const_iterator itera = theApp.m_list_comQue.begin();
		for(itera;itera!=theApp.m_list_comQue.end();itera++)
		{
			BOOL flag = FALSE;
			CString comQueNum(itera->queID.c_str());
			for(int i=0;i<m_map_que.GetCount();i++)
			{
				if(m_map_que[i].GetQueID()==comQueNum)
				{
					flag = TRUE;
					break;
				}
			}
			if(!flag)
			{
				CProduceClientPacket produce;
				CString wOrgID(itera->orgID.c_str());
				CString packet = produce.ProduceDelQue(comQueNum,wOrgID);
				CComplSocketClient client;
				std::string recvMsg;
				int size = 0;
				client.SendData(packet,recvMsg,size);
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL SLZController::SendDelStaff()
{
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		list<CommDaoStaff>::const_iterator itera = theApp.m_list_comStaff.begin();
		for(itera;itera!=theApp.m_list_comStaff.end();itera++)
		{
			BOOL flag = FALSE;
			CString wStaffID(itera->staffID.c_str());
			if(m_staffTable.QueryStaffById(wStaffID)!=NULL)
			{
				flag = TRUE;
			}
			if(!flag)
			{
				CProduceClientPacket produce;
				CString wOrgID(itera->orgID.c_str());
				CString packet = produce.ProduceDelStaff(wStaffID,wOrgID);
				CComplSocketClient client;
				std::string recvMsg;
				int size = 0;
				client.SendData(packet,recvMsg,size);
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL SLZController::SendDelWnd()
{
	if(theApp.m_logicVariables.IsAutoSendToServer)
	{
		list<CommDaoWnd>::const_iterator itera = theApp.m_list_comWnd.begin();
		for(itera;itera!=theApp.m_list_comWnd.end();itera++)
		{
			CString wWndID(itera->wndID.c_str());
			BOOL flag = FALSE;
			map<UINT,SLZWindow>::const_iterator localItera = m_windowTable.m_mapIdWindow.begin();
			for(localItera;localItera!=m_windowTable.m_mapIdWindow.end();localItera++)
			{
				CString wLocalShowID;
				wLocalShowID.Format(_T("%d"),localItera->second.GetWindowId());
				if(wWndID == wLocalShowID)
				{
					flag = TRUE;
					break;
				}
			}
			if(!flag)
			{
				CProduceClientPacket produce;
				CString wOrgID(itera->orgID.c_str());
				CString packet = produce.ProduceDelWnd(wWndID,wOrgID);
				CComplSocketClient client;
				std::string recvMsg;
				int size = 0;
				client.SendData(packet,recvMsg,size);
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL SLZController::JudgeParOrgIsAt()
{
	CString localOrgID = theApp.m_logicVariables.strOrganID;
	CString localParOrgID = theApp.m_logicVariables.strParOrgID;
	//	CString localParOrgName = theApp.m_logicVariables.strParOrgName;
	if(theApp.m_list_comOrg.size()>0)
	{
		list<CommDaoOrg>::const_iterator daoItear = theApp.m_list_comOrg.begin();
		for(daoItear;daoItear!=theApp.m_list_comOrg.end();daoItear++)
		{
			CString orgID(daoItear->curOrgID.c_str());
			CString parOrgID(daoItear->parOrgID.c_str());
			if(localOrgID == localParOrgID)
				//�������Լ�Ϊ�Լ��ĸ�������//�ұ��ػ������ܺ͸�������ID��ͬ
			{
				return TRUE;
			}
			if(localOrgID == orgID && localParOrgID == parOrgID)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL SLZController::InitLoginMap()
{
	if(m_windowTable.m_mapIdWindow.size()==0)return FALSE;
	std::map<UINT,SLZWindow>::const_iterator itera = m_windowTable.m_mapIdWindow.begin();
	SLZWindow Window;
	for(itera;itera!=m_windowTable.m_mapIdWindow.end();itera++)
	{
		//		m_map_que[pWindow->GetWindowId()] = pWindow->GetLoginStaff();
		Window = itera->second;
		CString staffID = Window.GetLoginStaff();//Ĭ�ϵ�¼staffID
		std::map<UINT,CString>::const_iterator itera;
		itera = m_mapLoginList.find(Window.GetWindowId());
		if(itera != m_mapLoginList.end())//�ҵ���.
		{
			CString mapStaffID = itera->second;
			if(!staffID.IsEmpty())//��Ϊ�գ�������Ĭ�ϵ�¼
			{
				if(mapStaffID != staffID)
				{
					m_mapLoginList[Window.GetWindowId()] = staffID;
				}
			}
			else//û����Ĭ�ϵ�¼
			{
				//				theApp.m_Controller.m_mapLoginList[pWindow->GetWindowId()] = _T("");
			}
		}
		else//û�ҵ�
		{
			if(!staffID.IsEmpty())
				m_mapLoginList[Window.GetWindowId()] = staffID;
		}
	}
	return TRUE;
}

CString SLZController::GetStaffNameByID(const CString& staffID)
{
	SLZStaff* pStaff = m_staffTable.QueryStaffById(staffID);
	if(!pStaff)return _T("");
	return pStaff->GetStaffName();
}

CString SLZController::GetWindowNameByID(UINT nWindowID)
{
	SLZWindow slzWindow;
	m_windowTable.QueryWindowById(nWindowID,slzWindow);
	return slzWindow.GetWindowName();
}

CString SLZController::GetWindowCallNameByID(UINT nWindowID)
{
	SLZWindow slzWindow;
	m_windowTable.QueryWindowById(nWindowID,slzWindow);
	return slzWindow.GetWindowCallName();
}