#pragma once

// CServerSocket command target

class CClineDlg;

class CServerSocket : public CSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	// �����������½�һ��socket���ڽ��գ�������������Ϣ��
	virtual void OnAccept(int nErrorCode);


public:
	CString m_userName;
	CClineDlg* m_pDlg;
};


