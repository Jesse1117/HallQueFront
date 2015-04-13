#pragma once

class CUserInfo:public CObject
{
public:
	CUserInfo(void);
	~CUserInfo(void);

	CUserInfo(const CUserInfo& obj);
	CUserInfo& operator=(CUserInfo& obj);

	CString GetUserName()const{return m_UserName;}
	void SetUserName(const CString& username){m_UserName = username;}

	CString GetPassWord()const{return m_PassWord;}
	void SetPassWord(const CString& Password){m_PassWord= Password;}

	BOOL isLogin()const{return m_isAutoLogin;}
	void SetLogin(const BOOL& bAutologin){m_isAutoLogin = bAutologin;}

	BOOL IsRemember()const{return m_isRemember;}
	void SetRemember(const BOOL& bRemember){m_isRemember = bRemember;}

	virtual void Serialize( CArchive& ar );//支持序列化
	DECLARE_SERIAL(CUserInfo)
private:
	CString m_UserName;
	CString m_PassWord;
	BOOL m_isAutoLogin;
	BOOL m_isRemember;
};
