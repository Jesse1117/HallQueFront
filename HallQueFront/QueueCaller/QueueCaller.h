// QueueCaller.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include <list>
#include <map>
using namespace std;
// CQueueCallerApp:
// 有关此类的实现，请参阅 QueueCaller.cpp
//
typedef struct
{
	int nCallerid;
	int nWndid;
}SecWndInfo;

class CQueueCallerApp : public CWinApp
{
public:
	CQueueCallerApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
public:
	

	list<CString> m_list_caCheMsg;//数据缓存cache
	map<CString,SecWndInfo> m_map_secwndinfo;
};

extern CQueueCallerApp theApp;