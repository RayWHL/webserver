
// ServerGUI2Dlg.cpp : 实现文件
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


// CServerGUI2Dlg 对话框

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


// CServerGUI2Dlg 消息处理程序

BOOL CServerGUI2Dlg::OnInitDialog()
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

void CServerGUI2Dlg::OnPaint()
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
HCURSOR CServerGUI2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerGUI2Dlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString str("hello world");
	//C_List.AddString(str);
	
}


void CServerGUI2Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CServerGUI2Dlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CServerGUI2Dlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//设置修改
void CServerGUI2Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());


	//得到用户输入的IP地址
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
	// TODO: 在此添加控件通知处理程序代码
	srv.Close();
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());
	dlg->C_List.AddString(CString("Server Closed!"));
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
}
