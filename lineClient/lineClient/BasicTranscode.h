#pragma once
#include <string>
using namespace std;

namespace utility
{
	/*
	 *	desc: 编码转换
	 */
	class BasicTranscode
	{
	public:
		/*
		 *	功能: unicode to utf-8
		 */
		static void Unicode_to_UTF8(const wchar_t* in, size_t len, std::string& out);
		
		/*
		 *	功能: utf-8 to unicode
		 */
		static void UTF8_to_Unicode(const char* in, size_t len, std::wstring& out);

		/*
		 *	功能: ansic to utfunicode8
		 */
		static void ANSI_to_Unicode(const char* in, size_t len, std::wstring& out);

		/*
		 *	功能: ansic to utf-8
		 */
		static void ANSI_to_UTF8(const char* in, size_t len, std::string& out);

		/*
		 *	功能: unicode to ansic
		 */
		static void Unicode_to_ANSI(const wchar_t* wstr, std::string& str);
	};
}

