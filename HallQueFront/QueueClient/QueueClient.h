// QueueClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <string>
#include <list>
using namespace std;

// CQueueClientApp:
// �йش����ʵ�֣������ QueueClient.cpp
//
#define WM_WAITNUMMSG WM_USER + 101
#define WM_SHOWMSG WM_USER + 102
#define WM_SHOWCALLMSG WM_USER + 103
#define WM_SHOWDONEMSG WM_USER + 104


class CQueueClientApp : public CWinApp
{
public:
	CQueueClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	list<CString> m_list_caCheMsg;

};

extern CQueueClientApp theApp;