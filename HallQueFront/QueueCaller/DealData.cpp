#include "StdAfx.h"
#include "DealData.h"

CDealData::CDealData(void)
{
}

CDealData::~CDealData(void)
{
}

BOOL CDealData::HasData()
{
	if (m_DoneList.GetCount()>0)
	{
		return TRUE;
	}
	else return FALSE;
}

void CDealData::AddData(SLZData& data)
{
	m_DataList.AddTail(data);
}

DWORD WINAPI CallThread(LPVOID lpParam)
{
	while (TRUE)
	{
			
	}
	return 0;
}


