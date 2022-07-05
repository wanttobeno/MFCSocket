#include "StdAfx.h"
#include "BasicTranscode.h"
#include <shlobj.h>
#include <AccCtrl.h>
#include <AclAPI.h>
#include <TCHAR.h>
#include <shlwapi.h>

namespace utility
{
	void BasicTranscode::Unicode_to_UTF8( const wchar_t* in, size_t len, std::string& out )
	{
		size_t nUtf8Len = WideCharToMultiByte(CP_UTF8,0,in,len,NULL,0,NULL,NULL);

		char* lpUTF8 = new char[nUtf8Len + 1];
		memset(lpUTF8, 0, nUtf8Len);

		WideCharToMultiByte(CP_UTF8,0,in,len,lpUTF8,nUtf8Len,NULL,NULL);
		out.assign(lpUTF8, nUtf8Len);

		delete[] lpUTF8;
	}

	void BasicTranscode::UTF8_to_Unicode( const char* in, size_t len, std::wstring& out )
	{
		wchar_t* lpBuf = new wchar_t[len + 1];
		size_t out_len = (len + 1) * sizeof(wchar_t);

		memset(lpBuf, 0, (len + 1) * sizeof(wchar_t));

		out_len = ::MultiByteToWideChar(CP_UTF8, 0, in, len, lpBuf, len * sizeof(wchar_t));
		out.assign(lpBuf, out_len);

		delete [] lpBuf;
	}

	void BasicTranscode::ANSI_to_Unicode( const char* in, size_t len, std::wstring& out )
	{
		int wbufferlen = (int)::MultiByteToWideChar(CP_ACP,0, in, (int)len, NULL, 0);
		wchar_t* lpwbuffer = new wchar_t[wbufferlen+1];

		wbufferlen = (int)::MultiByteToWideChar(CP_ACP,0, in, (int)len, lpwbuffer, wbufferlen);
		lpwbuffer[wbufferlen] = '\0';
		out.assign( lpwbuffer, wbufferlen );
		delete[] lpwbuffer;
	}

	void BasicTranscode::ANSI_to_UTF8(const char* in, size_t len, std::string& out)
	{
		std::wstring tempstr;
		ANSI_to_Unicode(in,len,tempstr);
		Unicode_to_UTF8(tempstr.c_str(),tempstr.length(),out);
	}

	void BasicTranscode::Unicode_to_ANSI(const wchar_t* wstr, std::string& str)
	{
		//string str="";
		int nLen = lstrlenW(wstr)*2+100;

		char* pChar = new char[nLen];
		if (pChar)
		{
			memset(pChar,0,nLen);
			::WideCharToMultiByte(CP_ACP,0,wstr,lstrlenW(wstr),pChar,nLen,0,NULL);
			str = pChar;
			delete pChar;
		}
		//return str;
	} 

}
