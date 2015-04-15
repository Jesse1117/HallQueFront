#ifndef _QUEUECALLER_DATADEF_H
#define _QUEUECALLER_DATADEF_H

#define CALL_MSG WM_USER+1117

#define MYBUFLEN 256
#define DATABUFLEN 1024

typedef enum _tagCmdType
{
	cmdNotDefined		= 0,		//未定义
	cmdLogin			= 1,		//登录
	cmdQuit				= 2,		//退出
	cmdCall				= 3,		//呼叫
	cmdRecall			= 4,		//重呼
	cmdDiscard			= 5,		//过号
	cmdWait				= 6,		//呼叫下一位准备
	cmdEvaReq			= 7,		//评价请求
	cmdPause			= 8,		//暂停
	cmdResume			= 9,		//恢复
	cmdCallNum			= 10,		//呼叫特定号码
	cmdCallSec          = 11,		//呼叫保安
	cmdCallMana			= 12,		//呼叫大堂经理
	cmdCallBusc			= 13,		//呼叫业务顾问
	cmdExChange			= 14,		//转移队列
	//呼叫器显示以及其他命令
	callerCmdShowAdd	= 15,		//显示地址
	callerCmdShowSuc	= 16,		//显示成功(即处理成功)
	callerCmdShowFail	= 17,		//显示失败
	callerCmdShowNum	= 18,		//显示剩余人数
	callerCmdShowAlarm	= 19		//发出滴滴声
} CmdType;

struct UserInfo{
	CString UserName;
	CString PassWord;
	BOOL isAutoLogin;
	BOOL isRemember;
};

typedef enum _tagEvaLevel
{
	evaNone			= -1,		//未评价
	evaBad				= 0,		//不满意
	evaNormal		= 1,		//一般
	evaGood			= 2,		//满意
	evaPefect			= 3		//非常满意
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


///写串口数据
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