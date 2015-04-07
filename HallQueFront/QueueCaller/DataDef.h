#ifndef _QUEUECALLER_DATADEF_H
#define _QUEUECALLER_DATADEF_H

struct UserInfo{
	CString UserName;
	CString PassWord;
	BOOL isAutoLogin;
	BOOL isRemember;
};

struct ComInfo{
	CString strIP;
	CString strPort;
	
};

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