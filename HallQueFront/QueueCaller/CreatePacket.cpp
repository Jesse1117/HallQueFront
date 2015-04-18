#include "StdAfx.h"
#include "CreatePacket.h"

CCreatePacket::CCreatePacket(void)
{
}

CCreatePacket::~CCreatePacket(void)
{
}

CString CCreatePacket::ProducePacket(const SLZData& data)
{
	CString packet = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><dataPacket version=\"1.0\"><headCode>hallQue</headCode>");
	packet.AppendFormat(_T("<SerialId>%s</SerialId>"),data.GetSerialId());
	packet.AppendFormat(_T("<BussName>%s</BussName>"),data.GetBussName());
	packet.AppendFormat(_T("<QueNum>%s</QueNum>"),data.GetQueueNumber());
	packet.AppendFormat(_T("<CardNum>%s</CardNum>"),data.GetCardNumber());
	packet.AppendFormat(_T("<CustName>%s</CustName>"),data.GetCustName());
	packet.AppendFormat(_T("<WaitTime>%d</WaitTime>"),data.GetWaitTime());
	packet.AppendFormat(_T("<CustPhoneNum>%s</CustPhoneNum>"),data.GetPhoneNum());
	packet.AppendFormat(_T("<ShortMsg>%s</ShortMsg>"),data.GetSendMsg());
	return packet;
}

CString CCreatePacket::ExChangeTime(const CTime& time)
{
	CString wStrTime;
	wStrTime.Format(_T("%d-%d-%d %d:%d:%d"),time.GetYear(),time.GetMonth(),
		time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	return wStrTime;
}