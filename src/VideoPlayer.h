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
	
	//���ٶ���
	virtual void ReleasePlayer(){delete this;};

	//��ʼ�����Ŵ���
	virtual BOOL InitPlayer(CWnd* pParent) = 0;
	virtual CWnd* GetVideoWindow() = 0;

	//����һ��URL
	//ֹͣ��ǰ,�����µ�
	virtual void PlayUrl(LPCTSTR url) = 0;

	//���Ź��̿���
	//��Ե�ǰURL
	virtual BOOL	Play() = 0;
	virtual BOOL	Pause() = 0;
	virtual BOOL	Stop() = 0;
	virtual BOOL	Resume() = 0;

	//���沥�ſ���
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
