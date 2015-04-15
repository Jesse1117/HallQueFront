#ifndef _QUEUECALLER_DATADEF_H
#define _QUEUECALLER_DATADEF_H

#define CALL_MSG WM_USER+1117

#define MYBUFLEN 256
#define DATABUFLEN 1024

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

typedef enum _tagEvaLevel
{
	evaNone			= -1,		//δ����
	evaBad				= 0,		//������
	evaNormal		= 1,		//һ��
	evaGood			= 2,		//����
	evaPefect			= 3		//�ǳ�����
} EvaLevel;

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


///д��������
typedef struct _tagWriteComMsg
{
public:
	_tagWriteComMsg(){
		memset(buf,0,DATABUFLEN);
		length = 0;
	}
	int length;
	char buf[DATABUFLEN];
}WriteComMsg;

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