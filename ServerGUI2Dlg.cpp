
// ServerGUI2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerGUI2.h"
#include "ServerGUI2Dlg.h"
#include "afxdialogex.h"
#include "Server.h"
#include <thread>
#include "Config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerGUI2Dlg �Ի���

Server srv;

CServerGUI2Dlg::CServerGUI2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVERGUI2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerGUI2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, C_List);
	DDX_Control(pDX, IDC_IPADDRESS1, C_IPAddress);
	DDX_Control(pDX, IDC_EDIT1, C_Port);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, C_Browse);
}

BEGIN_MESSAGE_MAP(CServerGUI2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_LIST1, &CServerGUI2Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CServerGUI2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CServerGUI2Dlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CServerGUI2Dlg::OnEnChangeMfceditbrowse1)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CServerGUI2Dlg::OnIpnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDC_BUTTON1, &CServerGUI2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CServerGUI2Dlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CServerGUI2Dlg ��Ϣ�������

BOOL CServerGUI2Dlg::OnInitDialog()
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

void CServerGUI2Dlg::OnPaint()
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
HCURSOR CServerGUI2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerGUI2Dlg::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString str("hello world");
	//C_List.AddString(str);
	
}


void CServerGUI2Dlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());

	//CString recvBuf("HI");
	//dlg->C_List.AddString(CString(recvBuf));

	if(srv.WinsockStartup()!=-1)
		if(srv.ServerStartup()!=-1)
			if (srv.ListenStartup() != -1)
			{
				std::thread AcceptThread(&Server::AcceptConnection, &srv);
				AcceptThread.detach();
				GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
				GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
			}
}


void CServerGUI2Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CServerGUI2Dlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CServerGUI2Dlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

//�����޸�
void CServerGUI2Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());


	//�õ��û������IP��ַ
	BYTE f0,f1,f2,f3;
	//CIPAddressCtrl *ipCtrl = (CIPAddressCtrl *)this->GetDlgItem(IDC_IPADDRESS1);
	//ipCtrl->GetAddress(f0,f1,f2,f3);
	if (!C_IPAddress.IsBlank())
	{
		C_IPAddress.GetAddress(f0, f1, f2, f3);
		CString ip;
		ip.Format(_T("%d.%d.%d.%d"), f0, f1, f2, f3);

		Config::SERVERADDRESS = CW2A(ip.GetString());

	}
	else
	{
		dlg->C_List.AddString(CString("IP IS EMPTY!"));
	}
	//port
	int nValue = GetDlgItemInt(IDC_EDIT1);
	if (nValue <= 0)
	{
		dlg->C_List.AddString(CString("PORT IS ERROR!"));
	}
	else Config::PORT = nValue;

	CString strs;
	GetDlgItemText(IDC_MFCEDITBROWSE1,strs);
	string strDIR;
	strDIR= CW2A(strs.GetString());
	if (strDIR.length() == 0)
	{
		dlg->C_List.AddString(CString("ADDRESS IS EMPTY!"));
	}
	else Config::HTMLDIR = strDIR;
	dlg->C_List.AddString(CString("MODIFY SUCCESS!"));

}


void CServerGUI2Dlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	srv.Close();
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());
	dlg->C_List.AddString(CString("Server Closed!"));
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
}
