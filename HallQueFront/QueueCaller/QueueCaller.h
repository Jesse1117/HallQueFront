// QueueCaller.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


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
};

extern CQueueCallerApp theApp;