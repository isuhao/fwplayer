// fwclient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "fwclient.h"
#include "ClientDlg.h"

#pragma warning(disable:4018 4267 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfwclientApp

BEGIN_MESSAGE_MAP(CfwclientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CfwclientApp ����

CfwclientApp::CfwclientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CfwclientApp ����

CfwclientApp	theApp;
extern CString	GetBuildVersionFromReg();
extern int		CompareVersion(LPCTSTR lpszReg,LPCTSTR lpszIni);
// CfwclientApp ��ʼ��

BOOL CfwclientApp::InitInstance()
{
	AfxOleInit();

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CClientDlg		DlgClient;
	BOOL bCreate =DlgClient.Create(IDD_FWCLIENT_DIALOG);
	m_pMainWnd = &DlgClient;
	DlgClient.ShowWindow(FALSE);
	DlgClient.SetWindowText(_T("FireWind Player"));
	DlgClient.ShowWindow(TRUE);

	MSG	msg;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		HWND	hDlgModeless = DlgClient.GetSafeHwnd();
		if ( hDlgModeless== 0 || !IsDialogMessage (hDlgModeless, &msg))
		{
			TranslateMessage (&msg) ;
			DispatchMessage  (&msg) ;
		}
	}
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
