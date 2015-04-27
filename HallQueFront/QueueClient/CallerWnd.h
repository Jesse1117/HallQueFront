#pragma once

#define LENGTH 256

class CCallerWnd : public CObject
{
public:
	CCallerWnd(void);
	~CCallerWnd(void);

	
	CCallerWnd(const CCallerWnd& obj);
	CCallerWnd& operator=(const CCallerWnd& obj);


	UINT GetSerialID() const {return m_nSerialID;}
	
	int GetCallerID() const {return m_nCallerID;}
	void SetCallerID(int nCallerID){m_nCallerID = nCallerID;}

	int GetWndID()const {return m_nWndID;}
	void SetWndID(int nWndID){m_nWndID = nWndID;}

	CString GetWndName()const {return m_strWndName;}
	void SetWndName(const CString& wndName){m_strWndName = wndName;}
	

	virtual void Serialize( CArchive& ar );//支持序列化
	DECLARE_SERIAL(CCallerWnd)
private:
	CString m_strWndName;
	UINT m_nSerialID;
	int m_nCallerID;
	int m_nWndID;
public:
	WCHAR* m_strCanDoBuss[3];
};
