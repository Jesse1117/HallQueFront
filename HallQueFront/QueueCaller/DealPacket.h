#pragma once
#include <string>
using namespace std;

class SLZData;

typedef enum{
	enumDataTypeSerialId,
	enumDataTypeBussName,
	enumDataTypeQueNum,
	enumDataTypeCardNum,
	enumDataTypeCustName,
	enumDataTypeWaitTime,
	enumDataTypeCustPhoneNum,
	enumDataTypeShortMsg
}DataType;

class CDealPacket
{
public:
	CDealPacket(void);
	~CDealPacket(void);

public:
	static string ProduceSendPacket(const SLZData* pData);
	static BOOL  AnaSendPacket(const string& recvPacket,SLZData* pData);
	static string ProduceRetPacket(BOOL isSuccse);
	static BOOL AnaRetPacket(const string& recvPacket);
	static BOOL GetDataTypeValue(DataType type,const string& recvPacket,string& datavalue);
};
