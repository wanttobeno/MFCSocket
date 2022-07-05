
// ClineDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
class CServerSocket;
class CReceivingSocket;
// CClineDlg �Ի���
class CClineDlg : public CDialogEx
{
// ����
public:
	CClineDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CClineDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLINE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	/*
	* ����������
	*/
	void StartServer();
	/*
	* ֪ͨ�����û����رշ�����
	*/
	void StopServer();
	/*
	* ���½�����Ϣ
	*/
	void UpdateMsgText(CString str);
	/*
	* ���տͻ��˵���Ϣ
	*/
	void RecvData(CReceivingSocket* pSocket);
	/*
	* ������Ϣ
	*/
	void translateMsg(CString str, CReceivingSocket* pSocket);
	/*
	* ���ͻ��˷�����Ϣ
	*/
	void SendMSGToEvery(CString &str);
	void SendMSGToOne(CString &str, CString &userSendName);
	afx_msg void OnBnClickedBnServerStart();
	afx_msg void OnBnClickedBtnSend();
	/*
	* ÿ���ͻ����½�һ��Socket����
	*/
	void AddClient();
	/*
	* �Ƴ�Socket����
	*/
	void RemoveClient(CReceivingSocket* pSocket);
protected:
	CString GetAllUsers();
public:
	bool m_isConnect;
	int m_port;
	int m_userCount;
	char * m_strNoUser;
	CEdit m_editPort;
	CButton m_btnSend;
	CListBox m_list_users_ctrl;
	CServerSocket* m_listenSocket;
	CPtrList m_clientList;
protected:
	CEdit m_edit_show_msg;
	CEdit m_edit_msg;
	CString m_strSendMsg;
};
