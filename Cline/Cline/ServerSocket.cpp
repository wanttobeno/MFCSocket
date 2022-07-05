// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Cline.h"
#include "ServerSocket.h"
#include "ClineDlg.h"
#include "ReceivingSocket.h"
// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


void CServerSocket::OnAccept(int nErrorCode)
{
	//CReceivingSocket *pSocket = new CReceivingSocket();
	////CServerSocket *pSocket = new CServerSocket();
	//this->Accept(*pSocket);
	//pSocket->SetDlg(m_pDlg);

	////pSocket->m_pDlg = m_pDlg;
	//m_pDlg->m_clientList.AddTail(pSocket);
	if (nErrorCode == 0) {
		m_pDlg->AddClient();
	}
}


// CServerSocket member functions
