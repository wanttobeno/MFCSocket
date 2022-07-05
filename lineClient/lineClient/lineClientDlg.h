
// lineClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <memory>
class CClientSocket;

// ClineClientDlg 对话框
class ClineClientDlg : public CDialogEx
{
// 构造
public:
	ClineClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~ClineClientDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINECLIENT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	boolean m_connect;
	CClientSocket* m_pSock;


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void ConnectServer();
	void DisConnectServer();

public:
	/*
	* 向服务器发送消息
	*/
	void SendMsg();
	/*
	*  更新在线用户列表
	*/
	void UpdateUsers(CString &str, int index);
	/*
	* 更新界面消息
	*/
	void UpdateMsgText(CString &str);
	/*
	* 接收服务区消息
	*/
	void ReceiveMsg(CString &str, CClientSocket* pSocket);
	/*
	* 主动要求离线
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
