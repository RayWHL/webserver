
// ServerGUI2Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxeditbrowsectrl.h"


// CServerGUI2Dlg �Ի���
class CServerGUI2Dlg : public CDialogEx
{
// ����
public:
	CServerGUI2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERGUI2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton2();
	CListBox C_List;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeMfceditbrowse1();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	CIPAddressCtrl C_IPAddress;
	CEdit C_Port;
	CMFCEditBrowseCtrl C_Browse;
	afx_msg void OnBnClickedButton3();
};
