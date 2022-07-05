#pragma once
#include <afxwin.h>

class StringHelper
{
public:
	StringHelper();
	~StringHelper();
	static void StringHelper::Split(CStringArray& strResult, CString strInput, CString strFlag);
};

