
// lineClientDlg.cpp : ʵ���ļ�
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



// ClineClientDlg �Ի���



ClineClientDlg::ClineClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LINECLIENT_DIALOG, pParent)
	, m_userName(_T("С��"))
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


// ClineClientDlg ��Ϣ�������

BOOL ClineClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ClineClientDlg::OnPaint()
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
HCURSOR ClineClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���ӷ�����
void ClineClientDlg::ConnectServer()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	//����Ѿ����ӣ���Ͽ�������
	if (m_connect)
	{
		this->DisConnectServer();
		return;
	}
	//�������ӷ�����
	else
	{
		//�����ﴴ���µ�socket
		m_pSock = new CClientSocket();
		m_pSock->m_pDlg = this;
		//�����׽���
		if (!m_pSock->Create())
		{
			AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
			return;
		}
	}
	//���ӷ�����
	if (!m_pSock->Connect(m_strServerip, m_port))
	{
		AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
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

		m_btnConnect.SetWindowTextW(_T("�Ͽ�������"));
		m_btnSend.EnableWindow(TRUE);
		m_btnHistory.EnableWindow(TRUE);
		UpdateData(false);
	}
}

void ClineClientDlg::DisConnectServer() {
	//�رղ���ɾ���ͻ��˵�socket
	if (m_pSock == NULL) {
		return;
	}
	m_connect = false;
	m_pSock->Close();
	delete m_pSock;
	m_pSock = NULL;
	m_list_users_ctrl.ResetContent();
	m_btnConnect.SetWindowTextW(_T("���ӷ�����"));
	m_btnSend.EnableWindow(FALSE);
	m_btnHistory.EnableWindow(FALSE);
	this->UpdateMsgText(CString(_T("�Ͽ����ӣ�")));
}


void ClineClientDlg::SendMsg()
{
	UpdateData(true);
	//δ���ӷ�������ִ��
	if (!m_connect)
	{
		return;
	}
	int selectIndex = m_list_users_ctrl.GetCurSel();
	if (selectIndex == -1)
	{
		MessageBox(_T("������������ѡ���Ͷ���"));
		return;
	}
	if (m_str_DataSend == _T(""))
	{
		MessageBox(_T("������Ҫ���͵���Ϣ���ݣ�"));
		return;
	}
	CString usermsg;
	CString user;
	m_list_users_ctrl.GetText(selectIndex, user);

	if (user == "������")
	{
		usermsg = _T("MEVERY") + m_str_DataSend;
	}
	else
	{
		usermsg = _T("M") + user + _T(" ") + m_str_DataSend;
	}
	if (!usermsg.IsEmpty())
	{
		CString info = _T("����") + user + _T("˵��") + m_str_DataSend;
		this->UpdateMsgText(info);
		std::string out_str;
		utility::BasicTranscode::Unicode_to_UTF8(usermsg.GetBuffer(0), usermsg.GetLength(), out_str);
		m_pSock->SendMSG(out_str.c_str(), out_str.length());
	}
}

//�����������б�
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

