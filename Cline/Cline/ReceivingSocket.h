#pragma once

// CReceivingSocket command target
class CClineDlg;
class CReceivingSocket : public CSocket
{
public:
	CReceivingSocket();
	virtual ~CReceivingSocket();

	// ������д���շ���
	void OnReceive(int nErrorCode);

	void SetDlg(CClineDlg* pDlg);

public:
	CString m_userName;

private:
	CClineDlg* m_pDlg;
};


