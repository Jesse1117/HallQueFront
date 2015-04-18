#pragma once
#include "SLZStaff.h"
#include "CommonConvert.h"


class SLZWindow:public CObject
{
public:
	SLZWindow();
	~SLZWindow();
	SLZWindow(const SLZWindow& obj);
	SLZWindow& operator=(const SLZWindow& obj);	

	//��ȡ���ڱ��
	UINT GetWindowId() const
	{
		return m_iWindowId;
	}
	//���ô��ڱ��
	void SetWindowId(UINT iWindowId)
	{
		m_iWindowId = iWindowId;
	}
	//��ȡ��ʾ���
	UINT GetShowWndId() const
	{
		return m_iShowWndId;
	}
	//������ʾ���
	void SetShowWndId(UINT iShowWndId)
	{
		m_iShowWndId = iShowWndId;
	}
	//��ȡ��������
	CString GetWindowName() const
	{
		return m_strWindowName;
	}
	//���ô�������
	void SetWindowName(const CString& strWindowName)
	{
		m_strWindowName = strWindowName;
	}
	//��ȡ��������
	CString GetWindowCallName() const
	{
		return m_strWindowCallName;
	}
	//���ô�������
	void SetWindowCallName(const CString& strWindowCallName)
	{
		m_strWindowCallName = strWindowCallName;
	}
	//��ȡ��������ַ
	int GetCallerId() const
	{
		return m_iCallerId;
	}
	//���ú�������ַ
	void SetCallerId(int iCallerId)
	{
		m_iCallerId = iCallerId;
	}
	//��ȡ��������ַ
	int GetEvaluatorId() const
	{
		return m_iEvaluatorId;
	}
	//������������ַ
	void SetEvaluatorId(int iEvaluatorId)
	{
		m_iEvaluatorId = iEvaluatorId;
	}
	//��ȡ��������ַ
	void  GetWndScreenId(CArray<int>& wndscradd)const
	{
		wndscradd.Copy(m_iWndScreenId);
	}
	//���ô�������ַ
	void SetWndScreenId(CArray<int>& wndscradd)
	{
		m_iWndScreenId.Copy(wndscradd);
	}
	//��ȡ�ۺ�����ַ
	void GetComScreenId(CArray<int>& wndcomadd) const
	{
		wndcomadd.Copy(m_iComScreenId);
	}
	//�����ۺ�����ַ
	void SetComScreenId(CArray<int>& wndcomadd) 
	{
		m_iComScreenId.Copy(wndcomadd);
	}

	//��ȡͨ��IP��ַ
	void  GetLEDIPId(CStringArray& IpAdd)
	{
		IpAdd.Copy(m_strLEDIPId);
	}

	void SetLEDIPId(CStringArray&  IpAdd)
	{
		m_strLEDIPId.Copy(IpAdd);
	}
	
	//��ȡͨ�������ַ
	void GetLEDPhyId(CArray<int>& PhyId)
	{
		PhyId.Copy(m_iLEDPhyId);
	}

	void SetLEDPhyId(CArray<int>& PhyId)
	{
		m_iLEDPhyId.Copy(PhyId);
	}

	int GetLEDPipeId()
	{
		return m_iLEDPipeId;
	}

	void SetLEDPipeId(const int LEDPipeId)
	{
		m_iLEDPipeId = LEDPipeId;
	}

	CString GetStbId()const 
	{
		return m_strStbId;
	}
	//������
	void SetStbId(const CString stbid)
	{
		m_strStbId=stbid;
	}

	////��ȡ��¼Ա��
	//CString GetCurLoginStaff() 
	//{
	//	return m_staffLoginId;
	//}
	////���õ�¼Ա��
	//void SetCurLoginStaff(CString& staff)
	//{
	//	m_staffLoginId=staff;
	//}

	//��ȡԤ��Ա��
	CString GetLoginStaff() 
	{
		return m_staffDefaultId;
	}

	//����Ԥ��Ա��

	void SetLoginStaff(const CString& staff)
	{
		m_staffDefaultId = staff;
	}

	//CString GetCurStaffName()
	//{
	//	return m_StrCurStaffName;
	//}
	//void SetCurStaffName(const CString& CurStaffName )
	//{
	//	m_StrCurStaffName=CurStaffName;
	//}
	//CString GetDefStaffName()
	//{
	//	return m_StrDefStaffName;
	//}
	//void SetDefStaffName(const CString& DefStaffName)
	//{
	//	m_StrDefStaffName=DefStaffName;
	//}
	void GetArrayQueId(CStringArray& arrQueId)
	{
		arrQueId.Copy(m_arrBussId);
	}

	void SetArrayQueId( CStringArray& arrQueId)
	{
		m_arrBussId.Copy(arrQueId);
	}
	int GetArraySize()const
	{
		return m_ArraySize;
	}
	void SetArraySize(const int size)
	{
		m_ArraySize=size;
	}
	//��ȡ������Ϣ
	CString GetCallMsg()
	{
		return m_CalledMsg;
	}
	void SetCallMsg(const CString& CallMsg)
	{
		m_CalledMsg=CallMsg;
	}
	//��ȡ�ȴ���Ϣ
	CString GetWaitCallMsg()
	{
		return m_WaitCalledMsg;
	}
	void SetWaitCallMsg(const CString& CallMsg)
	{
		m_WaitCalledMsg=CallMsg;
	}
	//��ȡ��ʾ��Ϣ
	CString GetShowMsg()
	{
		return m_ShowMsg;
	}
	void SetShowMsg(const CString& ShowMsg)
	{
		m_ShowMsg=ShowMsg;
	}
	//��ȡ�ȴ���ʾ��Ϣ
	CString GetWaitShowMsg()
	{
		return m_WaitShowMsg;
	}
	void SetWaitShowMsg(const CString& ShowMsg)
	{
		m_WaitShowMsg=ShowMsg;
	}
	
	UINT GetMsgShowTime()
	{
		return m_iMsgShowTime;
	}

	void SetMsgShowTime(const UINT& MsgShowTime)
	{
		m_iMsgShowTime=MsgShowTime;
	}

	CString GetAdMsg()
	{
		return m_strAdMsg;
	}

	void SetAdMsg(const CString& AdMsg)
	{
		m_strAdMsg=AdMsg;
	}

	//BOOL GetFlagMustEval()
	//{
	//	return m_FlagMustEval;
	//}
	//void SetFlagMustEval(const BOOL& FlagMustEval)
	//{
	//	m_FlagMustEval=FlagMustEval;
	//}
	UINT GetEvaTimeOut() const
	{
		return m_iEvaTimeOut;
	}
	void SetEvaTimeOut(UINT iEvaTimeOut)
	{
		m_iEvaTimeOut = iEvaTimeOut;
	}

	BOOL GetUnOrder()const	{return m_bUnOrder;}

	void SetUnOrder(BOOL bUnOrder){m_bUnOrder = bUnOrder;}

	virtual void Serialize( CArchive& ar );
	DECLARE_SERIAL(SLZWindow)

private:
	UINT m_iWindowId;				//����Ψһ���
	UINT m_iShowWndId;			//������ʾ���
	CString m_strWindowName;		//��������
	CString m_strWindowCallName;	//��������
	int m_iCallerId;				//��������ַ
	int m_iEvaluatorId;				//��������ַ
	CArray<int> m_iWndScreenId;				//��������ַ
	CArray<int> m_iComScreenId;				//�ۺ�����ַ
	int m_wndScrSize;			
	int m_comScrSize;
	CStringArray m_strLEDIPId;			//ͨ��IP��ַ
	CArray<int> m_iLEDPhyId;				//ͨ�������ַ
	int m_IPSize;
	int m_PhySize;
	int m_iLEDPipeId;				//ͨ��ͨ����
	CString m_strStbId;				//�����б��
	//CString m_staffLoginId;			//��ǰ��¼Ա��
	CString m_staffDefaultId;		//�Զ���¼ʱԤ��Ա��
	//CString m_StrCurStaffName;
	//CString m_StrDefStaffName;
	//CString m_strAbleQueId;			//�ɴ������
	int m_ArraySize;				//���������С
	CStringArray m_arrBussId;		//�ɴ���ҵ���������
	//CString	m_AbleQueName;			//�ɴ���ҵ������
	CString m_CalledMsg;			//������Ϣ
	CString m_WaitCalledMsg;		//�ȴ�������Ϣ
	CString m_ShowMsg;				//��ʾ��Ϣ
	CString m_WaitShowMsg;			//�ȴ���ʾ��Ϣ
	unsigned int m_iMsgShowTime;		//��Ϣ��ʾʱ��
	CString m_strAdMsg;				//�����Ϣ
	//BOOL m_FlagMustEval;			//�Ƿ�ǿ������
	UINT m_iEvaTimeOut;			//���۳�ʱ����
	BOOL m_bUnOrder;			//�Ƿ�ȥ�����ȼ�
};

