#pragma once

// CServerSocket command target

class CClineDlg;

class CServerSocket : public CSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	// 服务器必须新建一个socket用于接收，及处理后面的消息。
	virtual void OnAccept(int nErrorCode);


public:
	CString m_userName;
	CClineDlg* m_pDlg;
};


