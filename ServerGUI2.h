
// ServerGUI2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CServerGUI2App: 
// �йش����ʵ�֣������ ServerGUI2.cpp
//

class CServerGUI2App : public CWinApp
{
public:
	CServerGUI2App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServerGUI2App theApp;
