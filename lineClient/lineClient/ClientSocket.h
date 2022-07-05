#pragma once

// CClientSocket command target
class ClineClientDlg;

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();

	void SendMSG(const char* pBuf,int nBufSize);

	virtual void OnReceive(int nErrorCode);

	virtual void Close();

public:
	ClineClientDlg* m_pDlg;
};


