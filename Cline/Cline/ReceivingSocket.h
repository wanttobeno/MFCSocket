#pragma once

// CReceivingSocket command target
class CClineDlg;
class CReceivingSocket : public CSocket
{
public:
	CReceivingSocket();
	virtual ~CReceivingSocket();

	// 必须重写接收方法
	void OnReceive(int nErrorCode);

	void SetDlg(CClineDlg* pDlg);

public:
	CString m_userName;

private:
	CClineDlg* m_pDlg;
};


