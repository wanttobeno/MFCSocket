
// ClineDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
class CServerSocket;
class CReceivingSocket;
// CClineDlg 对话框
class CClineDlg : public CDialogEx
{
// 构造
public:
	CClineDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CClineDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLINE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	/*
	* 启动服务器
	*/
	void StartServer();
	/*
	* 通知所有用户，关闭服务器
	*/
	void StopServer();
	/*
	* 更新界面消息
	*/
	void UpdateMsgText(CString str);
	/*
	* 接收客户端的消息
	*/
	void RecvData(CReceivingSocket* pSocket);
	/*
	* 处理消息
	*/
	void translateMsg(CString str, CReceivingSocket* pSocket);
	/*
	* 给客户端发送消息
	*/
	void SendMSGToEvery(CString &str);
	void SendMSGToOne(CString &str, CString &userSendName);
	afx_msg void OnBnClickedBnServerStart();
	afx_msg void OnBnClickedBtnSend();
	/*
	* 每个客户端新建一个Socket对象
	*/
	void AddClient();
	/*
	* 移除Socket对象
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
