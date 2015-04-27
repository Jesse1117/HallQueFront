#include "StdAfx.h"
#include "DealPacket.h"
#include "SLZData.h"

CDealPacket::CDealPacket(void)
{
}

CDealPacket::~CDealPacket(void)
{
}

string CDealPacket::ProduceSendPacket(const SLZData* pData)
{
	CString wPacket = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>hallQue</headCode>");
	wPacket.AppendFormat(_T("<SerialId>%s</SerialId>"),pData->GetSerialId());
	wPacket.AppendFormat(_T("<BussName>%s</BussName>"),pData->GetBussName());
	wPacket.AppendFormat(_T("<QueNum>%s</QueNum>"),pData->GetQueueNumber());
	wPacket.AppendFormat(_T("<CardNum>%s</CardNum>"),pData->GetCardNumber());
	wPacket.AppendFormat(_T("<CustName>%s</CustName>"),pData->GetCustName());
	wPacket.AppendFormat(_T("<WaitTime>%d</WaitTime>"),pData->GetWaitTime());
	wPacket.AppendFormat(_T("<CustPhoneNum>%s</CustPhoneNum>"),pData->GetPhoneNum());
	wPacket.AppendFormat(_T("<ShortMsg>%s</ShortMsg>"),pData->GetSendMsg());
	wPacket.Append(_T("</dataPacket>"));

	CStringA aPacket ( wPacket.GetBuffer(0) );
	wPacket.ReleaseBuffer(0);

	string strPacket = aPacket.GetBuffer(0);
	aPacket.ReleaseBuffer(0);
	return strPacket;
}

BOOL CDealPacket::AnaSendPacket(const string& recvPacket,SLZData* pData)
{
	BOOL flag = FALSE;
	string aSerialId;
	flag = GetDataTypeValue(enumDataTypeSerialId,recvPacket,aSerialId);
	CString wSerialId(aSerialId.c_str());
	pData->SetSerialId(wSerialId);

	string aBussName;
	flag = GetDataTypeValue(enumDataTypeBussName,recvPacket,aBussName);
	CString wBussName(aBussName.c_str());
	pData->SetBussName(wBussName);

	string aQueNum;
	flag = GetDataTypeValue(enumDataTypeQueNum,recvPacket,aQueNum);
	CString wQueNum(aQueNum.c_str());
	pData->SetQueueNumber(wQueNum);

	string aCardNum;
	flag = GetDataTypeValue(enumDataTypeCardNum,recvPacket,aCardNum);
	CString wCardNum(aCardNum.c_str());
	pData->SetCardNumber(wCardNum);

	string aCustName;
	flag = GetDataTypeValue(enumDataTypeCustName,recvPacket,aCustName);
	CString wCustName(aCustName.c_str());
	pData->SetCustName(wCustName);

	string aWaitTime;
	flag = GetDataTypeValue(enumDataTypeWaitTime,recvPacket,aWaitTime);
	pData->SetWaitTime(atoi(const_cast<char*>(aWaitTime.c_str())));

	string aCustPhoneNum;
	flag = GetDataTypeValue(enumDataTypeCustPhoneNum,recvPacket,aCustPhoneNum);
	CString wCustPhoneNum(aCustPhoneNum.c_str());
	pData->SetPhoneNum(wCustPhoneNum);

	string aShortMsg;
	flag = GetDataTypeValue(enumDataTypeShortMsg,recvPacket,aShortMsg);
	CString wShortMsg(aShortMsg.c_str());
	pData->SetSendMsg(wShortMsg);

	return flag;
}

BOOL CDealPacket::GetDataTypeValue(DataType type,const string& recvPacket,string& datavalue)
{
	string sample1,sample2;
	switch(type)
	{
	case enumDataTypeSerialId:
		sample1 = "<SerialId>";
		sample2 = "</SerialId>";
		break;
	case enumDataTypeBussName:
		sample1 = "<BussName>";
		sample2 = "</BussName>";
		break;
	case enumDataTypeQueNum:
		sample1 = "<QueNum>";
		sample2 = "</QueNum>";
		break;
	case enumDataTypeCardNum:
		sample1 = "<CardNum>";
		sample2 = "</CardNum>";
		break;
	case enumDataTypeCustName:
		sample1 = "<CustName>";
		sample2 = "</CustName>";
		break;
	case enumDataTypeWaitTime:
		sample1 = "<WaitTime>";
		sample2 = "</WaitTime>";
		break;
	case enumDataTypeCustPhoneNum:
		sample1 = "<CustPhoneNum>";
		sample2 = "</CustPhoneNum>";
		break;
	case enumDataTypeShortMsg:
		sample1 = "<ShortMsg>";
		sample2 = "</ShortMsg>";
		break;
	default:
		return FALSE;
	}
	string::size_type pos1,pos2;
	pos1 = recvPacket.find(sample1.c_str());
	pos2 = recvPacket.find(sample2.c_str());
	if(pos1 == recvPacket.npos || pos2 == recvPacket.npos)
	{
		return FALSE;
	}

	datavalue = recvPacket.substr(pos1 + sample1.size(),pos2 - pos1 - sample1.size());
	return TRUE;
}


string CDealPacket::ProduceRetPacket(BOOL isSuccse)
{
	CString wPacket = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>hallQue</headCode>");
	wPacket.AppendFormat(_T("<retCode>%d</retCode>"),isSuccse);
	wPacket.Append(_T("</dataPacket>"));

	CStringA aPacket( wPacket.GetBuffer(0) );
	wPacket.ReleaseBuffer(0);

	string strPacket = aPacket.GetBuffer(0);
	aPacket.ReleaseBuffer(0);
	return strPacket;
}

BOOL CDealPacket::AnaRetPacket(const string& recvPacket)
{
	string::size_type pos1,pos2;
	pos1 = recvPacket.find("<retCode>");
	pos2 = recvPacket.find("</retCode>");
	if(pos1 == recvPacket.npos || pos2 == recvPacket.npos)
		return FALSE;

	string strRetCode = recvPacket.substr(pos1 + strlen("<retCode>"),pos2 - pos1 - strlen("<retCode>"));
	return BOOL(atoi(const_cast<char*>(strRetCode.c_str())));
}