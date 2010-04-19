// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "fwclient.h"
#include "ClientDlg.h"
#include "AboutDlg.h"
#include "OpenUrlDlg.h"

#include "wmp.h"

#pragma warning(disable:4267 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TRACKPOS_TIMER_ID		1113
#define TRACKPOS_TIMER_ELAPSE	1000

#define ID_SWITCHSKIN_FIRST 32790
#define ID_SWITCHSKIN_LAST ID_SWITCHSKIN_FIRST+10

// CClientDlg �Ի���
CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	,_config(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bLoadFlashSkin	=FALSE;
	m_bFullScreen	=FALSE;
	m_dwScreenSave	=0;
	m_bCanChangeProgressByExternProgram	=TRUE;

	m_hSwitchSkinMenu	=NULL;
	m_nSwitchSkinID	= ID_SWITCHSKIN_FIRST;

	m_pVideoPlayer = NULL;
}

CClientDlg::~CClientDlg()
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCX_FLASH, m_FlashPlayer);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	//�˵�����
	ON_COMMAND(ID_FILE_EXIT, &CClientDlg::OnFileExit)
	ON_COMMAND(ID_FILE_OPEN, &CClientDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_OPENURL, &CClientDlg::OnFileOpenurl)
	ON_COMMAND(ID_HELP_ABOUT, &CClientDlg::OnHelpAbout)
	ON_COMMAND(ID_HELP_WEBSITE, &CClientDlg::OnHelpWebsite)
	ON_COMMAND_RANGE(ID_SWITCHSKIN_FIRST,ID_SWITCHSKIN_LAST,OnSwitchSkinRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SWITCHSKIN_FIRST,ID_SWITCHSKIN_LAST,OnUpdateSwitchSkinRange)

	//��������Ϣ
	ON_MESSAGE(WM_VIDEO_OPEN_STATE, OnWmpOpenState)
	ON_MESSAGE(WM_VIDEO_PLAY_STATE, OnWmpPlayState)

	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CClientDlg, CDialog)
	//{{AFX_EVENTSINK_MAP(CClientDlg)
	ON_EVENT(CClientDlg, IDC_OCX_FLASH, 150 /* FSCommand */, OnFSCommandFlashPlayer, VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

// CClientDlg ��Ϣ�������
void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if(nID ==0xf060)
		{
			OnFileExit();
		}
		else 
			CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

void CClientDlg::OnOK()
{
}

void CClientDlg::OnCancel()
{
	if(m_bFullScreen)
	{
		SetFullScreen(FALSE);
	}
}

HBRUSH CClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);	
	pDC->SetBkMode(OPAQUE);

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CClientDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if(_config)
		_config->release();
}


BOOL CClientDlg::OnInitDialog()
{
	USES_CONVERSION;

	BOOL	bRet	=FALSE;
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu("About...");
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//���ڷ���޸�
	//�ޱ߿򴰿�
	ModifyStyle(WS_BORDER|WS_CAPTION|WS_THICKFRAME, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	//�ļ�·��
	TCHAR szAppFile[MAX_PATH];
	GetModuleFileName(NULL,szAppFile,sizeof(szAppFile));

	TCHAR szLongFile[MAX_PATH];
	GetLongPathName(szAppFile, szLongFile, MAX_PATH);

	CString strPathName(szLongFile);
	m_AppPath = strPathName.Left(strPathName.ReverseFind('\\'));
	m_SkinPath = m_AppPath + _T("\\skin");
	CString strConfig = m_AppPath + _T("\\fwplayer.cfg");

	//��������
	_config = new TextConfig();
	ASSERT(_config != NULL);
	_config->initialize(T2A(strConfig));

	//װ�ص�ǰƤ���ļ�
	LoadCurSkin();

	//��һ�β���ʱ��ʼ��������
	m_pVideoPlayer = _playerFactory.CreatePlayer(PT_WMP);
	m_pVideoPlayer->InitPlayer(this);

	//�������ڴ�СΪFlashƤ���ļ���ԭʼ��С
	m_FlashPlayer.put_BackgroundColor(RGB(0,0,0));
	AutoFitFlashOriginSize();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//װ��FlashƤ���ļ�
void CClientDlg::LoadCurSkin(CString strCurSkin)
{
	USES_CONVERSION;
	CString strFlashInterfaceSkinFile;

	if(strCurSkin.IsEmpty()) //���ص�ǰƤ��
	{
		std::string strCurSkin = "GUI.Skin.Current";
		const std::string strSkinItem = _config->getValueWithDefault(strCurSkin,"Default");
		strCurSkin	= "GUI.Skin.File.";
		strCurSkin	+=	strSkinItem;
		std::string	strSkinFile	= _config->getValueWithDefault(strCurSkin,"default.swf");

		
		strFlashInterfaceSkinFile.Format(_T("%s\\%s"),m_SkinPath,A2T(strSkinFile.c_str()));

		StringDict skinfiles = _config->getValuesForPrefix("GUI.Skin.File.");
		StringDict skinorders = _config->getValuesForPrefix("GUI.Skin.Order.");

		StringDict::iterator p;
		std::string	skinorder;

		for(p = skinfiles.begin(); p != skinfiles.end(); ++p)
		{
			std::string skinfile= p->second;
			if(skinfile ==strSkinFile)
			{
				std::string	key	=p->first;
				string_replace(key,"File","Order");
				skinorder = skinorders[key];			
				break;
			}
		}
		m_nSwitchSkinID	=ID_SWITCHSKIN_FIRST+atoi(skinorder.c_str());
	}
	else
	{

		strFlashInterfaceSkinFile.Format(_T("%s\\%s"),m_SkinPath,strCurSkin);

		StringDict skinfiles = _config->getValuesForPrefix("GUI.Skin.File.");

		StringDict::iterator p;

		for(p = skinfiles.begin(); p != skinfiles.end(); ++p)
		{
			std::string skinfile= p->second;
			if(skinfile ==T2A(strCurSkin))
			{
				std::string	skinfilekey	=p->first;
				std::string strsubstrstart ="GUI.Skin.File.";
				std::string strCurSkinItem	=skinfilekey.substr(strsubstrstart.length());
				_config->setValue("GUI.Skin.Current",strCurSkinItem);
				break;
			}
		}
	}

	m_FlashPlayer.LoadMovie(0, strFlashInterfaceSkinFile);
	m_bLoadFlashSkin =( m_FlashPlayer.PercentLoaded() >0);//PercentLoaded From 0 To 100

	if(m_bLoadFlashSkin)
	{
		m_FlashPlayer.Play();
		m_FlashPlayer.put_AllowNetworking(_T("all"));
	}
}

//���ݵ�ǰ��Ƥ���������Ӧ��ȫ��Ƥ��
void CClientDlg::LoadCurMiniSkin()
{
	USES_CONVERSION;

	//��ǰƤ��
	std::string strCurSkin = "GUI.Skin.Current";
	const std::string strSkinItem = _config->getValueWithDefault(strCurSkin,"Default");

	//��ǰƤ����Ӧ��MINIƤ��
	std::string strMiniSkin = "GUI.MiniSkin.File.";
	strMiniSkin += strSkinItem;

	std::string	strSkinFile	= _config->getValueWithDefault(strMiniSkin,"mini.swf");

	CString strFlashInterfaceSkinFile;
	strFlashInterfaceSkinFile.Format(_T("%s\\%s"),m_SkinPath,A2T(strSkinFile.c_str()));
	m_FlashPlayer.LoadMovie(0, strFlashInterfaceSkinFile);

	BOOL bLoadFlashSkin =(m_FlashPlayer.PercentLoaded() >0);//PercentLoaded From 0 To 100
	if(bLoadFlashSkin)
	{
		m_FlashPlayer.Play();		
		m_FlashPlayer.put_AllowNetworking(_T("all"));
	}
}

//��Ӧ�л�Ƥ���˵�����
void CClientDlg::OnSwitchSkinRange( UINT nID )
{
	StringDict skinnames = _config->getValuesForPrefix("GUI.Skin.Name.");
	StringDict skinfiles = _config->getValuesForPrefix("GUI.Skin.File.");
	StringDict::iterator pi,pj;
	std::string	skinfile;

	USES_CONVERSION;

	TCHAR	szMenuItemName[64]	=_T("");
	int	nLen =GetMenuString(
		m_hSwitchSkinMenu,  // handle to the menu
		nID,				// menu item identifier
		szMenuItemName,		// pointer to the buffer for the string
		64,					// maximum length of the string
		MF_BYCOMMAND        // menu flags
		);

	for(pi = skinnames.begin(); pi != skinnames.end(); ++pi)
	{
		std::string skinname = pi->second;
		if(skinname ==T2A(szMenuItemName))
		{
			std::string	key	=pi->first;
			string_replace(key,"Name","File");
			skinfile =skinfiles[key];			
			break;
		}
	}

	m_nSwitchSkinID	= nID;

	CString	strMediaState	=GetFSVariable(_T("MediaState"));
	CString	strCanChangeProgressByExternProgram =GetFSVariable(_T("CanChangeProgressByExternProgram"));
	CString	strvideodur =GetFSVariable(_T("videodur"));	
	CString	strPosPercent =GetFSVariable(_T("PosPercent"));	
	CString	strvolume	=GetFSVariable(_T("volume"));	
	CString	strMute		=GetFSVariable(_T("mute"));	
	CString	strCanSeek	=GetFSVariable(_T("CanSeek"));

	LoadCurSkin(A2T(skinfile.c_str()));	

	//װ��Ƥ������Ҫ������Ƥ����״̬���ݵ�����

	SetFSVariable(_T("videodur"),strvideodur);	
	SetFSVariable(_T("PosPercent"),strPosPercent);	
	SetFSVariable(_T("volume"),strvolume);	
	SetFSVariable(_T("mute"),strMute);	
	SetFSVariable(_T("CanChangeProgressByExternProgram"),strCanChangeProgressByExternProgram);
	SetFSVariable(_T("MediaState"),strMediaState);
	SetFSVariable(_T("CanSeek"),strCanSeek);
}

//����Ƥ���˵���
void CClientDlg::OnUpdateSwitchSkinRange(CCmdUI* pCmdUI)
{
	if(pCmdUI->m_nID == m_nSwitchSkinID)
	{
		pCmdUI->SetCheck(TRUE);
	}
}

//��������Ϊ����Flash��ԭʼ��С
void CClientDlg::AutoFitFlashOriginSize()
{
	ASSERT(_config != NULL);

	//ԭʼƤ����С
	int nFlashSkinWidth = _config->getIntValueWithDefault("GUI.Skin.Width",590);
	int nFlashSkinHeight = _config->getIntValueWithDefault("GUI.Skin.Height",420);

	//�����Ի���Ĵ�С
	MoveWindow(0,0,nFlashSkinWidth,nFlashSkinHeight);
}

//��Ƶ��λ��
//ע�⣺�˺����еļ���VideoPos�ķ�������Flash����VideoPos��Χ��Ԫ���ڷ������Ƿ񱣳�
//��С���䣬������߿�Ĵ�С����
//����ļ��㷽���ȼٶ����С���ֲ���
//������Ҫ��Ӧ�ķ���
void CClientDlg::AdjustVideoWindow()
{
	if(m_pVideoPlayer != NULL)
	{
		ASSERT(_config != NULL);

		if(m_bFullScreen)
		{
			//MINI���ڿ�������С
			int	nFlashSkinWidth = _config->getIntValueWithDefault("GUI.MiniSkin.Width",769);
			int	nFlashSkinHeight = _config->getIntValueWithDefault("GUI.MiniSkin.Height",46);

			m_pVideoPlayer->GetVideoWindow()->MoveWindow(0,0,GetSystemMetrics( SM_CXSCREEN),GetSystemMetrics( SM_CYSCREEN) - nFlashSkinHeight);
		}
		else
		{
			//���ڴ�С
			int	nFlashSkinWidth = _config->getIntValueWithDefault("GUI.Skin.Width",590);
			int	nFlashSkinHeight = _config->getIntValueWithDefault("GUI.Skin.Height",420);

			//ԭʼ��Ƶλ��
			CRect rcOriginVideoWindow;
			rcOriginVideoWindow.left = _config->getIntValueWithDefault("GUI.Skin.WindowVideoPos.Left",2);
			rcOriginVideoWindow.top = _config->getIntValueWithDefault("GUI.Skin.WindowVideoPos.Top",32);
			rcOriginVideoWindow.right = _config->getIntValueWithDefault("GUI.Skin.WindowVideoPos.Right",588);
			rcOriginVideoWindow.bottom = _config->getIntValueWithDefault("GUI.Skin.WindowVideoPos.Bottom",365);

			//���±߾�����ұ߾�
			int nLeftRight = nFlashSkinWidth - rcOriginVideoWindow.right + rcOriginVideoWindow.left;
			int nTopBottom = nFlashSkinHeight - rcOriginVideoWindow.bottom + rcOriginVideoWindow.top;

			//���ڴ�С
			CRect rc;
			GetWindowRect(rc);

			//����������ʵ����Ƶ��С
			m_pVideoPlayer->GetVideoWindow()->SetWindowPos(NULL,rcOriginVideoWindow.left,rcOriginVideoWindow.top,rc.Width() - nLeftRight, rc.Height() - nTopBottom,0);
		}
	}
}

//ȫ��
void CClientDlg::SetFullScreen(BOOL bFullScreen)
{
	//���ⲿҪ��״̬Ϊ׼,�����ڲ�״̬��׼ȷ��
	if ( bFullScreen == m_bFullScreen ) return;

	//�����ǰ����ȫ��,�ͱ���һ�µ�ǰ�Ĵ��ڴ�С,�˳�ȫ����ʱ�ָ��˴�С
	if(m_bFullScreen == FALSE)
		GetWindowRect(&m_rcRestore);

	//��ȡ�õ�ǰ״̬
	CString	strMediaState	= GetFSVariable(_T("MediaState"));
	CString	strCanChangeProgressByExternProgram = GetFSVariable(_T("CanChangeProgressByExternProgram"));
	CString	strvideodur = GetFSVariable(_T("videodur"));	
	CString	strPosPercent = GetFSVariable(_T("PosPercent"));	
	CString	strvolume	= GetFSVariable(_T("volume"));	
	CString	strMute		= GetFSVariable(_T("mute"));	
	CString	strCanSeek	= GetFSVariable(_T("CanSeek"));

	//ֱ�Ӹ���״̬,����������Ҫ��״̬
	//�л��ڲ�״̬
	m_bFullScreen = bFullScreen;

	if ( m_bFullScreen )
	{
		//�ر�����
		SystemParametersInfo(SPI_GETSCREENSAVEACTIVE,0,&m_dwScreenSave,0);
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,0,NULL,0);

		//����Ϊ����㴰��,������������
		ModifyStyle(WS_CHILD, 0);
		SetParent(NULL);
		SetWindowPos(&wndTopMost, 0, 0, GetSystemMetrics( SM_CXSCREEN ),GetSystemMetrics( SM_CYSCREEN ), SWP_FRAMECHANGED|SWP_SHOWWINDOW );

		//������Ƥ��
		LoadCurMiniSkin();		
	}
	else
	{
		//������
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,m_dwScreenSave,NULL,0);

		//�ָ����ڴ�С
		ModifyStyle(0, WS_CHILD);
		SetWindowPos(&wndNoTopMost, m_rcRestore.left, m_rcRestore.top, m_rcRestore.Width(), m_rcRestore.Height(),SWP_FRAMECHANGED|SWP_SHOWWINDOW );

		//��ǰƤ��
		LoadCurSkin();
	}

	//���õ�ǰ״̬
	SetFSVariable(_T("videodur"),strvideodur);	
	SetFSVariable(_T("PosPercent"),strPosPercent);	
	SetFSVariable(_T("volume"),strvolume);	
	SetFSVariable(_T("mute"),strMute);
	SetFSVariable(_T("CanChangeProgressByExternProgram"),strCanChangeProgressByExternProgram);
	SetFSVariable(_T("MediaState"),strMediaState);		
	SetFSVariable(_T("CanSeek"),strCanSeek);
}

//////////////////////////////////////////////////////////////////////////
void CClientDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//��������Flash��С
	if(IsWindow(m_FlashPlayer)) 
	{
		//Flash�봰��ͬ��
		CRect	rc;
		GetClientRect(&rc);
		m_FlashPlayer.MoveWindow(rc); 

		if(!m_bFullScreen)
		{
			//��������,Բ��
			CRect rc1(rc);
			rc1.InflateRect(-1,-1,0,0);
			HRGN rgn = CreateRoundRectRgn(rc1.left,rc1.top,rc1.right,rc1.bottom,6,6);
			SetWindowRgn(rgn,TRUE); //��������,ȥ���߿�	
		} 
		else //ȫ��
		{
			//ԭ��������
			HRGN hrgn = CreateRectRgn(0,0,0,0);
			int regionType = GetWindowRgn(hrgn);
			if (regionType != ERROR)  
			{ 
				DeleteObject(hrgn);
			}

			//������,����
			HRGN rgn = CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);
			SetWindowRgn(rgn,TRUE); //��������,ȥ���߿�
		}
	}

	//������Ƶ��λ��
	if(m_pVideoPlayer != NULL)
	{
		AdjustVideoWindow();
	}
}

LRESULT CClientDlg::OnWmpOpenState(WPARAM x, LPARAM y)
{
	if(x == wmposMediaOpen)
	{
		m_pVideoPlayer->GetVideoWindow()->ShowWindow(SW_SHOW);
		m_FlashPlayer.SetWindowPos(m_pVideoPlayer->GetVideoWindow(),0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}

	TRACE("OnWmpOpenState:%d\n",x);

	return 0;
}

LRESULT CClientDlg::OnWmpPlayState(WPARAM x, LPARAM y)
{
	switch(x)
	{
	case wmppsPlaying:
		OnPlayingBegin();
		break;
	case wmppsStopped:
		OnPlayingEnd();
		break;
	case wmppsMediaEnded:
		OnPlayingEnd();
		break;
	}

	TRACE("OnWmpPlayState:%d\n",x);

	return 0;
}

LRESULT CClientDlg::OnWmpPosChange(WPARAM x, LPARAM y)
{
	return 0;
}

//�����ں���Ϣ����
void CClientDlg::OnPlayingBegin()
{
	SetFSVariable(_T("CanSeek"),_T("1"));
	SetFSVariable(_T("MediaState"),_T("1"));

	KillTimer(TRACKPOS_TIMER_ID);
	CString	strValue;
	double	dDuration = m_pVideoPlayer->GetDuration();
	SetFSVariable(_T("CanChangeProgressByExternProgram"), _T("1"));
	strValue.Format(_T("%f"),dDuration);
	SetFSVariable(_T("videodur"),strValue);	

	SetTimer(TRACKPOS_TIMER_ID,TRACKPOS_TIMER_ELAPSE,NULL);
}

void CClientDlg::OnPlayingEnd()
{
	KillTimer(TRACKPOS_TIMER_ID);

	CString strValue;
	strValue.Format(_T("%f"),0);
	SetFSVariable(_T("videopos"),strValue);

	SetFSVariable(_T("MediaState"),_T("2"));
}

//�˵�����
void CClientDlg::OnFileExit()
{
	KillTimer(TRACKPOS_TIMER_ID);
	m_pVideoPlayer->Stop();
	m_FlashPlayer.Stop();
	
	DestroyWindow();
}

void CClientDlg::OnFileOpen()
{	
	CFileDialog	FileDlg(TRUE);
	FileDlg.m_ofn.lpstrFilter = _T("Windows Media movies(*.avi;*.wmv;*.wmp;*.wm;*.asf)\0*.avi;*.wmv;*.wmp;*.wm;*.asf\0Windows Media audios(*.wma;*.wav;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi)\0*.wma;*.wav;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi\0All files(*.*)\0*.*\0\0");
	FileDlg.m_ofn.Flags |= OFN_ENABLESIZING;
	CString	strFile;
	if(FileDlg.DoModal() == IDOK)
	{
		strFile = FileDlg.GetPathName();
		m_pVideoPlayer->PlayUrl(strFile);
	}	
}

void CClientDlg::OnFileOpenurl()
{
	COpenUrlDlg	Dlg;
	if(Dlg.DoModal() ==IDOK)
	{
		if(Dlg.m_strComboURL.GetLength())
		{
			m_pVideoPlayer->PlayUrl(Dlg.m_strComboURL);
		}
	}
}

void CClientDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CClientDlg::OnHelpWebsite()
{
	USES_CONVERSION;
	std::string	strWebsite =_config->getValueWithDefault("GUI.Help.Website","http://www.gwu.edu");	
	ShellExecute(NULL, _T( "open" ),_T("explorer"), A2T(strWebsite.c_str()), NULL, SW_SHOWNORMAL ) ;	
}

void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent ==TRACKPOS_TIMER_ID && m_bCanChangeProgressByExternProgram)
	{

		//����������ֵ:����������,ȫ��,��ǰλ��
		double	newPositionPercent	= m_pVideoPlayer->GetPositionPercent();
		double	dDuration	= m_pVideoPlayer->GetDuration();
		double	dPosition = m_pVideoPlayer->GetPosition();

		TRACE("Timer: %f\n", dPosition);

		CString	strValue;

		strValue.Format(_T("%f"),newPositionPercent);
		SetFSVariable(_T("PosPercent"),strValue);					

		strValue.Format(_T("%f"),dPosition);
		SetFSVariable(_T("videopos"),strValue);

		strValue.Format(_T("%f"),dDuration);
		SetFSVariable(_T("videodur"),strValue);	
	}
}

//��Ӧ����Flash��״̬֪ͨ
void CClientDlg::OnFSCommandFlashPlayer(LPCTSTR command, LPCTSTR args) 
{	
	//���Ű�ť
	if( 0 == _tcsicmp(command,_T("play")) )
	{
		m_pVideoPlayer->Play();
	}
	//��ͣ��ť
	else if( 0 == _tcsicmp(command,_T("pause")) )
	{
		m_pVideoPlayer->Pause();
	}
	//ֹͣ��ť
	else if( 0 == _tcsicmp(command,_T("stop")) )
	{
		m_pVideoPlayer->GetVideoWindow()->SetWindowPos(&m_FlashPlayer,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		m_pVideoPlayer->GetVideoWindow()->ShowWindow(SW_HIDE);
		m_pVideoPlayer->Stop();
	}
	//ȫ����ť
	else if( 0 == _tcsicmp(command,_T("FullScreen")) )
	{
		SetFullScreen(TRUE);
	}
	else if( 0 == _tcsicmp(command,_T("ExitFullScreen")) )
	{
		SetFullScreen(FALSE);	
	}
	else if( 0 == _tcsicmp(command,_T("Mute")) )
	{
		m_pVideoPlayer->SetMute(_ttoi(args));
	}
	else if( 0 == _tcsicmp(command,_T("volume")) )
	{
		m_pVideoPlayer->SetVolume(_ttoi(args));
	}
	//����λ���϶�
	else if( 0 == _tcsicmp(command,_T("pospercent")) )
	{
		USES_CONVERSION;
		m_pVideoPlayer->SetPositionPercent(atof(T2A(args)));
	}
	//�Ƿ�����ⲿ�ı�״̬
	//����������Ƴ�ͻ
	else if( 0 == _tcsicmp(command,_T("CanChangeProgressByExternProgram")) )
	{
		TCHAR* lpszPos	=(TCHAR*)args;
		BOOL bCanChangeProgressByExternProgram = _tcstol(lpszPos,&lpszPos,10);
		if(bCanChangeProgressByExternProgram)
		{
			lpszPos++;
			double dTrackPositionPercent = _tcstod(lpszPos,&lpszPos);
			m_pVideoPlayer->SetPositionPercent(dTrackPositionPercent);

			CString	strValue;
			strValue.Format(_T("%f"),dTrackPositionPercent);
			SetFSVariable(_T("PosPercent"),strValue);	

			TRACE("CanChangeProgressByExternProgram: %s\n",strValue);
		}
		m_bCanChangeProgressByExternProgram = bCanChangeProgressByExternProgram;
	}
	else if( 0 == _tcsicmp(command,_T("quit")) )
	{
		OnFileExit();
	}
	else if( 0 == _tcsicmp(command,_T("exit")) )
	{
		OnFileExit();
	}
	else if( 0 == _tcsicmp(command,_T("Minimize")) )
	{
		ShowWindow(SW_MINIMIZE);
	}
	else if( 0 == _tcsicmp(command,_T("Restore")) )
	{
		ShowWindow(SW_RESTORE);
	}
	else if( 0 == _tcsicmp(command,_T("Maximize")) )
	{
		CRect	rect_max;
		ShowWindow(SW_SHOWMAXIMIZED);
		::SystemParametersInfo(SPI_GETWORKAREA,0,&rect_max,0);   
		MoveWindow(rect_max.left,rect_max.top,rect_max.Width(),rect_max.Height(),TRUE);
	}
	else if (0 == _tcsicmp(command,_T("WM_NCLBUTTONDOWN")) )
	{
		SendMessage(WM_NCLBUTTONDOWN,(WPARAM)HTCAPTION,MAKELPARAM( 5, 5));
	}
	else if( 0 == _tcsicmp(command,_T("Menu_File")) )
	{
		PopupFileMenu();
	}
	else if (0 == _tcsicmp(command,_T("Menu_help")) )
	{
		PopupHelpMenu();
	}	
	else if (0 == _tcsicmp(command,_T("Menu_SwitchSkin")) )
	{
		PopupSwitchSkinMenu();
	}
}

//�ļ��˵�
void	CClientDlg::PopupFileMenu()
{
	ASSERT(_config != NULL);

	CMenu	FileMenu;
	FileMenu.LoadMenu(IDR_MAINFRAME);
	CMenu*	pSubMenu	=FileMenu.GetSubMenu(0);

	int nFlashSkinMenuFileX = _config->getIntValueWithDefault("GUI.Skin.Menu.File.X",55);
	int nFlashSkinMenuFileY = _config->getIntValueWithDefault("GUI.Skin.Menu.File.Y",25);

	CPoint	mousePt(nFlashSkinMenuFileX,nFlashSkinMenuFileY);
	ClientToScreen(&mousePt);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,mousePt.x,mousePt.y,this,NULL);
}

//�����˵�
void	CClientDlg::PopupHelpMenu()
{
	ASSERT(_config != NULL);

	CMenu	FileMenu;
	FileMenu.LoadMenu(IDR_MAINFRAME);
	CMenu*	pSubMenu	=FileMenu.GetSubMenu(1);

	int nFlashSkinMenuFileX = _config->getIntValueWithDefault("GUI.Skin.Menu.Help.X",109);
	int nFlashSkinMenuFileY = _config->getIntValueWithDefault("GUI.Skin.Menu.Help.Y",25);

	CPoint	mousePt(nFlashSkinMenuFileX,nFlashSkinMenuFileY);
	ClientToScreen(&mousePt);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,mousePt.x,mousePt.y,this,NULL);
}

//���浯��ѡ��Ƥ���˵�
void	CClientDlg::PopupSwitchSkinMenu()
{
	ASSERT(_config != NULL);
	USES_CONVERSION;
	if(m_hSwitchSkinMenu)
	{
		DestroyMenu(m_hSwitchSkinMenu);
		m_hSwitchSkinMenu	=NULL;
	}
	m_hSwitchSkinMenu = ::CreatePopupMenu();

	StringDict skinnames = _config->getValuesForPrefix("GUI.Skin.Name.");
	StringDict skinorders = _config->getValuesForPrefix("GUI.Skin.Order.");
	StringDict::iterator p;

	int	nSkinCount	=skinorders.size();
	mappair**	pTempArry	=(mappair**)malloc(sizeof(mappair*)*nSkinCount);

	int		i	=0;
	for(p = skinorders.begin(),i=0; p != skinorders.end(); ++p,i++)
	{
		mappair*	pItem	=(mappair*)malloc(sizeof(mappair));
		strcpy(pItem->name,p->first.c_str());
		pItem->order	=atoi(p->second.c_str());
		pTempArry[i]	=pItem;
	}
	qsort(pTempArry,nSkinCount,sizeof(mappair*),&cmpOrder);
	for(i = 0;i<nSkinCount;i++)
	{
		std::string	key	=pTempArry[i]->name;
		string_replace(key,"Order","Name");
		std::string skinname =skinnames[key];	
		::AppendMenu(m_hSwitchSkinMenu,MF_STRING,ID_SWITCHSKIN_FIRST+i,A2T(skinname.c_str()));	
		if(m_nSwitchSkinID == ID_SWITCHSKIN_FIRST+i)
		{
			MENUITEMINFO	mmi;
			mmi.cbSize	=sizeof(MENUITEMINFO);
			mmi.wID		=m_nSwitchSkinID;
			mmi.fMask	=MIIM_STATE;
			mmi.fState	=MFS_CHECKED;
			::SetMenuItemInfo(m_hSwitchSkinMenu,m_nSwitchSkinID,FALSE,&mmi);
		}
		free(pTempArry[i]);
	}

	free(pTempArry);

#if 1
	CPoint	mousePt;
	GetCursorPos(&mousePt);
	mousePt.x -=4;
	mousePt.y +=8;
#else
	//int nFlashSkinMenuFileX = _config->getIntValueWithDefault("GUI.Skin.Menu.SwitchSkin.X",510);
	//int nFlashSkinMenuFileY = _config->getIntValueWithDefault("GUI.Skin.Menu.SwitchSkin.Y",25);
	//CPoint	mousePt(nFlashSkinMenuFileX,nFlashSkinMenuFileY);
	//ClientToScreen(&mousePt);
#endif
	TrackPopupMenu(m_hSwitchSkinMenu,TPM_LEFTALIGN,mousePt.x,mousePt.y,0,GetSafeHwnd(),NULL);
}

//�Ҽ��˵�
void	CClientDlg::PopupContextMenu()
{
	ASSERT(_config != NULL);

	CMenu	FileMenu;
	FileMenu.LoadMenu(IDR_CONTEXT);
	CMenu*	pSubMenu	=FileMenu.GetSubMenu(0);

	CPoint	pt;
	GetCursorPos(&pt);	
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this,NULL);
}

//��FlashƤ���õ�����
CString CClientDlg::GetFSVariable(LPCTSTR name)
{
	CString	strValue;
	if(m_bLoadFlashSkin)
	{
		try
		{
			strValue =m_FlashPlayer.GetVariable(name);
		}
		catch(...)
		{
			AfxMessageBox(_T("CClientDlg::GetFSVariable"));
		}
	}
	return strValue;
}

//��FlashƤ�����ñ���
void	CClientDlg::SetFSVariable(LPCTSTR name,LPCTSTR value)
{
	if(IsWindow(m_FlashPlayer))
	{
		try 
		{
			m_FlashPlayer.SetVariable(name,value);
		}
		catch(...)
		{
			AfxMessageBox(_T("CClientDlg::SetFSVariable"));
		}
	}
}

