#pragma once

#define WM_VIDEO_PLAY_STATE	(WM_USER+131)
#define WM_VIDEO_OPEN_STATE	(WM_USER+132)
#define WM_VIDEO_POS_CHANGE	(WM_USER+133)

// CVideoPlayer Inteface

class CVideoPlayer
{

public:
	CVideoPlayer(){};
	virtual ~CVideoPlayer(){};
	
	//销毁对象
	virtual void ReleasePlayer(){delete this;};

	//初始化播放窗口
	virtual BOOL InitPlayer(CWnd* pParent) = 0;
	virtual CWnd* GetVideoWindow() = 0;

	//播放一个URL
	//停止当前,播放新的
	virtual void PlayUrl(LPCTSTR url) = 0;

	//播放过程控制
	//针对当前URL
	virtual BOOL	Play() = 0;
	virtual BOOL	Pause() = 0;
	virtual BOOL	Stop() = 0;
	virtual BOOL	Resume() = 0;

	//常规播放控制
	virtual void	SetFullScreen() = 0;
	virtual BOOL	GetFullScreen() = 0;

	virtual BOOL	GetMute() = 0;
	virtual void	SetMute(BOOL bMute) = 0;

	virtual void	SetVolume( LONG nVolume ) = 0;
	virtual LONG	GetVolume() = 0;

	virtual double	GetPosition() = 0;
	virtual double	GetDuration() = 0;

	virtual double	GetPositionPercent() = 0;
	virtual void	SetPositionPercent( double dPercent) = 0;
};
