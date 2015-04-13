#pragma once
#include "SLZData.h"
class CDealData
{
private:
	CDealData(void);
public:
	static CDealData* GetInstance()
	{
		CDealData* data = new CDealData();
		return data;
	}
	~CDealData(void);
	void AddData(SLZData& data);  // �����ŶӶ���
	BOOL HasData();   //�ж���ɶ����Ƿ�������
private:
	CList<SLZData,SLZData&> m_DataList;   //�ŶӶ���
	CList<SLZData,SLZData&> m_DoneList;   //��ɶ���
	HANDLE m_hCallerHandle;
	static DWORD WINAPI CallThread(LPVOID lpParam);
	
};
