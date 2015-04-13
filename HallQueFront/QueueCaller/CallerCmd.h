#pragma once

#include "DataDef.h"

class CallerCmd
{
public:
	CallerCmd(void): m_iWndId(0)
		, m_cmdType(cmdNotDefined)
		, m_strCarriedData(_T(""))
		, m_bSuccess(FALSE){}
	~CallerCmd(void){}

	UINT GetWindowId() const{ return m_iWndId;}//����ID
	void SetWindowId(const UINT iWndId){ m_iWndId = iWndId;}
	CmdType GetCmdType() const{ return m_cmdType;}//��������
	void SetCmdType(CmdType cmdType){ m_cmdType = cmdType;}
	CString GetCarriedData() const { return m_strCarriedData;}//��������
	void SetCarriedData(const CString& strData) { m_strCarriedData = strData;}
	void SetSuccess(const BOOL flag){m_bSuccess = flag;}//�ɹ���־
	BOOL GetSuccess(){return m_bSuccess;}
private:
	UINT m_iWndId;
	CmdType m_cmdType;
	CString m_strCarriedData;
	BOOL m_bSuccess;
};
