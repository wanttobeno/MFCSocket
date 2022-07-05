
// ClineDlg.cpp : ʵ���ļ�
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
CString UPDATEUSERLIST = _T("����1|����2");


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CClineDlg �Ի���



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


// CClineDlg ��Ϣ�������

BOOL CClineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	int n;
	CString noUser = _T("���û������ڻ���δ����");
	n = WideCharToMultiByte(CP_UTF8, 0, noUser, -1, NULL, 0, 0, FALSE);
	m_strNoUser = new char[n+1];
	memset(m_strNoUser, 0, (n+1) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, noUser.GetBuffer(0), n, m_strNoUser, n, 0, FALSE);
	m_isConnect = false;
	this->StartServer();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClineDlg::StartServer()
{
	CString cport;
	//��ȡ�������Ķ˿ں�
	GetDlgItemText(IDC_EDIT_PORT, cport);
	//�˿ںŵ��ַ���ת��Ϊ����
	int port = _ttoi(cport);
	//���û����д�˿ںţ���Ĭ�ϵĶ˿ں�Ϊ1111
	if (port == 0)
	{
		m_port = 6666;
	}
	else
	{
		m_port = port;
	}
	//�����ǰ��������״̬Ϊ��������رշ�����
	if (m_isConnect)
	{
		this->StopServer();
		return;
	}
	//����ʹ򿪷�����
	m_listenSocket = new CServerSocket();
	m_listenSocket->m_pDlg = this;
	m_listenSocket->m_userName = _T("������");

	// �������������׽��֣�IP��ַĬ�ϱ���IP
	if (!m_listenSocket->Create(m_port))
	{
		AfxMessageBox(_T("�����׽��ִ���"));
		m_listenSocket->Close();
		return;
	}
	if (!m_listenSocket->Listen())
	{
		AfxMessageBox(_T("����ʧ�ܣ�"));
		m_listenSocket->Close();
		return;
	}
	m_isConnect = true;
	SetDlgItemText(IDC_BN_SERVER_START, _T("�رշ�����"));
	//�������촰���е���Ϣ
	UpdateMsgText(_T("ϵͳ�򿪷�����."));
	//���������˿ںŵĿؼ�����Ϊֻ��״̬
	if (port == 0)
	{
		m_editPort.SetWindowTextW(_T("1111"));
	}
	else
	{
		m_editPort.SetWindowTextW(cport);
	}
	m_list_users_ctrl.ResetContent();
	m_list_users_ctrl.AddString(_T("������"));
	m_editPort.SetReadOnly(TRUE);
	//���á����͡���ťΪ����
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
		SetDlgItemText(IDC_BN_SERVER_START, _T("�򿪷�����"));
		UpdateMsgText(_T("ϵͳ�رշ�����."));
		m_editPort.SetReadOnly(FALSE);
		//���á����͡���ťΪ����
		m_btnSend.EnableWindow(FALSE);
	}
	m_isConnect = false;
}

void CClineDlg::UpdateMsgText(CString str)
{
	CString string;
	// ��ȡϵͳ��ǰʱ��
	CTime time = CTime::GetCurrentTime();
	// ���ڻ�����ʾ��־
	str += _T("\r\n");
	// ��ʽ����ǰʱ��
	string = time.Format(_T("%Y/%m/%d %H:%M:%S  ")) + str;
	//��ȡ�༭�����һ������
	int lastLine = m_edit_show_msg.LineIndex(m_edit_show_msg.GetLineCount() - 1);
	//ѡ��༭�����һ��
	m_edit_show_msg.SetSel(lastLine + 1, lastLine + 2, 0);
	//�滻��ѡ��һ�е�����
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
	// ����
	int nFindKey = str.Find(_T("MOFFLINE"));
	if (nFindKey == 0) {
		this->RemoveClient(pSocket);
		return;
	}

	// ת�����ݸ������û��������������ݵ��û�
	CString cstr;
	int calls = str.Find(_T("MEVERY"));
	//��ȡ���������
	cstr = str.Right(str.GetLength() - calls - 6);
	//���ѡ�е��ǡ������ˡ�����������˷���Ϣ
	if (calls != -1)
	{
		//��socket�е�userName�;������ݵ��ַ���ƴ����������userName�ǡ���������������ǡ���������ÿ����˵��xxxxx����
		cstr = pSocket->m_userName + _T("��ÿ����˵��") + cstr;
		SendMSGToEvery(cstr);
	}
	//����͸�ָ���ͻ��˷���Ϣ
	else
	{
		//�����ʾ����û������˷������������ʹ���MUSERNAME���ַ�����Ϣ
		int nFind = str.Find(_T("MUSERNAME"));
		if (nFind == 0)
		{
			CString userName = str.Right(str.GetLength() - nFind - 9);
			pSocket->m_userName = userName;
			m_list_users_ctrl.AddString(userName);
			cstr = _T("�û�") + userName + _T("���ӷ�����");
			UpdateMsgText(cstr);
			//�����пͻ��˰���������������ĳĳĳ�������˷���������Ϣ
			SendMSGToEvery(cstr);
			Sleep(100);
			//���·����������пͻ��˵��������б�
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
	strMsg = userSendName + _T("����˵��") + strMsg;

	std::string out_str;
	utility::BasicTranscode::Unicode_to_UTF8(strMsg.GetBuffer(0), strMsg.GetLength(), out_str);

	POSITION nPos = m_clientList.GetHeadPosition();
	//����Ŀ��ͻ���
	while (nPos)
	{
		pTemp = (CReceivingSocket*)m_clientList.GetNext(nPos);
		//����ҵ���Ŀ��ͻ��ˣ����շ���
		if (userName == pTemp->m_userName)
		{
			flag = true;
			pTemp->Send(out_str.c_str(), out_str.length());
		}
		//����ҵ��˷��ͷ�
		if (userSendName == pTemp->m_userName)
		{
			pSendUser = pTemp;
		}
	}
	//���û���ҵ����ͷ�����Ĭ���Ƿ�������һ��ָ���Ŀͻ��˷�����Ϣ
	if (!flag&&pSendUser!=NULL)
	{
		if (userName == __T("������")) {
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
		CString send_user = _T("������");
		CString recv_user;
		m_list_users_ctrl.GetText(nSel, recv_user);
		CString strMsg = recv_user +_T(" ") + m_strSendMsg;
		if (recv_user == _T("������")) {
			this->SendMSGToEvery(strMsg);
		}
		else
		{
			this->SendMSGToOne(strMsg, send_user);
		}
		CString info = _T("������Ϣ��") + recv_user + _T(":") + m_strSendMsg;
		this->UpdateMsgText(info);
	}
}

//����µĿͻ���
void CClineDlg::AddClient()
{
	CReceivingSocket *pSocket = new CReceivingSocket;
	pSocket->SetDlg(this);
	m_listenSocket->Accept(*pSocket);
	pSocket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
	m_clientList.AddTail(pSocket);
	m_userCount = m_clientList.GetCount();
}

//�Ƴ��Ѿ��˳����ӵĿͻ���
void CClineDlg::RemoveClient(CReceivingSocket* pSocket)
{
	POSITION nPos = m_clientList.GetHeadPosition();
	POSITION nTmpPos = nPos;
	//�ڿͻ����б��в��Ҹÿͻ���
	while (nPos)
	{
		CReceivingSocket* pSockItem = (CReceivingSocket*)m_clientList.GetNext(nPos);
		//�ҵ��˾�ɾ����
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
			CString msg = _T("�û�") + userName + _T("�뿪");
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