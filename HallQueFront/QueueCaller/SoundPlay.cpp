#include "StdAfx.h"
#include "SoundPlay.h"
#include "CommonStrMethod.h"
#include <Mmsystem.h>
#include <Shlwapi.h>

#pragma comment(lib, "WINMM.LIB")
CSoundPlay::CSoundPlay(void):m_pPlayVoiceThread(NULL)
{
	m_strWavLibPath = CommonStrMethod::GetModuleDir() + _T("wavLib\\");
	m_strCallPath = CommonStrMethod::GetModuleDir();
	m_strCallPath+=_T("\\config");
	CommonStrMethod::CreatePath(m_strCallPath);
	m_strCallPath+=_T("\\CallerSet.ini");
}

CSoundPlay::~CSoundPlay(void)
{
	if(m_pPlayVoiceThread)
	{
		WaitForSingleObject(m_pPlayVoiceThread->m_hThread,20);
		TerminateThread(m_pPlayVoiceThread->m_hThread,0);
		delete m_pPlayVoiceThread;
		m_pPlayVoiceThread = NULL;
	}
}


BOOL CSoundPlay::Init()
{
	BOOL bRet = ::PathFileExists(m_strWavLibPath);
	if (!bRet)
	{
		CString str;
		str.Format(_T("语音库路径不存在:%s"), m_strWavLibPath);
		AfxMessageBox(str);

		return FALSE;
	}
	//

	CFileFind finder;
	BOOL bFound = finder.FindFile(CommonStrMethod::GetModuleDir() + _T("wavLib\\*.wav"));
	while(bFound)
	{
		bFound = finder.FindNextFile();
		m_WavList.AddTail(finder.GetFileTitle());
	}
	finder.Close();
	m_pPlayVoiceThread = AfxBeginThread(PlayVoiceThread, this);
	
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CallSet"),_T("CallMsg"),NULL,wbuf,255,m_strCallPath);
	CString strCallMsg(wbuf);
	m_strCallMsg  = strCallMsg;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CallSet"),_T("ShowMsg"),NULL,wbuf,255,m_strCallPath);
	CString strShowMsg(wbuf);
	m_strShowMsg = strShowMsg;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("CallName"),NULL,wbuf,255,m_strCallPath);
	CString strWndName(wbuf);
	m_strWndName = strWndName;
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("WndId"),NULL,wbuf,255,m_strCallPath);
	CString strWndId(wbuf);
	m_iScreenId = CommonStrMethod::Str2Int(strWndId);
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("CompSet"),_T("PlayTimes"),NULL,wbuf,255,m_strCallPath);
	CString strPlaytimes(wbuf);
	m_iPlayTimes = CommonStrMethod::Str2Int(strPlaytimes);

	return TRUE;
}

BOOL CSoundPlay::DataPlay(const SLZData& Data,BOOL bWait)
{
	VOICEDISPLAYSTR vst = {0};
		if (bWait)
		{
			vst.strVoiceStr = ChangeWaitCallStr(Data);
			vst.strDisplayStr = ChangeWaitShowStr(Data);
		}
		else
		{
			vst.strVoiceStr = ChangeCallStr(Data);
			vst.strDisplayStr = ChangeShowStr(Data);
		}
		/////获取不同屏幕地址////

		vst.iWndScreenId = m_iScreenId;
		AddPlayText(vst);				//加入播放队列
		return TRUE;
}

BOOL CSoundPlay::AddPlayText(VOICEDISPLAYSTR& vdstr)
{
	if(vdstr.strVoiceStr.IsEmpty())
	{
		return FALSE;
	}
	m_mtPlayStrList.Lock();
	if(!m_PlayDataList.IsEmpty())
	{
		if(vdstr.strVoiceStr==m_PlayDataList.GetTail().strVoiceStr)
		{
			m_mtPlayStrList.Unlock();
			return FALSE;
		}
	}
	m_PlayDataList.AddTail(vdstr);
	m_mtPlayStrList.Unlock();
	return TRUE;
}

UINT CSoundPlay::PlayVoiceThread(LPVOID pParam)
{
	CSoundPlay* pThis = (CSoundPlay*)pParam;
	pThis->m_bPlayVoiceThread = TRUE;

	while(pThis->m_bPlayVoiceThread)
	{
		if(pThis->m_PlayDataList.GetCount())
		{
			pThis->m_mtPlayStrList.Lock();
			VOICEDISPLAYSTR PlayStr = pThis->m_PlayDataList.GetHead();
			pThis->m_PlayDataList.RemoveHead();
			pThis->m_mtPlayStrList.Unlock();
			if (PlayStr.iWndScreenId!=0)
			{
				//ShowMsgTime showtime={0};   //初始化计时结构体
				//showtime.iWndid = PlayStr.iWndid;
				//showtime.strAd = PlayStr.strAd;
				if (PlayStr.iWndScreenId!=0)
				{
					//SLZCWndScreen* WndScreen = SLZCWndScreen::GetInstance();
					//WndScreen->AddScreenMsg(PlayStr.strDisplayStr,PlayStr.iWndScreenId);
					//showtime.address = PlayStr.iWndScreenId;

				}
				//if (PlayStr.iLEDPhyId!=0&&PlayStr.iLEDPipeId!=0)
				//{
				//	SLZCWndScreen* WndScreen = SLZCWndScreen::GetInstance();
				//	WndScreen->AddThroughScreenMsg(PlayStr.strDisplayStr,PlayStr.iLEDPhyId,PlayStr.iLEDPipeId);
				//	showtime.LedAddress = PlayStr.iLEDPhyId;
				//	showtime.LedPipe = PlayStr.iLEDPipeId;
				//}
				//if (PlayStr.iWndComId!=0)
				//{
				//	SLZCWndScreen* WndScreen = SLZCWndScreen::GetInstance();
				//	WndScreen->AddScreenMsg(PlayStr.strDisplayStr,PlayStr.iWndComId);
				//	showtime.ComId = PlayStr.iWndComId;
				//}
				//ShowMsgTime checkshow={0};		//检查结构体
				//pThis->m_mtShowMsgList.Lock();
				//int length = pThis->m_MsgShowList.GetCount();
				//for (int i =0;i<length;i++)
				//{
				//	POSITION posFind = pThis->m_MsgShowList.FindIndex(i);
				//	if(!posFind)break;
				//	checkshow = pThis->m_MsgShowList.GetAt(posFind);
				//	if (checkshow.iWndid==showtime.iWndid)
				//	{
				//		pThis->m_MsgShowList.RemoveAt(posFind);			//如果
				//	}
				//}
				//pThis->m_MsgShowList.AddTail(showtime);
				//pThis->m_mtShowMsgList.Unlock();
			}
			//////////////////////声音播放//////////////////////////
			if (!PlayStr.strVoiceStr.IsEmpty())
			{
				for (int i=0;i<pThis->m_iPlayTimes;i++)
				{
					CStringArray soundarray;
					CommonStrMethod::StrSplit(PlayStr.strVoiceStr,soundarray,_T("#"));
					for (int j=0;j<soundarray.GetCount();j++)
					{
						CString strjts = soundarray.GetAt(j);
						pThis->PlayTheVoice(strjts,pThis->m_WavList);
					}
				}
			}
		}
		//pThis->m_mtThreadPlayVoice.Unlock();
		else
		{
			Sleep(20);
		}
	}
	::ExitThread(0);

	return 0;
}

CString CSoundPlay::ChangeCallStr(const SLZData& data)
{
	CString strPlaySound = m_strCallMsg;
	CString strQueNum = data.GetQueueNumber();
	CString strCallName = m_strWndName;
	CString strBussName = data.GetBussName();
	
		CStringArray strArr;
		SplitStrToNumArray(strCallName,strArr);   ///分割呼叫名称汉字与非汉字

		//if (theApp.m_logicVariables.iNumberCallType==2)   ///////////////一二三模式
		//{
			InsertSplit(strQueNum,_T("#"));
			strPlaySound.Replace(_T("[排队号码]"),strQueNum);
			for (int i=0;i<strArr.GetCount();i++)
			{
				CString strOrigin = strArr.GetAt(i);
				if (StrHasNum(strOrigin))							/////////只转换呼叫名称中的数字
				{
					InsertSplit(strOrigin,L"#");
				}
				strArr.SetAt(i,strOrigin);
			}
			CString strChange = InsertSplitToArray(strArr,L"#");
			strPlaySound.Replace(_T("[呼叫名称]"),strChange);
		//}
		//if (theApp.m_logicVariables.iNumberCallType==1)         /////一百二十三模式
		//{
		//	CStringArray strarr;
		//	SplitCusNumToStrArr(strQueNum,strarr);
		//	CString strChangeNum;
		//	strChangeNum = InsertSplitToArray(strarr,L"#");                      ////分好的数组中插入#号
		//	strPlaySound.Replace(_T("[排队号码]"),strChangeNum);

		//	for (int i=0;i<strArr.GetCount();i++)
		//	{
		//		CString strOrigin = strArr.GetAt(i);
		//		if (StrHasNum(strOrigin))
		//		{
		//			CStringArray strarr;
		//			SplitCusNumToStrArr(strOrigin,strarr);
		//			strArr.RemoveAt(i);
		//			strArr.InsertAt(i,&strarr);
		//			i += strarr.GetCount()-1;				
		//		}
		//	}
		//	CString strChangeCall = InsertSplitToArray(strArr,L"#");
		//	strPlaySound.Replace(_T("[呼叫名称]"),strChangeCall);
		//}
	strPlaySound.Replace(_T("[业务名称]"),strBussName);
	CString strQueNumEng;
	CString strWndNameEng;
	CString strWndNameNum = RemoveChinese(strCallName);
	strQueNum = data.GetQueueNumber();
	InsertSplit(strQueNum,L"#");
	InsertSplit(strWndNameNum,L"#");

		strQueNumEng = InsertSplitEng(strQueNum);
		strWndNameEng = InsertSplitEng(strWndNameNum);
	strPlaySound.Replace(_T("[排队号码(英)]"),strQueNumEng);
	strPlaySound.Replace(_T("[呼叫名称(英)]"),_T("number#")+strWndNameEng+_T("#window"));
	return strPlaySound;
}

CString CSoundPlay::ChangeShowStr(const SLZData& data)
{
	CString strPlaySound = m_strCallMsg;
	strPlaySound.Replace(_T("[排队号码]"),data.GetQueueNumber());
	strPlaySound.Replace(_T("[客户姓名]"),data.GetCustName());
	strPlaySound.Replace(_T("[窗口名称]"),m_strWndName);
	strPlaySound.Replace(_T("[业务名称]"),data.GetBussName());
	CString strwndname = m_strWndName;
	strwndname.Remove(_T('号'));
	strwndname.Remove(_T('窗'));
	strwndname.Remove(_T('口'));
	strPlaySound.Replace(_T("[排队号码(英)]"),data.GetQueueNumber());
	strPlaySound.Replace(_T("[窗口名称(英)]"),_T("No.")+strwndname+_T(" window"));
	return strPlaySound;
}

CString CSoundPlay::ChangeWaitCallStr(const SLZData& data)
{
	CString strPlaySound = m_strCallMsg;
	CString strQueNum = data.GetQueueNumber();
	CString strCallName = m_strWndName;
	CString strBussName = data.GetBussName();
	//if (theApp.m_logicVariables.IsUseJtts)						/////使用JTTS语音库
	//{
	//	if (theApp.m_logicVariables.iNumberCallType==2)   ////////一二三模式直接替换
	//	{
	//		strPlaySound.Replace(_T("[排队号码]"),strQueNum);
	//		strPlaySound.Replace(_T("[呼叫名称]"),strCallName);
	//	}
	//	if (theApp.m_logicVariables.iNumberCallType==1)
	//	{
	//		CStringArray strarr;
	//		SplitCusNumToStrArr(strQueNum,strarr);    ////转换排队号码
	//		if (strarr.GetCount()>0)
	//		{
	//			CString strQuenumChange = strarr[0];
	//			for (int i=1;i<strarr.GetCount();i++)
	//			{
	//				strQuenumChange += _T("#")+strarr[i];
	//			}
	//			strPlaySound.Replace(_T("[排队号码]"),strQuenumChange);
	//		}
	//		CStringArray strarr2;
	//		SplitCusNumToStrArr(strCallName,strarr2);   /////////转换呼叫名称
	//		if (strarr2.GetCount()>0)
	//		{
	//			CString strWndNameChange = strarr2[0];
	//			for (int i=1;i<strarr2.GetCount();i++)
	//			{
	//				strWndNameChange += _T("#")+strarr2[i];
	//			}
	//			strPlaySound.Replace(_T("[呼叫名称]"),strWndNameChange);
	//		}
	//	}
	//}
/*	else
	{*/			
		CStringArray strArr;
		SplitStrToNumArray(strCallName,strArr);   ///分割呼叫名称汉字与非汉字

		//if (theApp.m_logicVariables.iNumberCallType==2)   ///////////////一二三模式
		//{
			InsertSplit(strQueNum,_T("#"));
			strPlaySound.Replace(_T("[排队号码]"),strQueNum);
			for (int i=0;i<strArr.GetCount();i++)
			{
				CString strOrigin = strArr.GetAt(i);   
				if (StrHasNum(strOrigin))							/////////只转换呼叫名称中的数字
				{
					InsertSplit(strOrigin,L"#");
				}
				strArr.SetAt(i,strOrigin);
			}
			CString strChange = InsertSplitToArray(strArr,L"#");
			strPlaySound.Replace(_T("[呼叫名称]"),strChange);
		/*}*/
		//if (theApp.m_logicVariables.iNumberCallType==1)         /////一百二十三模式
		//{
		//	CStringArray strarr;
		//	SplitCusNumToStrArr(strQueNum,strarr);
		//	CString strChangeNum;
		//	strChangeNum = InsertSplitToArray(strarr,L"#");                      ////分好的数组中插入#号
		//	strPlaySound.Replace(_T("[排队号码]"),strChangeNum);

		//	for (int i=0;i<strArr.GetCount();i++)
		//	{
		//		CString strOrigin = strArr.GetAt(i);
		//		if (StrHasNum(strOrigin))
		//		{
		//			CStringArray strarr;
		//			SplitCusNumToStrArr(strOrigin,strarr);
		//			strArr.RemoveAt(i);
		//			strArr.InsertAt(i,&strarr);
		//			i += strarr.GetCount()-1;

		//		}
		//	}
		//	CString strChangeCall = InsertSplitToArray(strArr,L"#");
		//	strPlaySound.Replace(_T("[呼叫名称]"),strChangeCall);
		//}
	//}
	strPlaySound.Replace(_T("[业务名称]"),strBussName);
	CString strQueNumEng;
	CString strWndNameEng;
	CString strWndNameNum = RemoveChinese(strCallName);
	strQueNum = data.GetQueueNumber();
	InsertSplit(strQueNum,L"#");
	InsertSplit(strWndNameNum,L"#");
	//if (theApp.m_logicVariables.IsUseJtts)
	//{
	//	strQueNumEng= InsertJttsEng(strQueNum);
	//	strWndNameEng = InsertJttsEng(strWndNameNum);
	//}
	//else
	//{
		strQueNumEng = InsertSplitEng(strQueNum);
		strWndNameEng = InsertSplitEng(strWndNameNum);
	//}
	strPlaySound.Replace(_T("[排队号码(英)]"),strQueNumEng);
	strPlaySound.Replace(_T("[呼叫名称(英)]"),_T("number#")+strWndNameEng+_T("#window"));
	return strPlaySound;
}

CString CSoundPlay::ChangeWaitShowStr(const SLZData& data)
{
	CString strPlaySound = m_strCallMsg;
	strPlaySound.Replace(_T("[排队号码]"),data.GetQueueNumber());
	strPlaySound.Replace(_T("[客户姓名]"),data.GetCustName());
	strPlaySound.Replace(_T("[窗口名称]"),m_strWndName);
	strPlaySound.Replace(_T("[业务名称]"),data.GetBussName());
	CString strwndname = m_strWndName;
	strwndname.Remove(_T('号'));
	strwndname.Remove(_T('窗'));
	strwndname.Remove(_T('口'));
	strPlaySound.Replace(_T("[排队号码(英)]"),data.GetQueueNumber());
	strPlaySound.Replace(_T("[窗口名称(英)]"),_T("No.")+strwndname+_T(" window"));
	return strPlaySound;
}

void CSoundPlay::InsertSplit(CString &strinsert,CString strsplt)
{
	CString s1;
	int len = (strinsert.GetLength() -1);
	for(int i=len;i>0;i--)
	{
		s1 = strinsert.Right(strinsert.GetLength()-i);
		strinsert = strinsert.Left(i) + strsplt;
		strinsert += s1;
	}
}

CString CSoundPlay::InsertSplitEng(CString strinsert)
{
	strinsert.Replace(_T("0"),_T("e0"));
	strinsert.Replace(_T("1"),_T("e1"));
	strinsert.Replace(_T("2"),_T("e2"));
	strinsert.Replace(_T("3"),_T("e3"));
	strinsert.Replace(_T("4"),_T("e4"));
	strinsert.Replace(_T("5"),_T("e5"));
	strinsert.Replace(_T("6"),_T("e6"));
	strinsert.Replace(_T("7"),_T("e7"));
	strinsert.Replace(_T("8"),_T("e8"));
	strinsert.Replace(_T("9"),_T("e9"));
	return strinsert;
}

CString CSoundPlay::InsertJttsEng(CString strinsert)
{
	strinsert.Replace(_T("0"),_T("zero"));
	strinsert.Replace(_T("1"),_T("one"));
	strinsert.Replace(_T("2"),_T("two"));
	strinsert.Replace(_T("3"),_T("three"));
	strinsert.Replace(_T("4"),_T("four"));
	strinsert.Replace(_T("5"),_T("five"));
	strinsert.Replace(_T("6"),_T("six"));
	strinsert.Replace(_T("7"),_T("seven"));
	strinsert.Replace(_T("8"),_T("eight"));
	strinsert.Replace(_T("9"),_T("nine"));
	return strinsert;
}


BOOL CSoundPlay::StrHasNum(CString strOrigin)
{
	int iCount = strOrigin.GetLength();
	for (int i=0;i<iCount;i++)
	{
		if (strOrigin[i]>'0'&&strOrigin[i]<'9')
		{
			return TRUE;
		}		
	}
	return FALSE;
}

CString CSoundPlay::RemoveChinese(CString strOrigin)
{
	for (int i=0;i<strOrigin.GetLength();i++)
	{
		if ((strOrigin[i]>='0'&&strOrigin[i]<='9')||(strOrigin[i]>='a'&&strOrigin[i]<='z')||(strOrigin[i]>='A'&&strOrigin[i]<='Z'))
		{
			continue;
		}
		else
		{
			int iChange = strOrigin.Remove(strOrigin[i]);
			i--;
		}
	}
	return strOrigin;
}

int CSoundPlay::SplitStrToCharArray(const CString& strSrc, CStringArray& strarrDest)
{
	int i = 0;
	for(; i < strSrc.GetLength(); i++)
	{
		CString strChar;
		strChar.Format(_T("%c"), strSrc.GetAt(i));
		strarrDest.Add(strChar);
	}
	return i;
}

int CSoundPlay::SplitCusNumToStrArr(const CString& strSrc, CStringArray& strarrDest)
{
	int i = 0;
	CString strNumTemp;
	for(; i < strSrc.GetLength(); i++)
	{
		if(strSrc.GetAt(i) < '0'  || strSrc.GetAt(i) > '9')
		{
			if(!strNumTemp.IsEmpty())
			{
				if(strNumTemp.GetLength() > 1)
				{
					CStringArray strarr;
					SplitNumStrToNumbers(strNumTemp, strarr);
					for(int j = 0; j < strarr.GetCount(); j++)
					{
						strarrDest.Add(strarr[j]);
					}
				}
				else
				{
					strarrDest.Add(strNumTemp);
				}
				strNumTemp.Empty();
			}
			CString strChar;
			strChar.Format(_T("%c"), strSrc.GetAt(i));
			strarrDest.Add(strChar);
		}
		else
		{
			strNumTemp += strSrc.GetAt(i);
		}
	}
	if(!strNumTemp.IsEmpty())
	{
		if(strNumTemp.GetLength() > 1)
		{
			CStringArray strarr;
			SplitNumStrToNumbers(strNumTemp, strarr);
			for(int j = 0; j < strarr.GetCount(); j++)
			{
				strarrDest.Add(strarr[j]);
			}
		}
		else
		{
			strarrDest.Add(strNumTemp);
		}
		strNumTemp.Empty();
	}

	return i;
}

int CSoundPlay::SplitNumStrToNumbers(const CString& strNumSrc, CStringArray& strarrDest)
{
	if(strNumSrc.IsEmpty())
	{
		return -1;
	}
	for(int i = 0; i < strNumSrc.GetLength(); i++)
	{
		if( strNumSrc.GetAt(i) < '0' || strNumSrc.GetAt(i) > '9')
		{
			return -1;
		}
	}
	CString strTemp = strNumSrc;
	int nCount = 0;
	for(; !strTemp.IsEmpty(); nCount++)
	{
		if(strTemp.GetAt(0) == '0')
		{
			strTemp = strTemp.Mid(1);
			//strarrDest.Add(_T("0"));
			continue;
		}
		CString strNumPart;
		strNumPart.Format(_T("%c"), strTemp.GetAt(0));
		for(int i = 0; i < strTemp.GetLength()-1; i++)
		{
			strNumPart += _T("0");
		}
		strarrDest.Add(strNumPart);
		if(strNumPart == strTemp)
		{
			break;
		}
		strTemp = strTemp.Mid(1);
		if(strTemp.GetAt(0) == '0' && strarrDest[nCount] != _T("0"))
		{
			strarrDest.Add(_T("0"));
		}
	}

	return nCount;
}

CString CSoundPlay::InsertSplitToArray(CStringArray&strarrOrigin,CString strSplit)
{
	CString strChange;
	for (int i=0;i<strarrOrigin.GetCount();i++)
	{
		if (i!=strarrOrigin.GetCount()-1)
		{
			strChange +=strarrOrigin.GetAt(i)+L"#";
		}
		else strChange +=strarrOrigin.GetAt(i);
	}
	return strChange;
}

int CSoundPlay::SplitStrToNumArray(CString strSrc,CStringArray&strarrDest)
{
	while (strSrc.GetLength())
	{
		CString element = Getthesame(strSrc);
		strSrc = strSrc.Right(strSrc.GetLength()-element.GetLength());
		strarrDest.Add(element);
	}
	return strarrDest.GetCount();
}

CString CSoundPlay::Getthesame(CString strSrc)
{
	CString strSame;
	if (strSrc[0]>=255)
	{
		while (strSrc.GetLength())
		{
			if (strSrc[0]>=255)
			{
				strSame += strSrc[0];
				strSrc = strSrc.Right(strSrc.GetLength()-1);
			}
			else return strSame;
		}
		return strSame;
	}
	else
	{
		while (strSrc.GetLength())
		{
			if (strSrc[0]<255)
			{
				strSame += strSrc[0];
				strSrc = strSrc.Right(strSrc.GetLength()-1);
			}
			else return strSame;
		}
		return strSame;
	}
}

BOOL CSoundPlay::PlayTheVoice(CString strVoice,const CStringList& WavList)
{
	/*m_mtPlayStrList.Lock();*/
	for (int i=0;i<WavList.GetCount();i++)
	{
		POSITION pos = WavList.FindIndex(i);
		if (pos!=NULL)
		{
			if (WavList.GetAt(pos)==strVoice)
			{
				//m_mtPlayStrList.Unlock();
				CString sound = m_strWavLibPath+strVoice+_T(".wav");
				return ::sndPlaySound(sound,SND_NODEFAULT | SND_SYNC);				
			}
		}
	}
	if (strVoice.GetLength()>1)
	{
		while (strVoice.GetLength())
		{
			CString strPlayResult = PlayTheVoiceFromRight(strVoice,m_WavList);
			if (strPlayResult.IsEmpty())
			{
				strVoice = strVoice.Right(strVoice.GetLength()-1);
			}
			else
			{
				int iChange = strPlayResult.GetLength();
				strVoice = strVoice.Right(strVoice.GetLength()-iChange);
			}
		}
	}
	return TRUE;
}

CString CSoundPlay::PlayTheVoiceFromRight(CString strVoice,const CStringList& WavList)
{
	while (strVoice.GetLength())
	{
		for (int i=0;i<WavList.GetCount();i++)
		{
			POSITION pos = WavList.FindIndex(i);
			if (pos!=NULL)
			{
				if (WavList.GetAt(pos)==strVoice)
				{
					CString sound = m_strWavLibPath+strVoice+_T(".wav");
					::sndPlaySound(sound,SND_NODEFAULT | SND_SYNC);				
					return strVoice;
				}
			}			
		}
		strVoice = strVoice.Left(strVoice.GetLength()-1);
	}
	return L"";
}