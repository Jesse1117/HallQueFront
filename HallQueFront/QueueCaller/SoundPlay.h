#pragma once
#include "SLZData.h"
#include <afxwin.h>
#include <afxmt.h>
typedef struct _tagVoiceAndDisplayStr
{
	CString		strAd;		//广告
	CString		strVoiceStr;	//声音内容
	CString		strDisplayStr;	//显示内容
	int			iWndScreenId;		//窗口屏地址
} VOICEDISPLAYSTR, *LPVOICEDISPLAYSTR;

class CSoundPlay
{
private:
	CSoundPlay(void);
public:
	static CSoundPlay* GetInstance()
	{
		static CSoundPlay Instance;
		return &Instance;
	}
	~CSoundPlay(void);

	BOOL Init();
	//BOOL End();

	BOOL DataPlay(SLZData Data,BOOL bWait=FALSE);
	//BOOL DataPlay(const CString strPlay);
	static UINT PlayVoiceThread(LPVOID pParam);
private:
	BOOL AddPlayText(VOICEDISPLAYSTR& vdstr);
	CList<VOICEDISPLAYSTR> m_PlayDataList;    ////////中转结构体链表
	CStringList m_WavList;
	CString m_strCallMsg;
	CString m_strShowMsg;
	CString m_strWndName;
	int m_iPlayTimes;
	int m_iScreenId;
	int m_iScreenId2;

	BOOL m_bPlayVoiceThread;
	CMutex m_mtPlayStrList;
	CString m_strCallPath;
	CWinThread* m_pPlayVoiceThread;
	CString ChangeCallStr(const SLZData& data);
	CString ChangeShowStr(const SLZData& data);
	CString ChangeWaitCallStr(const SLZData& data);
	CString ChangeWaitShowStr(const SLZData& data);
	void InsertSplit(CString& strinsert,CString strsplt);//添加分隔符
	///////转换英语//////
	CString InsertSplitEng(CString strinsert);
	CString InsertJttsEng(CString strinsert);
	CString m_strWavLibPath;
	CString ReplaceQueNum(CString strQueNum);

	int SplitNumStrToNumbers(const CString& strNumSrc, CStringArray& strarrDest);			 //////转换数字格式到数组中
	int SplitCusNumToStrArr(const CString& strSrc, CStringArray& strarrDest);
	int SplitStrToCharArray(const CString& strSrc, CStringArray& strarrDest);
	int SplitStrToNumArray(CString strSrc,CStringArray&strarrDest);					/////分割汉字与非汉字
	CString InsertSplitToArray(CStringArray&strarrOrigin,CString strSplit);			///// 向数组中插入字符
	CString RemoveChinese(CString strOrigin);			////////删除字符串汇中的汉字
	BOOL StrHasNum(CString strOrigin);						//// 字符串中是否有数字
	CString Getthesame(CString strSrc);						//////提取字符串中和第一个字符类型相同的字符串（汉字或非汉字）
	BOOL PlayTheVoice(CString strVoice,const CStringList& WavList);			
	CString PlayTheVoiceFromRight(CString strVoice,const CStringList& WavList);			////////从字符串的右侧循环发声
};
