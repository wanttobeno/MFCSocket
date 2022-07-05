#pragma once
#include <string>
using namespace std;

namespace utility
{
	/*
	 *	desc: ����ת��
	 */
	class BasicTranscode
	{
	public:
		/*
		 *	����: unicode to utf-8
		 */
		static void Unicode_to_UTF8(const wchar_t* in, size_t len, std::string& out);
		
		/*
		 *	����: utf-8 to unicode
		 */
		static void UTF8_to_Unicode(const char* in, size_t len, std::wstring& out);

		/*
		 *	����: ansic to utfunicode8
		 */
		static void ANSI_to_Unicode(const char* in, size_t len, std::wstring& out);

		/*
		 *	����: ansic to utf-8
		 */
		static void ANSI_to_UTF8(const char* in, size_t len, std::string& out);

		/*
		 *	����: unicode to ansic
		 */
		static void Unicode_to_ANSI(const wchar_t* wstr, std::string& str);
	};
}

