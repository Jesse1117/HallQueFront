#ifndef _QUEUECALLER_DATADEF_H
#define _QUEUECALLER_DATADEF_H

typedef enum _tagCmdType
{
	cmdNotDefined		= 0,		//δ����
	cmdLogin			= 1,		//��¼
	cmdQuit				= 2,		//�˳�
	cmdCall				= 3,		//����
	cmdRecall			= 4,		//�غ�
	cmdDiscard			= 5,		//����
	cmdWait				= 6,		//������һλ׼��
	cmdEvaReq			= 7,		//��������
	cmdPause			= 8,		//��ͣ
	cmdResume			= 9,		//�ָ�
	cmdCallNum			= 10,		//�����ض�����
	cmdCallSec          = 11,		//���б���
	cmdCallMana			= 12,		//���д��þ���
	cmdCallBusc			= 13,		//����ҵ�����
	cmdExChange			= 14,		//ת�ƶ���
	//��������ʾ�Լ���������
	callerCmdShowAdd	= 15,		//��ʾ��ַ
	callerCmdShowSuc	= 16,		//��ʾ�ɹ�(������ɹ�)
	callerCmdShowFail	= 17,		//��ʾʧ��
	callerCmdShowNum	= 18,		//��ʾʣ������
	callerCmdShowAlarm	= 19		//�����ε���
} CmdType;

struct UserInfo{
	CString UserName;
	CString PassWord;
	BOOL isAutoLogin;
	BOOL isRemember;
};

typedef struct _tagComInfo{
	CString strIP;
	CString strPort;	
	UINT iWndId;
	CString strCallName;
	UINT iWatiTime;
	UINT iPlayTimes;
}ComInfo,*pComInfo;

typedef struct _tagShowInfomation
{
	CString strAdMsg;
	CString strShowMsg;
	UINT iShowTime;
}ShowInfo,*pShowInfo;

#define COLORCODE_RED				(_T("#0#"))			//��ɫ
#define COLORCODE_GREEN				(_T("#1#"))			//��ɫ
#define COLORCODE_YELLOW			(_T("#2#"))			//��ɫ

#define COLORREF_RED				0x0000FF			//��ɫ
#define COLORREF_GREEN				0x00FF00			//��ɫ
#define COLORREF_YELLOW				0x00FFFF			//��ɫ

#define STRING_QUEUEID_CALL			(_T("[�ŶӺ���]"))			//�����ŶӺ���
#define STRING_CUSTNAME_CALL		(_T("[�ͻ�����]"))			//���пͻ�����
#define STRING_WINDOWNAME			(_T("[��������]"))			//��������
#endif