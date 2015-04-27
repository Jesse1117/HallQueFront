/************************************************************************/
/*     Author:	Songyz                                                  */
/*     Time:	2013.11.14                                              */
/************************************************************************/
#include "StdAfx.h"
#include "CommonStrMethod.h"
#include <shlwapi.h>

CommonStrMethod::CommonStrMethod(void)
{
}

CommonStrMethod::~CommonStrMethod(void)
{
}
/**
desc:		���ֽ��ַ��� ת ���ֽ��ַ���

dstBuf:		Ŀ���ַ���������
src:		Դ�ַ���

return:		�ɹ������� TRUE;���򣬷��� FALSE
*/
BOOL CommonStrMethod::WChar2Char(char* dstBuf, size_t len, const wchar_t* src)
{
	if(src == NULL) return FALSE;

	size_t chlen = wcslen(src);
	size_t nBytes =(size_t)WideCharToMultiByte(0, 0,src, chlen, 
		NULL, 0, NULL, NULL);
	int iLastIndex;
	if(nBytes>=len)
	{
		nBytes=len-1;
		iLastIndex = len;
	}
	else
	{
		iLastIndex = nBytes;
	}
	nBytes = WideCharToMultiByte(0, 0, src, chlen, 
		dstBuf, nBytes, NULL, NULL);
	dstBuf[iLastIndex]=0;
	if(nBytes == 0) return FALSE;
	return TRUE;
}

/**
desc:		���ֽ��ַ��� ת ���ֽ��ַ���

dstBuf:		Ŀ���ַ���������
src:		Դ�ַ���

return:		�ɹ������� TRUE;���򣬷��� FALSE
*/
BOOL CommonStrMethod::Char2WChar(wchar_t *dstBuf, size_t len, const char *src)
{
	if(src == NULL) return FALSE;

	size_t chlen = strlen(src);
	size_t nBytes =(size_t)MultiByteToWideChar(CP_ACP, 0,
		(const char *)src,(int)chlen,NULL,0);
	int iLastIndex;
	if(nBytes>=len)
	{
		nBytes=len-1;
		iLastIndex = len;
	}
	else
	{
		iLastIndex = nBytes;
	}
	nBytes = MultiByteToWideChar(CP_ACP, 0, 
		(const char *)src,(int)chlen,dstBuf,(int)nBytes);
	dstBuf[iLastIndex]=0;
	if(nBytes == 0) return FALSE;
	return TRUE;
}

/**
desc:		���ַ����в������ַ�����һ�γ��ֵ��±�
strSrc:		Դ�ַ���
strFind:	Ҫ���ҵ��ַ���

return:		���ҳɹ��������������ַ�����һ�γ��ֵ��±ꣻ
���򣬷��� -1
*/
int CommonStrMethod::StrFind(CString strSrc, 
							 CString strFind)
{
	int nLength = strFind.GetLength();
	int nAbsStart = 0;
	int nStart = -1;
	for(; (nStart = strSrc.Find(strFind.GetAt(0))) != -1;)
	{
		int i = 0, j = nStart;
		BOOL bRes = TRUE;
		for(; i < nLength-1;)
		{
			if(strSrc.GetAt(++j) != strFind.GetAt(++i))
			{
				strSrc = strSrc.Mid(nStart+1);
				nAbsStart += nStart+1;
				nStart = -1;
				bRes = FALSE;
				break;
			}
		}
		if(i == nLength-1 && bRes)
		{
			nAbsStart += nStart;
			return nAbsStart;
		}
	}
	return -1;
}

/**
desc:		���ַ����в������ַ�����һ�γ��ֵ��±�, �����ִ�Сд
strSrc:		Դ�ַ���
strFind:	Ҫ���ҵ��ַ���

return:		���ҳɹ��������������ַ�����һ�γ��ֵ��±ꣻ
���򣬷��� -1
*/
int CommonStrMethod::StrFindNoCase(CString strSrc, 
								   CString strFind)
{
	int nLength = strFind.GetLength();
	int nAbsStart = 0;
	int nStart = -1;
	for(; (nStart = FindWcharNoCase(strSrc, strFind.GetAt(0))) != -1;)
	{
		int i = 0, j = nStart;
		BOOL bRes = TRUE;
		for(; i < nLength - 1;)
		{
			if(!CompareWcharNoCase(strSrc.GetAt(++j),strFind.GetAt(++i)))
			{
				strSrc = strSrc.Mid(nStart+1);
				nAbsStart += nStart+1;
				nStart = -1;
				bRes = FALSE;
				break;
			}
		}
		if(i == nLength - 1 && bRes)
		{
			nAbsStart += nStart;
			return nAbsStart;
		}
	}
	return -1;
}

/**
desc:		��Դ�ַ����е��ַ����滻�����ַ���

strSrc:		Դ�ַ���
strFrom:	Ҫ�滻���ַ���
strTo:		�����滻�����ַ���

return:		�����滻������ַ���
*/
CString CommonStrMethod::StrReplace(CString strSrc, 
									CString strFrom, 
									CString strTo)
{
	int nLength = strFrom.GetLength();
	CString strSrcTemp = strSrc;
	int nAbsStart = 0;
	BOOL bFound = FALSE;
	int nStart = -1;
	for(; (nStart = strSrcTemp.Find(strFrom.GetAt(0))) != -1;)
	{
		int i = 0, j = nStart;
		for(; i < nLength-1;)
		{
			if(strSrcTemp.GetAt(++j) != strFrom.GetAt(++i))
			{
				strSrcTemp = strSrcTemp.Mid(nStart+1);
				nAbsStart += nStart+1;
				nStart = -1;
				break;
			}
		}
		if(i == nLength-1)
		{
			bFound = TRUE;
			nAbsStart += nStart;
			break;
		}
	}
	if(bFound == FALSE)
	{
		return strSrc;
	}
	CString strDest = strSrc.Left(nAbsStart);
	strDest += strTo;
	strDest += strSrc.Mid(nAbsStart + nLength);

	return StrReplace(strDest, strFrom, strTo);
}

/**
desc:			���ַ������ݷָ����ָ���ַ�������, 
�ָ���������һ���ַ���Ҳ�����Ƕ���ַ���
���磬_T("\n") �� _T(" ") �� _T(" \n,.��")

strSrc:			Դ�ַ���
arrStrDest:		�ַ������飬����ָ�õ��ַ���
strDivisionSet:	�ָ�����

return:			�ɹ������� TRUE�����򣬷��� FALSE
*/
BOOL CommonStrMethod::StrSplit(CString strSrc, 
							   CStringArray& arrStrDest, 
							   CString strDivisionSet)
{
	if(strSrc.IsEmpty() || strDivisionSet.IsEmpty())
	{
		return FALSE;
	}
	int nIndex = -1;
	for(; (nIndex = strSrc.FindOneOf(strDivisionSet)) != -1;)
	{
		CString str = strSrc.Left(nIndex);
		arrStrDest.Add(str);
		strSrc = strSrc.Mid(nIndex+1);
	}
	if(!strSrc.IsEmpty())
	{
		arrStrDest.Add(strSrc);
	}
	return TRUE;
}

/**
desc:		���ַ����в���һ�����ַ���һ�γ��ֵ��±�, �����ִ�Сд

strSrc:		Դ�ַ���
chFind:		Ҫ���ҵ��ַ�

return:		���ҳɹ��������������ַ�����һ�γ��ֵ��±ꣻ
���򣬷��� -1
*/
int CommonStrMethod::FindWcharNoCase(CString& strSrc, WCHAR chFind)
{
	int nRes = -1;
	nRes = strSrc.Find(chFind);
	if(nRes == -1)
	{
		if(chFind >= 'A' && chFind <= 'Z')
			chFind = 'a' + chFind - 'A';
		else if(chFind >= 'a' && chFind <= 'z')
			chFind = 'A' + chFind - 'a';
		nRes = strSrc.Find(chFind);
	}
	return nRes;
}

/**
desc:		�Ƚ��������ַ��Ƿ����, �����ִ�Сд

return:		��ȣ����� TRUE�����򣬷��� FALSE
*/
BOOL CommonStrMethod::CompareWcharNoCase(WCHAR ch1, WCHAR ch2)
{
	BOOL bRes = FALSE;
	bRes = ch1 == ch2;
	if(!bRes)
	{
		if(ch2 >= 'A' && ch2 <= 'Z')
			ch2 = 'a' + ch2 - 'A';
		else if(ch2 >= 'a' && ch2 <= 'z')
			ch2 = 'A' + ch2 - 'a';
		bRes = ch1 == ch2;
	}
	return bRes;
}

/**
desc:				���ַ����л�ȡ����ֵ

strSrc:				Դ�ַ���
strProfileName:		Ҫ��ȡ��������
strProfileValue:	�����ȡ��������ֵ

return:				�ɹ������� TRUE�����򣬷��� FALSE
*/
BOOL CommonStrMethod::StrGetProfileStr(CString strSrc, 
									   CString strProfileName, 
									   CString& strProfileValue,
									   WCHAR wchSplitNameValue, 
									   WCHAR wchSplitProfile)
{
	strProfileName.AppendFormat(_T("%c"), wchSplitNameValue);
	int nStartIndex = StrFind(strSrc, strProfileName);
	if(nStartIndex == -1)
	{
		strProfileValue = _T("");
		return FALSE;
	}
	nStartIndex += strProfileName.GetLength();
	int nEndIndex = strSrc.Find(wchSplitProfile, nStartIndex);
	if(nEndIndex == -1)
	{
		nEndIndex = strSrc.GetLength();
	}
	strProfileValue = strSrc.Mid(nStartIndex, nEndIndex-nStartIndex);
	return TRUE;
}

/**
desc:				���ַ����л�ȡ����ֵ����������������Сд

strSrc:				Դ�ַ���
strProfileName:		Ҫ��ȡ��������
strProfileValue:	�����ȡ��������ֵ

return:				�ɹ������� TRUE�����򣬷��� FALSE
*/
BOOL CommonStrMethod::StrGetProfileStrNoCase(CString strSrc, 
											 CString strProfileName, 
											 CString& strProfileValue,
											 WCHAR wchSplitNameValue, 
											 WCHAR wchSplitProfile)
{
	strProfileName.AppendFormat(_T("%c"), wchSplitNameValue);
	int nStartIndex = StrFindNoCase(strSrc, strProfileName);
	if(nStartIndex == -1)
	{
		strProfileValue = _T("");
		return FALSE;
	}
	nStartIndex += strProfileName.GetLength();
	int nEndIndex = strSrc.Find(wchSplitProfile, nStartIndex);
	if(nEndIndex == -1)
	{
		nEndIndex = strSrc.GetLength();
	}
	strProfileValue = strSrc.Mid(nStartIndex, nEndIndex-nStartIndex);
	return TRUE;
}

/**
desc:		���������ַ��� ת�� ��������

strNum:		�����ַ���

return:		���� ת���õ���������
*/
int CommonStrMethod::Str2Int(CString strNum)
{
	CHAR buf[32] = {0};
	CommonStrMethod::WChar2Char(buf, 32, strNum.GetBuffer());
	return atoi(buf);
}

CString CommonStrMethod::Int2Str(int iNum)
{
	CString strNum;
	strNum.Format(_T("%d"), iNum);
	return strNum;
}

/**

*/
CString CommonStrMethod::GetModuleDir(void)
{
	WCHAR wszModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, wszModulePath, MAX_PATH);
	CString strModulePath(wszModulePath);
	int nPos = strModulePath.ReverseFind(_T('\\'));
	if(nPos < 0)
	{
		return CString();
	}
	else
	{
		return strModulePath.Left(nPos+1);
	}
}

/**

*/
/*
CString CommonStrMethod::GetModuleName(void)
{
WCHAR wszModulePath[MAX_PATH] = {0};
::GetModuleFileName(NULL, wszModulePath, MAX_PATH);

return wszModulePath;
}
*/
/**
desc:			��ȡĳ�ļ����µ��ļ�ȫ·����

strPath:		�ļ���·��
strarrFileList: �����ļ��б���ַ�������

return:			���� ��ǰ��������·��
*/
int CommonStrMethod::GetAbsNameListOfDir(CString strPath, CStringArray& strarrFileList)
{
	CFileFind finder;
	BOOL bFound = finder.FindFile(strPath);
	int nCount = 0;
	for(; bFound; nCount++)
	{
		bFound = finder.FindNextFile();
		strarrFileList.Add(finder.GetFilePath());
	}
	finder.Close();
	return nCount;
}

/**
desc:			��ȡĳ�ļ����µ��ļ�ȫ·����

strPath:		�ļ���·��
strarrFileList: �����ļ��б���ַ�������

return:			���� ��ǰ��������·��
*/
int CommonStrMethod::GetNameListOfDir(CString strPath, CStringArray& strarrFileList)
{
	CFileFind finder;
	BOOL bFound = finder.FindFile(strPath);
	int nCount = 0;
	for(; bFound; nCount++)
	{
		bFound = finder.FindNextFile();
		strarrFileList.Add(finder.GetFileTitle());
	}
	finder.Close();
	return nCount;
}

BOOL CommonStrMethod::GetLocalHostNameAndIP(CString& strHostName, CString& strHostIP)
{
	BOOL bRes = TRUE;
	WSADATA wsaData;
	CHAR szHostName[255] = {0};
	CHAR* szIP = NULL;
	PHOSTENT phostinfo; 
	if ( WSAStartup( MAKEWORD(2,2), &wsaData ) == 0 ) 
	{  
		if( gethostname( szHostName, 255) == 0) 
		{ 
			if((phostinfo = gethostbyname(szHostName)) != NULL) 
			{ //��Щ���ǻ��IP�ĺ���
				szIP= inet_ntoa (*(struct in_addr *)*phostinfo->h_addr_list);
			}
			else
			{
				bRes = FALSE;
				//AfxMessageBox(_T("error:gethostbyname"));
			}
		}
		else
		{
			bRes = FALSE;
			//AfxMessageBox(_T("error:gethostname"));

		}
		WSACleanup();
	}
	else
	{
		bRes = FALSE;
		AfxMessageBox(_T("error:gethostname"));
	}
	Char2WChar(strHostName.GetBuffer(), strlen(szHostName)+2, szHostName);
	if(szIP)
	{
		Char2WChar(strHostIP.GetBuffer(), strlen(szIP)+2, szIP);
	}

	return bRes;
}
/*
CString CommonStrMethod::GetLocalIP()
{
CString strHostName = _T(""), strIP = _T("");
GetLocalHostNameAndIP(strHostName, strIP);
return strIP;
}
*/

CString CommonStrMethod::GetLocalIP()
{
	BOOL bRes = TRUE;
	WSADATA wsaData;
	CHAR szHostName[255] = {0};
	CHAR* szIP = NULL;
	PHOSTENT phostinfo; 
	if ( WSAStartup( MAKEWORD(2,2), &wsaData ) == 0 ) 
	{  
		if( gethostname( szHostName, 255) == 0) 
		{ 
			if((phostinfo = gethostbyname(szHostName)) != NULL) 
			{ //��Щ���ǻ��IP�ĺ���
				szIP= inet_ntoa (*(struct in_addr *)*phostinfo->h_addr_list);
			}
			else
			{
				bRes = FALSE;
			}
		}
		else
		{
			bRes = FALSE;
		}
		WSACleanup();
	}
	else
	{
		bRes = FALSE;
	}
	//WCHAR wszHostName[255] = {0};
	//Char2WChar(wszHostName, 255, szHostName);
	WCHAR wszIP[128] = {0};
	Char2WChar(wszIP, 255, szIP);

	return wszIP;
}

CString CommonStrMethod::GetLocalHostName()
{
	BOOL bRes = TRUE;
	WSADATA wsaData;
	CHAR szHostName[255] = {0};
	CHAR* szIP = NULL;
	PHOSTENT phostinfo; 
	if ( WSAStartup( MAKEWORD(2,2), &wsaData ) == 0 ) 
	{  
		if( gethostname( szHostName, 255) == 0) 
		{ 
			if((phostinfo = gethostbyname(szHostName)) != NULL) 
			{ //��Щ���ǻ��IP�ĺ���
				szIP= inet_ntoa (*(struct in_addr *)*phostinfo->h_addr_list);
			}
			else
			{
				bRes = FALSE;
			}
		}
		else
		{
			bRes = FALSE;
		}
		WSACleanup();
	}
	else
	{
		bRes = FALSE;
	}
	WCHAR wszHostName[255] = {0};
	Char2WChar(wszHostName, 255, szHostName);
	//WCHAR wszIP[128] = {0};
	//Char2WChar(wszIP, 255, szIP);

	return wszHostName;
}

BOOL CommonStrMethod::StrIsNumbers(CString strNum)
{
	if(strNum.IsEmpty()) return FALSE;
	for(int i = 0; i < strNum.GetLength(); i++)
	{
		if(strNum.GetAt(i) < '0' || strNum.GetAt(i) > '9')
		{
			return FALSE;
		}
	}

	return TRUE;
}

CString CommonStrMethod::StrClearUnvisibleChar(CString& strSrc)
{
	int iCount = strSrc.GetLength();
	//WCHAR* pwszBuf = (WCHAR*)malloc(iCount * 2 + 2);
	//memset(pwszBuf, 0, iCount * 2 + 2);
	//WCHAR* p = pwszBuf;
	//for(int i = 0; i < iCount; i++, p++)
	//{
	//	WCHAR wch = strSrc.GetAt(i);
	//	//if(wch > 32)
	//	if(!(wch == ' ' || wch == '\t' || wch == '\r' || wch == '\n'))
	//	{
	//		*p = wch;
	//	}
	//}
	//CString strNew(pwszBuf);
	//free(pwszBuf);
	CString strNew;
	for(int i = 0; i < iCount; i++)
	{
		if(strSrc.GetAt(i) > 32)
		{
			strNew.AppendFormat(_T("%c"), strSrc.GetAt(i));
		}
	}

	return strNew;
}

BOOL CommonStrMethod::PathFileExist(CString strPath)
{
	return ::PathFileExists(strPath);
}

BOOL CommonStrMethod::CreatePath(CString strPath)
{
	strPath.Replace('\\', '/');
	return _CreatePath(strPath);
}

BOOL CommonStrMethod::CreateFile(CString strFile)
{
	CString strFileName = GetNameFromAbsName(strFile);
	CString strPath = GetPathFromAbsName(strFile);
	if(!IsAbsPath(strPath))
	{
		if(strPath.GetLength() > 0)
		{
			if(strPath.GetAt(0) == _T('\\') || strPath.GetAt(0) == _T('/'))
			{
				strPath.Delete(0);
			}
		}
		strPath = GetModuleDir() + strPath;
	}
	if(!CreatePath(strPath))
	{
		return FALSE;
	}
	HANDLE hFile = ::CreateFile(strPath + strFileName, GENERIC_ALL, FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	::CloseHandle(hFile);

	return TRUE;
}

CString CommonStrMethod::GetPathFromAbsName(CString strAbsName)
{
	int nPos = strAbsName.ReverseFind(_T('\\'));
	if(nPos < 0)
	{
		return CString();
	}
	else
	{
		return strAbsName.Left(nPos+1);
	}
}

CString CommonStrMethod::GetNameFromAbsName(CString strAbsName)
{
	int nPos = strAbsName.ReverseFind(_T('\\'));
	if(nPos < 0)
	{
		return strAbsName;
	}
	else
	{
		return strAbsName.Mid(nPos+1);
	}
}

BOOL CommonStrMethod::IsAbsPath(CString strAbsPath)
{
	if(strAbsPath.GetLength() < 3)
	{
		strAbsPath += _T("\\");
	}
	return ::PathFileExists(strAbsPath.Left(3));
}

CString CommonStrMethod::GetPrivateProfileString(CString strAppName, 
												 CString strKeyName, 
												 CString strDefault, 
												 CString strFileName)
{
	if(!PathFileExists(strFileName))
	{
		if(!CreateFile(strFileName))
		{
			return strDefault;
		}
	}
	WCHAR wszPort[512] = {0};
	DWORD dwRet = ::GetPrivateProfileString(
		strAppName, strKeyName, NULL, wszPort, 512, strFileName);
	if(dwRet == 0)
	{
		//::wcscpy_s(wszPort, 11, Int2Str(MY_DEFAULT_SOCKET_PORT));
		::WritePrivateProfileString(strAppName, strKeyName, strDefault, strFileName);

		return strDefault;
	}

	return wszPort;
}

BOOL CommonStrMethod::WritePrivateProfileString(CString strAppName,
												CString strKeyName,
												CString strProfile,
												CString strFileName)
{
	if(!PathFileExists(strFileName))
	{
		if(!CreateFile(strFileName))
		{
			return FALSE;
		}
	}
	return ::WritePrivateProfileString(
		strAppName, strKeyName, strProfile, strFileName);
}

USHORT CommonStrMethod::NetToHostShort(USHORT uNetShort)
{
	return ::ntohs(uNetShort);
}

CString CommonStrMethod::NetToHostIP(IN_ADDR& in_addrNet)
{
	CHAR * pszIP = ::inet_ntoa(in_addrNet);
	WCHAR wszIP[64] = {0};
	CommonStrMethod::Char2WChar(wszIP, 64, pszIP);

	return wszIP;
}





///////////////////////////////		private method		/////////////////////////////////
// C:/1/2/3/
// 012345678
BOOL CommonStrMethod::_CreatePath(CString strPath)
{
	if(::PathFileExists(strPath))
	{
		return TRUE;
	}
	else
	{
		int iIndex = strPath.ReverseFind('/');
		if(iIndex < 2)
		{
			return FALSE;
		}
		CString strParent = strPath.Left(iIndex);
		if(::PathFileExists(strParent))
		{
			return ::CreateDirectory(strPath, NULL);
		}
		else
		{
			if(CreatePath(strParent))
			{
				return CreatePath(strPath);
			}
			else
			{
				return FALSE;
			}
		}
	}
}

CString CommonStrMethod::GetModuleName(void)
{
	WCHAR wszModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, wszModulePath, MAX_PATH);

	return wszModulePath;
}