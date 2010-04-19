// DlgWMPHost.cpp : implementation file
//

#include "stdafx.h"
#include "WMPDlg.h"
#include "wmp.h"

// CWMPDlg dialog

IMPLEMENT_DYNAMIC(CWMPDlg, CDialog)

CWMPDlg::CWMPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWMPDlg::IDD, pParent)
{
	m_bInit	=FALSE;
}

CWMPDlg::~CWMPDlg()
{
	if(IsWindow(m_hWnd))
	{
		DestroyWindow();
	}
}

void CWMPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCX_WMP, m_wmpPlayer);
}


BEGIN_MESSAGE_MAP(CWMPDlg, CDialog)
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		ON_WM_MOVE()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CWMPDlg, CDialog)
	ON_EVENT(CWMPDlg, IDC_OCX_WMP, 5101, CWMPDlg::PlayStateChangeWmplayer, VTS_I4)
	ON_EVENT(CWMPDlg, IDC_OCX_WMP, 5001, CWMPDlg::OpenStateChangeWmplayer, VTS_I4)
	ON_EVENT(CWMPDlg, IDC_OCX_WMP, 5002, CWMPDlg::StatusChangeWmplayer, VTS_NONE)
	ON_EVENT(CWMPDlg, IDC_OCX_WMP, 5202, CWMPDlg::OnPositionChangeWmplayer, VTS_R8 VTS_R8)
END_EVENTSINK_MAP()

// CWMPDlg message handlers

BOOL CWMPDlg::OnInitDialog()
{
	BOOL	bRet	=FALSE;
	__super::OnInitDialog();
	
	m_bInit	=TRUE;
	return	TRUE;
}

void CWMPDlg::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);

	if(m_bInit)
	{
		CRect	rc;
		GetClientRect(&rc);
		m_wmpPlayer.MoveWindow(&rc);
	}
}

BOOL CWMPDlg::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

BOOL CWMPDlg::InitPlayer(CWnd* pParent)
{
	if(!::IsWindow(m_hWnd))
	{
		Create(CWMPDlg::IDD,pParent);
			
		if(::IsWindow(m_hWnd))
		{
			ShowWindow(SW_HIDE);

			//设置WMP的外观，如状态条等是否显示
			m_wmpPlayer.put_enableContextMenu(TRUE);
			m_wmpPlayer.put_stretchToFit(TRUE);
			m_wmpPlayer.put_windowlessVideo(FALSE);
			m_wmpPlayer.put_enableContextMenu(FALSE);
			m_wmpPlayer.put_uiMode(_T("none"));	//Possible Values:invisible,none,mini,full,custom
		}
	}

	return TRUE;
}

CWnd* CWMPDlg::GetVideoWindow()
{
	return this;
}

void CWMPDlg::PlayUrl(LPCTSTR url)
{
	if(m_bInit)
	{
		//启动播放
		m_wmpPlayer.put_URL(url);
		Play();
	}
}

BOOL CWMPDlg::Play()
{
	if(m_bInit)
	{
		m_wmpControls.AttachDispatch(m_wmpPlayer.get_controls());
		m_wmpControls.play();
		m_wmpControls.DetachDispatch();
	}

	return TRUE;
}
BOOL CWMPDlg::Stop()
{
	if(m_bInit)
	{
		m_wmpControls.AttachDispatch(m_wmpPlayer.get_controls());
		m_wmpControls.stop();
		m_wmpControls.DetachDispatch();
	}

	return TRUE;
}
//暂停节目播放
BOOL CWMPDlg::Pause()
{
	if(m_bInit)
	{
		m_wmpControls.AttachDispatch(m_wmpPlayer.get_controls());
		m_wmpControls.pause();
		m_wmpControls.DetachDispatch();
	}

	return TRUE;
}
BOOL CWMPDlg::Resume()
{
	if(m_bInit)
	{
		m_wmpControls.AttachDispatch(m_wmpPlayer.get_controls());
		m_wmpControls.play();
		m_wmpControls.DetachDispatch();
	}

	return TRUE;
}
void CWMPDlg::SetMute(BOOL bMute)
{
	if(m_bInit)
	{
		m_wmpSettings.AttachDispatch(m_wmpPlayer.get_settings());
		m_wmpSettings.SetMute(bMute);
		m_wmpSettings.DetachDispatch();
	}
}

BOOL CWMPDlg::GetMute()
{
	BOOL bMute = FALSE;
	LONG nVolume = 0;

	if(m_bInit)
	{
		m_wmpSettings.AttachDispatch(m_wmpPlayer.get_settings());
		bMute =m_wmpSettings.GetMute();
		m_wmpSettings.DetachDispatch();
	}

	return bMute;
}
LONG CWMPDlg::GetVolume()
{
	LONG nVolume = 0;

	if(m_bInit)
	{
		m_wmpSettings.AttachDispatch(m_wmpPlayer.get_settings());
		nVolume =m_wmpSettings.GetVolume();
		m_wmpSettings.DetachDispatch();
	}

	return nVolume;
}

void CWMPDlg::SetVolume( LONG nVolume )
{
	if(m_bInit)
	{
		m_wmpSettings.AttachDispatch(m_wmpPlayer.get_settings());
		m_wmpSettings.SetVolume(nVolume);
		m_wmpSettings.DetachDispatch();
	}
}

BOOL CWMPDlg::GetFullScreen()
{
	BOOL bFullScreen = FALSE;

	if(m_bInit)
	{
		bFullScreen =m_wmpPlayer.get_fullScreen();
	}

	return bFullScreen;
}

void CWMPDlg::SetFullScreen()
{
	if(m_bInit)
	{
		m_wmpPlayer.put_fullScreen(TRUE);
	}
}

double CWMPDlg::GetPosition()
{
	double dPosition = 0;

	if(m_bInit )
	{
		LPDISPATCH	lpwmpMediaDispatch	=m_wmpPlayer.get_currentMedia();
		if(lpwmpMediaDispatch)
		{
			m_wmpControls.AttachDispatch(m_wmpPlayer.get_controls());
			dPosition =m_wmpControls.GetCurrentPosition();
			m_wmpControls.DetachDispatch();	
			return ( dPosition);
		}
	}

	return	0.0;
}

double	CWMPDlg::GetDuration()
{
	double dDuration = 0;

	if(m_bInit )
	{
		LPDISPATCH	lpwmpMediaDispatch	=m_wmpPlayer.get_currentMedia();
		if(lpwmpMediaDispatch)
		{
			m_wmpMedia.AttachDispatch(lpwmpMediaDispatch);
			dDuration =m_wmpMedia.GetDuration();
			m_wmpMedia.DetachDispatch();

			return dDuration;
		}
	}

	return	0.0;
}

double CWMPDlg::GetPositionPercent()
{
	double dDuration = 0;
	double dPosition = 0;

	if(m_bInit )
	{
		LPDISPATCH	lpwmpMediaDispatch	=m_wmpPlayer.get_currentMedia();
		if(lpwmpMediaDispatch)
		{
			m_wmpMedia.AttachDispatch(lpwmpMediaDispatch);
			dDuration =m_wmpMedia.GetDuration();
			m_wmpMedia.DetachDispatch();

			m_wmpControls.AttachDispatch(m_wmpPlayer.get_controls());
			dPosition =m_wmpControls.GetCurrentPosition();
			m_wmpControls.DetachDispatch();	
			return ( dPosition/dDuration);
		}
	}

	return	0.0;
}

void CWMPDlg::SetPositionPercent( double dPercent)
{
	double dDuration = 0;

	if(m_bInit)
	{
		m_wmpMedia.AttachDispatch(m_wmpPlayer.get_currentMedia());
		dDuration =m_wmpMedia.GetDuration();
		m_wmpMedia.DetachDispatch();

		m_wmpControls.AttachDispatch(m_wmpPlayer.get_controls());
		m_wmpControls.SetCurrentPosition(dPercent*dDuration);
		m_wmpControls.DetachDispatch();	
	}
}

void CWMPDlg::PlayStateChangeWmplayer(long NewState)
{
	if(wmppsMediaEnded == NewState
		|| wmppsPlaying == NewState)
	{
		CWnd* pPlayPanel =(CWnd*)GetParent();
		if(pPlayPanel)
		{
			pPlayPanel->PostMessage(WM_VIDEO_PLAY_STATE,NewState);
		}
	}
}

void CWMPDlg::OpenStateChangeWmplayer(long NewState)
{
	if(NewState ==wmposMediaOpen)
	{
		CWnd* pPlayPanel =(CWnd*)GetParent();
		if(pPlayPanel)
		{
			pPlayPanel->PostMessage(WM_VIDEO_OPEN_STATE,NewState);
		}
	}
}

void CWMPDlg::StatusChangeWmplayer()
{
}
void CWMPDlg::OnPositionChangeWmplayer(double oldPosition, double newPosition) 
{
	CWnd* pPlayPanel =(CWnd*)GetParent();
	if(pPlayPanel)
	{
		pPlayPanel->PostMessage(WM_VIDEO_POS_CHANGE,newPosition, oldPosition);
	}
}
