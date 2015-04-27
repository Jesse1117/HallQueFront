// QueueClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <string>
#include <list>
using namespace std;

// CQueueClientApp:
// 有关此类的实现，请参阅 QueueClient.cpp
//
#define WM_WAITNUMMSG WM_USER + 101
#define WM_SHOWMSG WM_USER + 102
#define WM_SHOWCALLMSG WM_USER + 103
#define WM_SHOWDONEMSG WM_USER + 104


class CQueueClientApp : public CWinApp
{
public:
	CQueueClientApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
	list<CString> m_list_caCheMsg;

};

extern CQueueClientApp theApp;