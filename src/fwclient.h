// fwclient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


class CClientDlg;

// CfwclientApp:
// �йش����ʵ�֣������ fwclient.cpp
//

class CfwclientApp : public CWinApp
{
public:
	CfwclientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

};

extern CfwclientApp theApp;
