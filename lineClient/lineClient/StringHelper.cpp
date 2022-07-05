#include "stdafx.h"
#include "StringHelper.h"


StringHelper::StringHelper()
{
}


StringHelper::~StringHelper()
{
}



//Í¨¹ý¡°|¡±·ûºÅ²ÃÇÐ×Ö·û´®ÐÅÏ¢
void StringHelper::Split(CStringArray& strResult, CString strInput, CString strFlag)
{
	int nPos = strInput.Find(strFlag);
	CString strLeft;
	while (0 <= nPos)
	{
		strLeft = strInput.Left(nPos);
		if (!strLeft.IsEmpty())
		{
			strResult.Add(strLeft);
		}
		strInput = strInput.Right(strInput.GetLength() - nPos - strFlag.GetLength());
		nPos = strInput.Find(strFlag);
	}
	if (!strInput.IsEmpty())
	{
		strResult.Add(strInput);
	}
}
