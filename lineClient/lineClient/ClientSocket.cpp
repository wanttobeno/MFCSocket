// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "lineClient.h"
#include "ClientSocket.h"
#include "BasicTranscode.h"
#include "lineClientDlg.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::SendMSG(const char * pBuf, int nBufSize)
{
	int nSend = this->Send(pBuf, nBufSize);
}


void CClientSocket::OnReceive(int nErrorCode)
{
	char* pBuf[1025] = { 0 };
	int nBuffSize = 1024;
	int r = this->Receive(pBuf, nBuffSize);
	if (r != SOCKET_ERROR)
	{
		std::wstring wStr;
		utility::BasicTranscode::UTF8_to_Unicode((const char*)pBuf, nBuffSize, wStr);
		CString msg = CString(wStr.c_str());
		this->m_pDlg->ReceiveMsg(msg, this);
	}
	else
	{
		AfxMessageBox(_T("数据接收错误！"), MB_OK | MB_ICONSTOP);
		return;
	}
}

void CClientSocket::Close() {

	this->m_pDlg->Offline();

	CSocket::Close();
}

// CClientSocket member functions
