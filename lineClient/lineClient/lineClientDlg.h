
// lineClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <memory>
class CClientSocket;

// ClineClientDlg �Ի���
class ClineClientDlg : public CDialogEx
{
// ����
public:
	ClineClientDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~ClineClientDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINECLIENT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	boolean m_connect;
	CClientSocket* m_pSock;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void ConnectServer();
	void DisConnectServer();

public:
	/*
	* �������������Ϣ
	*/
	void SendMsg();
	/*
	*  ���������û��б�
	*/
	void UpdateUsers(CString &str, int index);
	/*
	* ���½�����Ϣ
	*/
	void UpdateMsgText(CString &str);
	/*
	* ���շ�������Ϣ
	*/
	void ReceiveMsg(CString &str, CClientSocket* pSocket);
	/*
	* ����Ҫ������
	*/
	void Offline();

public:
	CString m_userName;
	CButton m_btnSend;
	CButton m_btnConnect;
	UINT m_port;
	CListBox m_list_users_ctrl;
	CString m_str_DataSend;
	CButton m_btnHistory;
	CString m_strServerip;
	CEdit m_edit_msg_ctrl;
	CEdit m_edit_history_ctrl;
};
