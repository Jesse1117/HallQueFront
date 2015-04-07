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

#define COLORCODE_RED				(_T("#0#"))			//红色
#define COLORCODE_GREEN				(_T("#1#"))			//绿色
#define COLORCODE_YELLOW			(_T("#2#"))			//黄色

#define COLORREF_RED				0x0000FF			//红色
#define COLORREF_GREEN				0x00FF00			//绿色
#define COLORREF_YELLOW				0x00FFFF			//黄色

#define STRING_QUEUEID_CALL			(_T("[排队号码]"))			//呼叫排队号码
#define STRING_CUSTNAME_CALL		(_T("[客户姓名]"))			//呼叫客户姓名
#define STRING_WINDOWNAME			(_T("[窗口名称]"))			//窗口名称
#endif