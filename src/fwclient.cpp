// fwclient.cpp : 定义应用程序的类行为。
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


// CfwclientApp 构造

CfwclientApp::CfwclientApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CfwclientApp 对象

CfwclientApp	theApp;
extern CString	GetBuildVersionFromReg();
extern int		CompareVersion(LPCTSTR lpszReg,LPCTSTR lpszIni);
// CfwclientApp 初始化

BOOL CfwclientApp::InitInstance()
{
	AfxOleInit();

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

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
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
