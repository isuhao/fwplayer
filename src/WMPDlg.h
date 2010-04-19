#pragma once

#include "resource.h"
#include "wmpplayer.h"
#include "wmpcontrols.h"
#include "wmpsettings.h"
#include "wmpmedia.h"

#include "VideoPlayer.h"

// CWMPDlg dialog
class CWMPDlg : public CDialog, public CVideoPlayer
{
	DECLARE_DYNAMIC(CWMPDlg)

public:
	CWMPDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWMPDlg();
	
// Dialog Data
	enum { IDD = IDD_WMP_HOST };

	//初始化窗口
	BOOL InitPlayer(CWnd* pParent);
	virtual CWnd* GetVideoWindow();

	//播放一个URL
	//停止当前,播放新的
	void PlayUrl(LPCTSTR url);

	//播放过程控制
	//针对当前URL
	BOOL	Play();
	BOOL	Pause();
	BOOL	Stop();
	BOOL	Resume();

	//常规播放控制
	void	SetFullScreen();
	BOOL	GetFullScreen();

	BOOL	GetMute();
	void	SetMute(BOOL bMute);

	void	SetVolume( LONG nVolume );
	LONG	GetVolume();

	double	GetPosition();
	double	GetDuration();

	double	GetPositionPercent();
	void	SetPositionPercent( double dPercent);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual	BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

protected:
	DECLARE_EVENTSINK_MAP()

	afx_msg void PlayStateChangeWmplayer(long NewState);
	afx_msg void OpenStateChangeWmplayer(long NewState);
	afx_msg void StatusChangeWmplayer();
	afx_msg void OnPositionChangeWmplayer(double oldPosition, double newPosition);

private:
	CWMPPlayer		m_wmpPlayer;
	CWMPControls	m_wmpControls;
	CWMPSettings	m_wmpSettings;
	CWMPMedia		m_wmpMedia;
	BOOL			m_bInit;
};
