
// ClineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Cline.h"
#include "ClineDlg.h"
#include "afxdialogex.h"

#include "resource.h"
#include "ServerSocket.h"
#include "ReceivingSocket.h"
#include "BasicTranscode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 
CString UPDATEUSERLIST = _T("测试1|测试2");


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClineDlg 对话框



CClineDlg::CClineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLINE_DIALOG, pParent)
	, m_port(6666), m_userCount(0)
	, m_strSendMsg(_T(""))
	, m_strNoUser(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_isConnect = false;
}

CClineDlg::~CClineDlg()
{
	this->StopServer();

	if (m_listenSocket != NULL) {
		m_listenSocket->Close();
		delete m_listenSocket;
		m_listenSocket = NULL;
	}

	if (m_strNoUser != NULL) {
		delete m_strNoUser;
		m_strNoUser = NULL;
	}
}

void CClineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
	DDX_Control(pDX, IDC_BTN_SEND, m_btnSend);
	DDX_Control(pDX, IDC_LIST_USERS, m_list_users_ctrl);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Control(pDX, IDC_EDIT_MSG_SHOW, m_edit_show_msg);
	DDX_Control(pDX, IDC_EDIT_MSG_INPUT, m_edit_msg);
	DDX_Text(pDX, IDC_EDIT_MSG_INPUT, m_strSendMsg);
}

BEGIN_MESSAGE_MAP(CClineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BN_SERVER_START, &CClineDlg::OnBnClickedBnServerStart)
	ON_BN_CLICKED(IDC_BTN_SEND, &CClineDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CClineDlg 消息处理程序

BOOL CClineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	int n;
	CString noUser = _T("该用户不存在或者未上线");
	n = WideCharToMultiByte(CP_UTF8, 0, noUser, -1, NULL, 0, 0, FALSE);
	m_strNoUser = new char[n+1];
	memset(m_strNoUser, 0, (n+1) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, noUser.GetBuffer(0), n, m_strNoUser, n, 0, FALSE);
	m_isConnect = false;
	this->StartServer();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClineDlg::OnPaint()
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
HCURSOR CClineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClineDlg::StartServer()
{
	CString cport;
	//获取服务器的端口号
	GetDlgItemText(IDC_EDIT_PORT, cport);
	//端口号的字符串转换为数字
	int port = _ttoi(cport);
	//如果没有填写端口号，则默认的端口号为1111
	if (port == 0)
	{
		m_port = 6666;
	}
	else
	{
		m_port = port;
	}
	//如果当前服务器的状态为开启，则关闭服务器
	if (m_isConnect)
	{
		this->StopServer();
		return;
	}
	//否则就打开服务器
	m_listenSocket = new CServerSocket();
	m_listenSocket->m_pDlg = this;
	m_listenSocket->m_userName = _T("服务器");

	// 创建服务器的套接字，IP地址默认本机IP
	if (!m_listenSocket->Create(m_port))
	{
		AfxMessageBox(_T("创建套接字错误！"));
		m_listenSocket->Close();
		return;
	}
	if (!m_listenSocket->Listen())
	{
		AfxMessageBox(_T("监听失败！"));
		m_listenSocket->Close();
		return;
	}
	m_isConnect = true;
	SetDlgItemText(IDC_BN_SERVER_START, _T("关闭服务器"));
	//更新聊天窗口中的消息
	UpdateMsgText(_T("系统打开服务器."));
	//将服务器端口号的控件设置为只读状态
	if (port == 0)
	{
		m_editPort.SetWindowTextW(_T("1111"));
	}
	else
	{
		m_editPort.SetWindowTextW(cport);
	}
	m_list_users_ctrl.ResetContent();
	m_list_users_ctrl.AddString(_T("所有人"));
	m_editPort.SetReadOnly(TRUE);
	//设置“发送”按钮为可用
	m_btnSend.EnableWindow(TRUE);
}

void CClineDlg::StopServer()
{
	if (m_isConnect == false) {
		return;
	}
	this->SendMSGToEvery(CString(_T("MOFFSERVER")));

	POSITION nPos = m_clientList.GetHeadPosition();
	while (nPos)
	{
		CReceivingSocket* pTemp = (CReceivingSocket*)m_clientList.GetNext(nPos);
		pTemp->Close();
		delete pTemp;
	}
	m_clientList.RemoveAll();

	if (m_listenSocket != NULL) {
		m_listenSocket->Close();
		delete m_listenSocket;
		m_listenSocket = NULL;
	}
	if (m_edit_show_msg.m_hWnd != NULL)
	{
		SetDlgItemText(IDC_BN_SERVER_START, _T("打开服务器"));
		UpdateMsgText(_T("系统关闭服务器."));
		m_editPort.SetReadOnly(FALSE);
		//设置“发送”按钮为可用
		m_btnSend.EnableWindow(FALSE);
	}
	m_isConnect = false;
}

void CClineDlg::UpdateMsgText(CString str)
{
	CString string;
	// 获取系统当前时间
	CTime time = CTime::GetCurrentTime();
	// 用于换行显示日志
	str += _T("\r\n");
	// 格式化当前时间
	string = time.Format(_T("%Y/%m/%d %H:%M:%S  ")) + str;
	//获取编辑框最后一行索引
	int lastLine = m_edit_show_msg.LineIndex(m_edit_show_msg.GetLineCount() - 1);
	//选择编辑框最后一行
	m_edit_show_msg.SetSel(lastLine + 1, lastLine + 2, 0);
	//替换所选那一行的内容
	m_edit_show_msg.ReplaceSel(string);
}


void CClineDlg::RecvData(CReceivingSocket* pSocket)
{
	char* pData = NULL;
	pData = new char[1025];
	memset(pData, 0, sizeof(char) * 1025);
	UCHAR leng = 0;
	CString str;
	if (pSocket->Receive(pData, 1024, 0) != SOCKET_ERROR)
	{
		std::wstring out_str;
		utility::BasicTranscode::UTF8_to_Unicode(pData,1024, out_str);
		str = out_str.c_str();
		translateMsg(str, pSocket);
	}
	delete pData;
}


void CClineDlg::translateMsg(CString str, CReceivingSocket* pSocket)
{
	// 离线
	int nFindKey = str.Find(_T("MOFFLINE"));
	if (nFindKey == 0) {
		this->RemoveClient(pSocket);
		return;
	}

	// 转发数据给所有用户，包括发送数据的用户
	CString cstr;
	int calls = str.Find(_T("MEVERY"));
	//获取具体的内容
	cstr = str.Right(str.GetLength() - calls - 6);
	//如果选中的是“所有人”，则给所有人发消息
	if (calls != -1)
	{
		//将socket中的userName和具体内容的字符串拼接起来（如userName是“服务器”，则就是“服务器对每个人说：xxxxx”）
		cstr = pSocket->m_userName + _T("对每个人说：") + cstr;
		SendMSGToEvery(cstr);
	}
	//否则就给指定客户端发消息
	else
	{
		//这里表示如果用户连接了服务器，将发送带有MUSERNAME的字符串消息
		int nFind = str.Find(_T("MUSERNAME"));
		if (nFind == 0)
		{
			CString userName = str.Right(str.GetLength() - nFind - 9);
			pSocket->m_userName = userName;
			m_list_users_ctrl.AddString(userName);
			cstr = _T("用户") + userName + _T("连接服务器");
			UpdateMsgText(cstr);
			//给所有客户端包括服务器都发送某某某连接上了服务器的消息
			SendMSGToEvery(cstr);
			Sleep(100);
			//更新服务器和所有客户端的聊天室列表
			CString userList = _T("MUPDATEUSERLIST") + this->GetAllUsers();
			SendMSGToEvery(userList);
		}
		else
		{
			int call = str.Find(_T("M"));
			cstr = str.Right(str.GetLength() - call - 1);
			if (call != -1)
			{
				SendMSGToOne(cstr, pSocket->m_userName);
			}
		}
	}
}

void CClineDlg::SendMSGToEvery(CString &str)
{
	std::string out_str;
	utility::BasicTranscode::Unicode_to_UTF8(str.GetBuffer(0), str.GetLength(), out_str);

	POSITION nPos = m_clientList.GetHeadPosition();
	while (nPos)
	{
		CReceivingSocket* pTemp = (CReceivingSocket*)m_clientList.GetNext(nPos);
		pTemp->Send(out_str.c_str(), out_str.length());
	}
}


void CClineDlg::SendMSGToOne(CString& str, CString& userSendName)
{
	bool flag = false;
	CReceivingSocket* pTemp = NULL;
	CReceivingSocket* pSendUser = NULL;
	int cone = str.Find(_T(" "));
	CString userName = str.Left(cone);
	CString strMsg = str.Right(str.GetLength() - cone);
	strMsg = userSendName + _T("对你说：") + strMsg;

	std::string out_str;
	utility::BasicTranscode::Unicode_to_UTF8(strMsg.GetBuffer(0), strMsg.GetLength(), out_str);

	POSITION nPos = m_clientList.GetHeadPosition();
	//查找目标客户端
	while (nPos)
	{
		pTemp = (CReceivingSocket*)m_clientList.GetNext(nPos);
		//如果找到了目标客户端（接收方）
		if (userName == pTemp->m_userName)
		{
			flag = true;
			pTemp->Send(out_str.c_str(), out_str.length());
		}
		//如果找到了发送方
		if (userSendName == pTemp->m_userName)
		{
			pSendUser = pTemp;
		}
	}
	//如果没有找到发送方，则默认是服务器给一个指定的客户端发送消息
	if (!flag&&pSendUser!=NULL)
	{
		if (userName == __T("服务器")) {
			flag = true;
			pSendUser->Send(out_str.c_str(), out_str.length());
		}
		else {
			pSendUser->Send(m_strNoUser, strlen(m_strNoUser));
		}
	}
	//delete pSend;
}

void CClineDlg::OnBnClickedBnServerStart()
{
	// TODO: Add your control notification handler code here
	this->StartServer();
}


void CClineDlg::OnBnClickedBtnSend()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int nSel = m_list_users_ctrl.GetCurSel();
	if (nSel < -1) {
		this->SendMSGToEvery(m_strSendMsg);
	}
	else
	{
		CString send_user = _T("服务器");
		CString recv_user;
		m_list_users_ctrl.GetText(nSel, recv_user);
		CString strMsg = recv_user +_T(" ") + m_strSendMsg;
		if (recv_user == _T("所有人")) {
			this->SendMSGToEvery(strMsg);
		}
		else
		{
			this->SendMSGToOne(strMsg, send_user);
		}
		CString info = _T("发送消息给") + recv_user + _T(":") + m_strSendMsg;
		this->UpdateMsgText(info);
	}
}

//添加新的客户端
void CClineDlg::AddClient()
{
	CReceivingSocket *pSocket = new CReceivingSocket;
	pSocket->SetDlg(this);
	m_listenSocket->Accept(*pSocket);
	pSocket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
	m_clientList.AddTail(pSocket);
	m_userCount = m_clientList.GetCount();
}

//移除已经退出连接的客户端
void CClineDlg::RemoveClient(CReceivingSocket* pSocket)
{
	POSITION nPos = m_clientList.GetHeadPosition();
	POSITION nTmpPos = nPos;
	//在客户端列表中查找该客户端
	while (nPos)
	{
		CReceivingSocket* pSockItem = (CReceivingSocket*)m_clientList.GetNext(nPos);
		//找到了就删除它
		if (pSockItem->m_hSocket == pSocket->m_hSocket)
		{
			CString userName;
			userName = pSockItem->m_userName;
			pSockItem->Close();
			delete pSockItem;
			int userIndex = m_list_users_ctrl.FindString(0, userName);
			m_list_users_ctrl.DeleteString(userIndex);
			m_clientList.RemoveAt(nTmpPos);
			m_userCount = m_clientList.GetCount();
			UpdateData(false);
			CString msg = _T("用户") + userName + _T("离开");
			UpdateMsgText(msg);
			break;
		}
		nTmpPos = nPos;
	}
	CString userList = _T("MUPDATEUSERLIST") + this->GetAllUsers();
	SendMSGToEvery(userList);
}


CString  CClineDlg::GetAllUsers() {
	int nCount = m_list_users_ctrl.GetCount();
	CString users = _T("");
	for (int i = 0; i < nCount; i++) {
		CString user;
		m_list_users_ctrl.GetText(i, user);
		if (i == 0) {
			users = user;
		}
		else
		{
			users = users + _T("|") + user;
		}
	}
	return users;
}