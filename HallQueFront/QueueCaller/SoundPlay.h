#pragma once
#include "SLZData.h"
#include <afxwin.h>
#include <afxmt.h>
typedef struct _tagVoiceAndDisplayStr
{
	CString		strAd;		//���
	CString		strVoiceStr;	//��������
	CString		strDisplayStr;	//��ʾ����
	int			iWndScreenId;		//��������ַ
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
	CList<VOICEDISPLAYSTR> m_PlayDataList;    ////////��ת�ṹ������
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
	void InsertSplit(CString& strinsert,CString strsplt);//��ӷָ���
	///////ת��Ӣ��//////
	CString InsertSplitEng(CString strinsert);
	CString InsertJttsEng(CString strinsert);
	CString m_strWavLibPath;
	CString ReplaceQueNum(CString strQueNum);

	int SplitNumStrToNumbers(const CString& strNumSrc, CStringArray& strarrDest);			 //////ת�����ָ�ʽ��������
	int SplitCusNumToStrArr(const CString& strSrc, CStringArray& strarrDest);
	int SplitStrToCharArray(const CString& strSrc, CStringArray& strarrDest);
	int SplitStrToNumArray(CString strSrc,CStringArray&strarrDest);					/////�ָ����Ǻ���
	CString InsertSplitToArray(CStringArray&strarrOrigin,CString strSplit);			///// �������в����ַ�
	CString RemoveChinese(CString strOrigin);			////////ɾ���ַ������еĺ���
	BOOL StrHasNum(CString strOrigin);						//// �ַ������Ƿ�������
	CString Getthesame(CString strSrc);						//////��ȡ�ַ����к͵�һ���ַ�������ͬ���ַ��������ֻ�Ǻ��֣�
	BOOL PlayTheVoice(CString strVoice,const CStringList& WavList);			
	CString PlayTheVoiceFromRight(CString strVoice,const CStringList& WavList);			////////���ַ������Ҳ�ѭ������
};
