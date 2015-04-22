#pragma once
#include <afxmt.h>
#include <list>
#include "SLZData.h"
#include <string>
#include "JudyeShortMsg.h"
using namespace std;

class CFinshQueData
{
public:
	CFinshQueData(void);
	~CFinshQueData(void);
	void Add(const SLZData& data);//��������
	BOOL GetFinshedData(const UINT winID,SLZData& data);//��ȡ����,��ɾ��
	BOOL GetFinshedData();
private:
	std::list<SLZData> m_listFinshQue;//��ɶ���
	std::list<CJudgeShortMsg*> m_list_shortmsg;//����������Ϣ����
	CMutex m_mtFinshQue;//��ɶ�����
	CString m_filePath;//���ļ�·��
private:
	static UINT DoFinshedData(LPVOID pParam);
	CWinThread* m_pDoFinshedDataThread;//������������߳�
	BOOL Start();
	void ReleaseListShorMsg();
	BOOL ReadJudgeShortMsg();
	BOOL SendMsgToPhone(const SLZData& data);

	void WriteErrLog(CString strErr);
	void WriteLogWithTime(CString strSockLog);			//д�������־
	BOOL AppendWriteFile(CString strText, CString strFileName);
	CString m_strLogFilePath;
public:
	BOOL ReFlushListShortMsg();//�ӿ�ˢ�³��ֲ������Ͷ��ŵĶ���
};
