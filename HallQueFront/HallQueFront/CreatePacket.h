#pragma once
#include <string>
#include "SLZData.h"
using namespace std;
class CCreatePacket
{
public:
	CCreatePacket(void);
	~CCreatePacket(void);
	CString ProducePacket(const SLZData& data);
private:
	CString ExChangeTime(const CTime& time);
};
