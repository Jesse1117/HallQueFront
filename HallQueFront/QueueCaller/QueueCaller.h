// QueueCaller.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include <list>
using namespace std;
// CQueueCallerApp:
// �йش����ʵ�֣������ QueueCaller.cpp
//

class CQueueCallerApp : public CWinApp
{
public:
	CQueueCallerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	list<CString> m_list_caCheMsg;//���ݻ���cache
};

extern CQueueCallerApp theApp;