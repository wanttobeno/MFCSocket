
// lineClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "lineClient.h"
#include "lineClientDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "ClientSocket.h"
#include "BasicTranscode.h"
#include "StringHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ClineClientDlg 对话框



ClineClientDlg::ClineClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LINECLIENT_DIALOG, pParent)
	, m_userName(_T("小丽"))
	, m_port(6666)
	, m_str_DataSend(_T(""))
	, m_strServerip(_T("127.0.0.1"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

ClineClientDlg::~ClineClientDlg()
{
	if (m_pSock != NULL) {
		m_pSock->Close();
		delete m_pSock;
		m_pSock = NULL;
	}

}

void ClineClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, m_userName);
	DDX_Control(pDX, IDC_BTN_SEND, m_btnSend);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnConnect);
	DDX_Text(pDX, IDC_SERVERPORT, m_port);
	DDX_Control(pDX, IDC_LIST_USERS, m_list_users_ctrl);
	DDX_Text(pDX, IDC_EDIT_MSG, m_str_DataSend);
	DDX_Control(pDX, IDC_BTN_HISTORY_MSG, m_btnHistory);
	DDX_Text(pDX, IDC_SERVERIPADDRESS, m_strServerip);
	DDX_Control(pDX, IDC_EDIT_SHOW_MSG, m_edit_msg_ctrl);
	DDX_Control(pDX, IDC_EDIT_SHOW_HISTORY, m_edit_history_ctrl);
}

BEGIN_MESSAGE_MAP(ClineClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &ClineClientDlg::ConnectServer)
	ON_BN_CLICKED(IDC_BTN_SEND, &ClineClientDlg::SendMsg)
END_MESSAGE_MAP()


// ClineClientDlg 消息处理程序

BOOL ClineClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void ClineClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR ClineClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//连接服务器
void ClineClientDlg::ConnectServer()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	//如果已经连接，则断开服务器
	if (m_connect)
	{
		this->DisConnectServer();
		return;
	}
	//否则连接服务器
	else
	{
		//在这里创建新的socket
		m_pSock = new CClientSocket();
		m_pSock->m_pDlg = this;
		//创建套接字
		if (!m_pSock->Create())
		{
			AfxMessageBox(_T("创建套接字失败！"));
			return;
		}
	}
	//连接服务器
	if (!m_pSock->Connect(m_strServerip, m_port))
	{
		AfxMessageBox(_T("连接服务器失败！"));
		return;
	}
	else
	{
		m_connect = true;
		int n = 0;
		CString userName;
		userName = _T("MUSERNAME") + m_userName;
		std::string out_str;
		utility::BasicTranscode::Unicode_to_UTF8(userName.GetBuffer(0), userName.GetLength(), out_str);
		m_pSock->SendMSG(out_str.c_str(), out_str.length());

		m_btnConnect.SetWindowTextW(_T("断开服务器"));
		m_btnSend.EnableWindow(TRUE);
		m_btnHistory.EnableWindow(TRUE);
		UpdateData(false);
	}
}

void ClineClientDlg::DisConnectServer() {
	//关闭并且删除客户端的socket
	if (m_pSock == NULL) {
		return;
	}
	m_connect = false;
	m_pSock->Close();
	delete m_pSock;
	m_pSock = NULL;
	m_list_users_ctrl.ResetContent();
	m_btnConnect.SetWindowTextW(_T("连接服务器"));
	m_btnSend.EnableWindow(FALSE);
	m_btnHistory.EnableWindow(FALSE);
	this->UpdateMsgText(CString(_T("断开连接！")));
}


void ClineClientDlg::SendMsg()
{
	UpdateData(true);
	//未连接服务器则不执行
	if (!m_connect)
	{
		return;
	}
	int selectIndex = m_list_users_ctrl.GetCurSel();
	if (selectIndex == -1)
	{
		MessageBox(_T("请在聊天室中选择发送对象！"));
		return;
	}
	if (m_str_DataSend == _T(""))
	{
		MessageBox(_T("请输入要发送的消息内容！"));
		return;
	}
	CString usermsg;
	CString user;
	m_list_users_ctrl.GetText(selectIndex, user);

	if (user == "所有人")
	{
		usermsg = _T("MEVERY") + m_str_DataSend;
	}
	else
	{
		usermsg = _T("M") + user + _T(" ") + m_str_DataSend;
	}
	if (!usermsg.IsEmpty())
	{
		CString info = _T("您对") + user + _T("说：") + m_str_DataSend;
		this->UpdateMsgText(info);
		std::string out_str;
		utility::BasicTranscode::Unicode_to_UTF8(usermsg.GetBuffer(0), usermsg.GetLength(), out_str);
		m_pSock->SendMSG(out_str.c_str(), out_str.length());
	}
}

//更新聊天室列表
void ClineClientDlg::UpdateUsers(CString &str, int index)
{
	CString str_users = str.Right(str.GetLength() - strlen("MUPDATEUSERLIST"));
	CStringArray strArray;
	StringHelper::Split(strArray, str_users, _T("|"));

	int size = strArray.GetSize();
	m_list_users_ctrl.ResetContent();
	for (int i = 0; i < size; i++)
	{
		CString user = strArray.GetAt(i);
		m_list_users_ctrl.AddString(user);
	}
}

void ClineClientDlg::UpdateMsgText(CString &str) {
	CString buff;
	m_edit_msg_ctrl.GetWindowText(buff);
	if (buff.IsEmpty()) {
		buff = str;
	}
	else {
		buff = buff + _T("\r\n") + str;
	}
	m_edit_msg_ctrl.SetWindowText(buff);
	UpdateData(FALSE);
	m_edit_msg_ctrl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void ClineClientDlg::ReceiveMsg(CString &str, CClientSocket* pSocket)
{
	int nFind = str.Find(_T("MUPDATEUSERLIST"));
	if (nFind == 0) {
		this->UpdateUsers(str,0);
		return;
	}
	nFind = str.Find(_T("MOFFSERVER"));
	if (nFind == 0) {
		this->DisConnectServer();
		return;
	}
	this->UpdateMsgText(str);
}

void ClineClientDlg::Offline()
{
	CString msg = _T("MOFFLINE");
	msg = msg + m_userName;

	std::string out_str;
	utility::BasicTranscode::Unicode_to_UTF8(msg.GetBuffer(0), msg.GetLength(), out_str);
	m_pSock->SendMSG(out_str.c_str(), out_str.length());
}

