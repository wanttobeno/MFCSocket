// ReceivingSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Cline.h"
#include "ReceivingSocket.h"
#include "ClineDlg.h"
#include "BasicTranscode.h"

// CReceivingSocket

CReceivingSocket::CReceivingSocket():m_pDlg(NULL)
{
}

CReceivingSocket::~CReceivingSocket()
{
}

void CReceivingSocket::OnReceive(int nErrorCode)
{
	//char* pBuf[1025] = { 0 };
	//int nBuffSize = 1024;
	//int r = this->Receive(pBuf, nBuffSize);
	//if (r != SOCKET_ERROR)
	//{
	//	std::wstring wStr;
	//	utility::BasicTranscode::UTF8_to_Unicode((const char*)pBuf, nBuffSize, wStr);
	//	CString msg = CString(wStr.c_str());
	//	//this->m_pDlg->AddMsg(msg);
	//	int nFind = msg.Find(_T("MUSERNAME"), 0);
	//	this->m_pDlg->translateMsg(msg, this);
	//}
	//else
	//{
	//	AfxMessageBox(_T("数据接收错误！"), MB_OK | MB_ICONSTOP);
	//	return;
	//}

	if (nErrorCode == 0)
	{
		this->m_pDlg->RecvData(this);
	}
}

void CReceivingSocket::SetDlg(CClineDlg * pDlg)
{
	this->m_pDlg = pDlg;
}


// CReceivingSocket member functions
